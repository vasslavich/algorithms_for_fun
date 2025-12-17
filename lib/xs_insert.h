#pragma once

struct Token{
    int val;
    Token *next;
    Token() : val(0){}
    Token(int val_) : val(val_){}
};

class XS_List{
public:
    Token* lookupLessLinear(Token * head, Token * tk){
      Token *prev = nullptr;
      while (head) {
        if (head->val >= tk->val)
          break;

        prev = head;
        head = head->next;
      }

      return prev; 
    }

    Token* insert(Token *head, Token* xs){
        Token* new_head = head;
        Token* prev = lookupLessLinear(head, xs);
        if(prev)
            prev->next = xs;
        else
            new_head = xs;
        
        xs->next = head;
        return new_head;
    }

    Token* go(Token * head){
        Token* xs_head = nullptr;
        while(head){
            xs_head = insert(xs_head, head);
            head = head->next;
        }

        return xs_head;
    }
};