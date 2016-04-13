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
	Object a = car(lvals);
	Object b = cadr(lvals);
	if (is_empty(a) && is_empty(b)){
		return bool_to_Object(true);
	}
	if (numberp(a) && numberp(b)){
		return bool_to_Object(Object_to_number(a) == Object_to_number(b));
	}
	if (symbolp(a) && symbolp(b)){
		return bool_to_Object(Object_to_string(a) == Object_to_string(b));
	}
	if (stringp(a) && stringp(b)){
		return bool_to_Object(Object_to_string(a) == Object_to_string(b));
	}
	if (listp(a) && listp(b)){
		return bool_to_Object(a == b);
		/*return bool_to_Object((Object_to_bool(do_eq(car(a), car(b))))
							&& Object_to_bool(do_eq(cdr(a), cdr(b))));*/
	}
	return bool_to_Object(false);
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
	throw Not_Subr();
}

