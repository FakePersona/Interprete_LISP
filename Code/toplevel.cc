#include "object.hh"
#include "env.hh"
#include "eval.hh"
#include "read.hh"
#include "toplevel.hh"
#include <stdio.h>
#include <stdexcept>

bool debug = false;



class Continue_Directive: public runtime_error {
public:
  Continue_Directive():
    runtime_error("Directive error") {
  }
};

void handle_directive(Object l,Environment* env) {
  if (listp(l) && Object_to_string(car(l)) == "setq") {
    Object bound_symbol = cadr(l);
    Object bound_part = caddr(l);
    env->add_new_binding(Object_to_string(bound_symbol),eval(bound_part,*env));
    throw Continue_Directive();
  }
}

void toplevel()
{
  Object a =  symbol_to_Object("a");
  Object one = number_to_Object(1);
  Object two = number_to_Object(2);

  Environment env;
  env.add_new_binding(Object_to_string(a), one);
  env.add_new_binding(Object_to_string(a), two);

  do {
    cout << "Lisp? " << flush;
    try {
    Object l = read();
    handle_directive(l,&env);
    cout << eval(l, env) << endl;
    } catch (Continue_Directive){

    } catch (Evaluation_Exception){
      
    } 
  } while (!end_input());
}

