#pragma once

#include <cassert>
#include <mutex>
#include <thread>
#include <vector>

template <typename T> class RingBufferLockFree {
private:
  std::vector<T> data_{};
  alignas(64) std::atomic<size_t> readIdx_{0};
  alignas(64) size_t writeIdxCached_{0};
  alignas(64) std::atomic<size_t> writeIdx_{0};
  alignas(64) size_t readIdxCached_{0};

public:
  RingBufferLockFree(size_t capacity, T init = {}) : data_(capacity, init) {}

  size_t getSize() const noexcept { return data_.size(); }

  size_t numFreeCells() const noexcept {
    auto const readIdx = readIdx_.load(std::memory_order_relaxed);
    auto const writeIdx = writeIdx_.load(std::memory_order_relaxed);
    if (writeIdx > readIdx)
      return writeIdx - readIdx;

    if (readIdx > writeIdx)
      return readIdx - writeIdx;

    if (readIdx == writeIdx)
      return data_.size();

    return 0;
  }

  bool isEmpty() const noexcept { return numFreeCells() == getSize(); }

  template <typename U = T> bool push(U &&v) {
    auto const writeIdx = writeIdx_.load(std::memory_order_relaxed);
    auto nextWriteIdx = (writeIdx + 1) % data_.size();
    if (nextWriteIdx == readIdxCached_) {
      readIdxCached_ = readIdx_.load(std::memory_order_acquire);
      if (nextWriteIdx == readIdxCached_) {
        return false;
      }
    }
    data_[writeIdx] = std::forward<U>(v);
    writeIdx_.store(nextWriteIdx, std::memory_order_release);
    return true;
  }

  template <typename U = T> bool pop(U &&val) {
    auto const readIdx = readIdx_.load(std::memory_order_relaxed);
    if (readIdx == writeIdxCached_) {
      writeIdxCached_ = writeIdx_.load(std::memory_order_acquire);
      if (readIdx == writeIdxCached_) {
        return false;
      }
    }
    std::swap(std::forward<U>(val), data_[readIdx]);
    auto nextReadIdx = (readIdx + 1) % data_.size();

    readIdx_.store(nextReadIdx, std::memory_order_release);
    return true;
  }
};

template <typename T> class RingBuffer {
public:
  RingBuffer(size_t size, T dflt = {}) : ring(size, dflt) {}

  template <typename U = T> bool push(U &&v) {
    bool wasInserted = false;
    auto stopTimer =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(100);

    while (!wasInserted && std::chrono::steady_clock::now() < stopTimer) {
      while (capacity >= getSize() &&
             std::chrono::steady_clock::now() < stopTimer)
        ;

      std::lock_guard lck{lock};
      if (capacity >= getSize())
        continue;

      ring[head] = std::forward<U>(v);

      head = (head + 1) % getSize();
      capacity++;

      wasInserted = true;
    }

    return wasInserted;
  }

  template <typename U = T> bool pop(U &&v) {
    return pop(std::addressof(v), 1);
  }

  size_t pop(T *v, size_t size) {
    bool tested = false;
    size_t numRetrived = 0;

    auto stopTimer =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(100);

    while (!tested && std::chrono::steady_clock::now() < stopTimer) {
      while (isEmpty() && std::chrono::steady_clock::now() < stopTimer)
        ;

      std::lock_guard lck(lock);
      if (!capacity)
        continue;

      int64_t cnt = std::min<int64_t>(size, capacity);
      while (numRetrived < cnt) {
        std::swap(v[numRetrived], ring[tail]);

        tail = (tail + 1) % getSize();
        --capacity;
        ++numRetrived;

        assert(capacity >= 0);
      }

      tested = true;
    }

    return numRetrived;
  }

  bool isEmpty() const noexcept { return 0 == capacity; }
  size_t getSize() const noexcept { return ring.size(); }

private:
  std::mutex lock;
  std::vector<T> ring;
  int64_t head = 0;
  int64_t tail = 0;
  int64_t capacity = 0;
};

void pinThread(int cpu) {
  if (cpu < 0) {
    return;
  }
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu, &cpuset);
  if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) ==
      -1) {
    perror("pthread_setaffinity_no");
    exit(1);
  }
}

template <typename T> void bench(int cpu1, int cpu2) {
  const size_t queueSize = 100000;
  const int64_t iters = 100000000;

  T q(queueSize);
  auto t = std::thread([&] {
    pinThread(cpu1);
    for (int i = 0; i < iters; ++i) {
      int val;
      while (!q.pop(val))
        ;
      if (val != i) {
        throw std::runtime_error("");
      }
    }
  });

  pinThread(cpu2);

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < iters; ++i) {
    while (!q.push(i))
      ;
  }
  while (!q.isEmpty())
    ;
  auto stop = std::chrono::steady_clock::now();
  t.join();
  std::cout << iters * 1000000000 /
                   std::chrono::duration_cast<std::chrono::nanoseconds>(stop -
                                                                        start)
                       .count()
            << " ops/s" << std::endl;
}

void TestRingBuf2() {
  int numCores = std::thread::hardware_concurrency();
  for (int i = 0; i < numCores; ++i) {
    for (int j = 0; j < numCores; ++j) {

      if (i == j)
        continue;

      bench<RingBuffer<int>>(0, 1);
      bench<RingBufferLockFree<int>>(0, 1);
    }
  }
}

void TestRingBuf(int Size) {
  RingBuffer<int> b(Size);
  std::thread w([&b, Size] {
    for (int i = 0; i < Size; ++i)
      b.push(i);
  });

  std::vector<int> result;
  std::thread r([&b, &result, Size] {
    int num = 0;
    while (num < Size) {
      int r[2];
      auto readCnt = b.pop(r, 2);

      for (int i = 0; i < readCnt; ++i)
        result.push_back(r[i]);

      num += readCnt;
    }
  });

  w.join();
  std::cout << "writers were joined\n";

  r.join();
  std::cout << "readres were joined\n";

  int x = 0;
  for (auto v : result) {
    if (v != x++) {
      throw std::runtime_error{"wrong result"};
    }
  }

  std::cout << "all checks were passed\n";
}