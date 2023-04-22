#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
  T_LPAREN,
  T_RPAREN,
  T_SYMBOL,
  T_NUMBER,
  T_EOF,
} TokenType;

typedef struct Token {
  TokenType type;
  union {
    char* symbol;
    int number;
  } value;
} Token;

Token* tokenize(const char* input);
void free_tokens(Token* tokens);

typedef struct Node Node;
typedef struct ConsCell {
  Node* car;
  Node* cdr;
} ConsCell;

typedef enum {
  NODE_CONS,
  NODE_SYMBOL,
  NODE_NUMBER,
} NodeType;

struct Node {
  NodeType type;
  union {
    ConsCell cons;
    char* symbol;
    int number;
    void* data;
  } value;
    
};

Node* create_node(NodeType type, const void* value);
Node* cons(Node* car, Node* cdr);
Node* car(const Node* cons_node);
Node* cdr(const Node* cons_node);
void free_node(Node* node);
Node* parse(Token **tokens_ptr);

extern const Node *nil;


#endif // READER_H
