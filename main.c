#include "evaluator.h"
#include "reader.h"
#include "print.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

  const char *input = "(foo, (bar, (baz))) (hurr)";
      
  Token *tokens = tokenize(input);
  Node *lst = parse(&tokens);

  print_node(lst);
  /* print_node(car(lst)); */
  
   free_tokens(tokens);
    
    return 0;
}






  /* const char *input = "(foo, (bar, (baz))) (hurr)"; */
      
  /* Token *tokens = tokenize(input); */
  /* for (Token *token = tokens; token->type != T_EOF; token++) { */
  /*   print_token(token); */
  /* } */
  /* free_tokens(tokens); */

    // Create the list (1 2 3)
    /* Node *node1 = create_node(NODE_NUMBER, &(int){1}); */
    /* Node *node2 = create_node(NODE_NUMBER, &(int){2}); */
    /* Node *node3 = create_node(NODE_NUMBER, &(int){3}); */
    /* Node *lst = cons(create_node(NODE_SYMBOL, "+"), cons(node1, cons(node2, cons(node3, NULL))) ); */
    
    /* /\* Node *lst = cons(create_node(NODE_SYMBOL, "+"), cons((cons(create_node(NODE_SYMBOL, "bar"), create_node(NODE_SYMBOL, "baz"))), cons(node2, cons(node3, NULL))) ); *\/ */
    
    /* print_node(lst); */
    /* free_node(lst); */
