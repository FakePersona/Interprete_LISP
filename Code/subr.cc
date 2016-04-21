#include "object.hh"
#include "eval.hh"
#include "subr.hh"
#include "exception.hh"
#include <stdio.h>
#include <stdexcept>


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

Object do_concat(Object lvals) {
	Object a = car(lvals);
	Object b = cadr(lvals);
	return string_to_Object(Object_to_string(a) + Object_to_string(b));
}

Object do_read() {
  char entry[256];
  scanf("%[^\n]s", entry);
  Object l = string_to_Object(entry);
  return l;
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
	if (Object_to_string(f) == "concat") {
		return do_concat(lvals);
	}
	if (Object_to_string(f) == "read") {
		return do_read();
	}
	throw Not_Subr();
}

