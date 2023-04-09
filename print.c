#include <stdio.h>
#include "reader.h"
#include "print.h"

void print_number(const Node *node) {
    printf("%d", node->value.number);
}

void print_symbol(const Node *node) {
    printf("%s", node->value.symbol);
}

void print_cons(const Node *node) {
    putchar('(');
    print_node(car(node));
    const Node *cdr_node = cdr(node);
    while (cdr_node != NULL) {
        if (cdr_node->type != NODE_CONS) {
            printf(" . ");
            print_node(cdr_node);
            break;
        } else {
            putchar(' ');
            print_node(car(cdr_node));
            cdr_node = cdr(cdr_node);
        }
    }
    putchar(')');
}

void print_node(const Node *node) {
    switch (node->type) {
        case NODE_CONS:
            print_cons(node);
            break;
        case NODE_SYMBOL:
            print_symbol(node);
            break;
        case NODE_NUMBER:
            print_number(node);
            break;
        default:
            printf("Unknown node type %d", node->type);
            break;
    }
}

void print_token(const Token *token) {
  switch (token->type) {
  case T_LPAREN:
    printf("T_LPAREN\n");
    break;
  case T_RPAREN:
    printf("T_RPAREN\n");
    break;
  case T_SYMBOL:
    printf("T_SYMBOL: %s\n", token->value.symbol);
    break;
  case T_NUMBER:
    printf("T_NUMBER: %d\n", token->value.number);
    break;
  case T_EOF:
    printf("T_EOF\n");
    break;
  }
}
