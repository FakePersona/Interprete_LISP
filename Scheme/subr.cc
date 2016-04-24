/*! \file subr.cc */ 

#include "object.hh"
#include "eval.hh"
#include "subr.hh"
#include "exception.hh"
#include <stdio.h>
#include <stdexcept>

/******************/
/* Subr functions */
/******************/

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

Object do_div(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a / b);
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
	}
	return bool_to_Object(false);
}

Object do_concat(Object lvals) {
	Object a = car(lvals);
	Object b = cadr(lvals);
	return string_to_Object(Object_to_string(a) + Object_to_string(b));
}

Object do_newline(Object lvals) {
  printf("\n");
  return nil();
}

//!
//! This functions takes lvals as argument  because of type issues
//!
Object do_read(Object lvals) {
  Object l = read();
  return l;
}

/**********************/
/* Subroutine binding */
/**********************/

//!
//! Creates the binding pertaining to a subr function
//!
void bind_subr(Environment subr_env, string name, Object(*subr_fun)(Object)) {
  Object subr = new Cell();
  subr->make_cell_subr(subr_fun);
	subr_env.add_new_binding(name, subr);
}

//!
//! Initialise environment with subroutines
//!
void init_subr(Environment subr_env){
	bind_subr(subr_env, "+", do_plus);
  bind_subr(subr_env, "*", do_times);
  bind_subr(subr_env, "-", do_minus);
  bind_subr(subr_env, "/", do_div);
  bind_subr(subr_env, "car", do_car);
  bind_subr(subr_env, "cdr", do_cdr);
  bind_subr(subr_env, "cons", do_cons);
  bind_subr(subr_env, "=", do_eq);
  bind_subr(subr_env, "concat", do_concat);
  bind_subr(subr_env, "read", do_read);
  bind_subr(subr_env, "newline", do_newline);
}
