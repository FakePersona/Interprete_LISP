<<<<<<< HEAD
#include "toplevel.hh"
=======
#include <stdio.h>
#include "object.hh"
#include "env.hh"
#include "eval.hh"

extern Object just_read;
extern "C" int yyparse();
extern "C" FILE *yyin;
>>>>>>> 9036985e371dc0ab15f1146ff3ded1b8cedce22d

using namespace std;

int main() {
  toplevel();
}
