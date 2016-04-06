#include "toplevel.hh"
#include <stdio.h>
#include "object.hh"
#include "env.hh"
#include "eval.hh"

extern Object just_read;
extern "C" int yyparse();
extern "C" FILE *yyin;

using namespace std;

int main() {
  toplevel();
}
