#include "reader.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

static bool is_whitespace(char c) {
    return isspace(c) || c == ',';
}

Token* tokenize(const char* input) {
    Token* tokens = NULL;
    size_t size = 0;
    while (*input != '\0') {
        if (is_whitespace(*input)) {
            input++;
        } else if (*input == '(') {
            tokens = realloc(tokens, ++size * sizeof(Token));
            if (tokens == NULL) {
                perror("Error: unable to allocate memory");
                exit(EXIT_FAILURE);
            }
            Token t = {.type = T_LPAREN};
            tokens[size - 1] = t;
            input++;
        } else if (*input == ')') {
            tokens = realloc(tokens, ++size * sizeof(Token));
            if (tokens == NULL) {
                perror("Error: unable to allocate memory");
                exit(EXIT_FAILURE);
            }
            Token t = {.type = T_RPAREN};
            tokens[size - 1] = t;
            input++;
        } else {
            size_t len = 1;
            while (isalnum(input[len])) {
                len++;
            }
            char* lexeme = strndup(input, len);
            input += len;
            
            tokens = realloc(tokens, ++size * sizeof(Token));
            if (tokens == NULL) {
                perror("Error: unable to allocate memory");
                exit(EXIT_FAILURE);
            }
            Token t;
            if (isdigit(*lexeme)) {
                t.type = T_NUMBER;
                t.value.number = strtol(lexeme, NULL, 10);
            } else {
                t.type = T_SYMBOL;
                t.value.symbol = lexeme;
            }
            tokens[size - 1] = t;
        }
    }
    Token t = {.type = T_EOF};
    tokens = realloc(tokens, ++size * sizeof(Token));
    if (tokens == NULL) {
        perror("Error: unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    tokens[size - 1] = t;
    return tokens;
}

void free_tokens(Token *tokens) {
    printf("free tokens...\n");
    for (size_t i = 0; tokens[i].type != T_EOF; i++) {
        printf("%d\n", tokens[i].type);
        if (tokens[i].type == T_SYMBOL) {
            free(tokens[i].value.symbol);
        }
    }
    free(tokens);
}

Node *create_node(NodeType type, const void *value) {
    Node *node = malloc(sizeof(Node));
    node->type = type;
    switch (type) {
        case NODE_SYMBOL:
            node->value.symbol = strdup(value);
            break;
        case NODE_NUMBER:
            node->value.number = *(int *)value;
            break;
        case NODE_CONS:
            memcpy((void *)&node->value.cons, (void *)value, sizeof(ConsCell));
            break;
    }
    return node;
}

Node *cons(Node *car, Node *cdr) {
    if (car == NULL && cdr == NULL) {
        return NULL;
    }

    ConsCell cell = {car, cdr};
    return create_node(NODE_CONS, &cell);
}


Node *car(const Node *cons_node) {
    if (cons_node->type != NODE_CONS) {
        return NULL;
    }
    return cons_node->value.cons.car;
}

Node *cdr(const Node *cons_node) {
    if (cons_node->type != NODE_CONS) {
        return NULL;
    }
    return cons_node->value.cons.cdr;
}


void free_node(Node *node) {
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case NODE_SYMBOL:
            free(node->value.symbol);
            break;
        case NODE_CONS:
            free_node(node->value.cons.car);
            free_node(node->value.cons.cdr);
            break;
        default:
            // do nothing for NODE_NUMBER
            break;
    }
    free(node);
}

const Node *nil = NULL;

Node* parse(Token **tokens_ptr) {
  Token *tokens = *tokens_ptr;
  if (!tokens) return NULL;

  switch (tokens->type) {
    case T_LPAREN: {
      tokens++; // Consume T_LPAREN token
      Node *head = NULL, *tail = NULL;
      while (tokens->type != T_RPAREN && tokens->type != T_EOF) {
        Node *new_node = cons(parse(&tokens), NULL); // Recursively parse
        if (!head) {
          head = new_node;
          tail = new_node;
        } else {
          tail->value.cons.cdr = new_node;
          tail = new_node;
        }
      }
      if (tokens->type == T_RPAREN) {
        tokens++; // Consume T_RPAREN token
      }
      *tokens_ptr = tokens;
      return head;
    }
    case T_SYMBOL: {
      Node* node = create_node(NODE_SYMBOL, tokens->value.symbol);
      *tokens_ptr = tokens + 1;
      return node;
    }
    case T_NUMBER: {
      Node* node = create_node(NODE_NUMBER, &(tokens->value.number));
      *tokens_ptr = tokens + 1;
      return node;
    }
    default: {
      return NULL;
    }
  }
}

