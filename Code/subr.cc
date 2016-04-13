#include "object.hh"
#include "eval.hh"
#include "subr.hh"
#include <stdio.h>
#include <stdexcept>

Not_Subr::Not_Subr():
    runtime_error("Subroutine error") {}

Object do_plus(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a + b);
}

Object do_times(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a * b);
}

Object do_minus(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a - b);
}

Object do_car(Object lvals) {
  Object l = car(lvals);
  return car(l);
}

Object do_cdr(Object lvals) {
  Object l = car(lvals);
  return cdr(l);
}

Object do_cons(Object lvals) {
  Object h = car(lvals);
  Object t = cadr(lvals);
  return cons(h, t);
}

Object do_eq(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
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
	if (Object_to_string(f) == "-") {
		return do_minus(lvals);
	}
	if (Object_to_string(f) == "car") {
		return do_car(lvals);
	}
	if (Object_to_string(f) == "cdr") {
		return do_cdr(lvals);
	}
	if (Object_to_string(f) == "cons") {
		return do_cons(lvals);
	}
	if (Object_to_string(f) == "=") {
		return do_eq(lvals);
	}
  if (Object_to_string(f) == "newline") {
    printf("\n");
    return nil();
	}
	throw Not_Subr();
}

