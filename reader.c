#include "reader.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

static bool is_whitespace(char c) {
    return isspace(c) || c == ',';
}

Token *tokenize(const char *input) {
    Token *tokens = NULL;
    size_t size = 0;
    while (*input != '\0') {
        if (is_whitespace(*input)) {
            input++;
        } else if (*input == '(') {
            tokens = realloc(tokens, ++size * sizeof(Token));
            tokens[size - 1].type = T_LPAREN;
            input++;
        } else if (*input == ')') {
            tokens = realloc(tokens, ++size * sizeof(Token));
            tokens[size - 1].type = T_RPAREN;
            input++;
        } else { /* symbol or number } */
            size_t len = 1;
            while (isalnum(input[len])) {
                len++;
            }
            char *lexeme = strndup(input, len);
            input += len;

            tokens = realloc(tokens, ++size * sizeof(Token));
            if (isdigit(*lexeme)) {
                tokens[size - 1].type = T_NUMBER;
                tokens[size - 1].value.number = strtol(lexeme, NULL, 10);
                free(lexeme);
            } else {
                tokens[size - 1].type = T_SYMBOL;
                tokens[size - 1].value.symbol = lexeme;
            }
        }
    }
    tokens = realloc(tokens, ++size * sizeof(Token));
    tokens[size - 1].type = T_EOF;
    return tokens;
}

void free_tokens(Token *tokens) {
    for (size_t i = 0; tokens[i].type != T_EOF; i++) {
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
