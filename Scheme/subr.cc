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

Object do_read(Object lvals) {
  Object l = read();
  return l;
}

void init_subr(Environment subr_env){
  Object plus_subr = new Cell();
  plus_subr->make_cell_subr(do_plus);
	subr_env.add_new_binding("+", plus_subr);

  Object times_subr = new Cell();
  times_subr->make_cell_subr(do_times);
	subr_env.add_new_binding("*", times_subr);

  Object minus_subr = new Cell();
  minus_subr->make_cell_subr(do_minus);
	subr_env.add_new_binding("-", minus_subr);

  Object div_subr = new Cell();
  div_subr->make_cell_subr(do_div);
	subr_env.add_new_binding("/", div_subr);

  Object car_subr = new Cell();
  car_subr->make_cell_subr(do_car);
	subr_env.add_new_binding("car", car_subr);

  Object cdr_subr = new Cell();
  cdr_subr->make_cell_subr(do_cdr);
	subr_env.add_new_binding("cdr", cdr_subr);

  Object cons_subr = new Cell();
  cons_subr->make_cell_subr(do_cons);
	subr_env.add_new_binding("cons", cons_subr);

  Object eq_subr = new Cell();
  eq_subr->make_cell_subr(do_eq);
	subr_env.add_new_binding("=", eq_subr);

  Object concat_subr = new Cell();
  concat_subr->make_cell_subr(do_concat);
	subr_env.add_new_binding("concat", concat_subr);

  Object read_subr = new Cell();
  read_subr->make_cell_subr(do_read);
	subr_env.add_new_binding("read", read_subr);
}

Object handle_subr(Object f,Object lvals, Environment env) {
	return ((env.find_value(Object_to_string(f)))->to_subr())(lvals);
}

