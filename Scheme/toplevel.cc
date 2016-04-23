#include "object.hh"
#include "env.hh"
#include "eval.hh"
#include "read.hh"
#include "subr.hh"
#include "toplevel.hh"
#include "exception.hh"
#include <stdio.h>
#include <stdexcept>

bool debug = false;
int counter = 0;

void handle_directive(Object l,Environment* env) {
  if (listp(l) && Object_to_string(car(l)) == "setq") {
    Object bound_symbol = cadr(l);
    Object bound_part = caddr(l);
    env->set_new_binding(Object_to_string(bound_symbol),eval(bound_part,*env));
    throw Continue_Directive();
  }
  if (listp(l) && Object_to_string(car(l)) == "define") {
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

  Environment env = Environment();

  init_subr(env);

  env.set_new_binding(Object_to_string(a), one);

  do {
    cout << "Lisp? " << flush;
    try {
    Object l = read();
    handle_directive(l,&env);
    cout << eval(l, env) << endl;
    } catch (Continue_Directive){

    } catch (Evaluation_Exception){

    }
      catch (No_Binding_Exception){
        printf("not bound\n");
	}
	  catch (Zipping_Exception){
	}
	  catch (Not_Subr){
	}


  } while (!end_input());
}
