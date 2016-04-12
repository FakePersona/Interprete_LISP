#include "object.hh"
#include "eval.hh"
#include "subr.hh"
#include <stdio.h>
#include <stdexcept>

Not_Subr::Not_Subr():
    runtime_error("Subroutine error") {}

Object do_times(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a * b);
}

Object do_plus(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a + b);
}

Object do_eq(Object lvals) {
	Object a = car(lvals);
	Object b = cadr(lvals);
	if (a == b){
		return number_to_Object(1);
	}
	else{
		return nil();
	}
}

Object handle_subr(Object f,Object lvals){
	if (Object_to_string(f) == "+") {
		return do_plus(lvals);
	}
	if (Object_to_string(f) == "*") {
		return do_times(lvals);
	}
	if (Object_to_string(f) == "=") {
		return do_eq(lvals);
	}
	throw Not_Subr();
}

