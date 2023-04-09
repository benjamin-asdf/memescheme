#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <stdbool.h>

typedef struct Scheme_Object {
    enum { T_PAIR, T_ATOM, T_EVAL_NUMBER, T_EVAL_SYMBOL, T_FUNCTION, T_NIL } type;
    union { struct { struct Scheme_Object *car; struct Scheme_Object *cdr; } pair; int number; char *symbol; void *function; } value;
} Scheme_Object;

typedef struct Environment {
    struct Environment *parent;
    Scheme_Object *bindings;
} Environment;

Environment *make_environment(Environment *parent);
Scheme_Object *lookup_binding(Environment *env, const char *symbol);
bool define_binding(Environment *env, const char *symbol, Scheme_Object *value);

Scheme_Object *eval(Scheme_Object *expr, Environment *env);

#endif // EVALUATOR_H
