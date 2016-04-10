#include <stdexcept>
#include <string>
#include <cassert>
#include "eval.hh"
#include "subr.hh"

using namespace std;

bool numberp(Object l) {
  return l -> is_number();
}

bool stringp(Object l) {
  return l -> is_string();
}

bool symbolp(Object l) {
  return l -> is_symbol();
}

bool listp(Object l) {
  return l -> is_pair();
}

Object cadr(Object l) {
  return car(cdr(l));
}

Object cddr(Object l) {
  return cdr(cdr(l));
}

Object caddr(Object l) {
  return car(cddr(l));
}

Object cdddr(Object l) {
  return cdr(cddr(l));
}

Object cadddr(Object l) {
  return car(cdddr(l));
}

class Evaluation_Exception: public runtime_error {
private:
  Object obj;
  Environment env;
  string message;
public:
  Evaluation_Exception(Object _obj, Environment _env, string _message):
    runtime_error("Evaluation error:" + _message) {
    obj = _obj;
    env = _env;
    message = _message;
  }
  virtual ~Evaluation_Exception() throw () {}
};

Object eval(Object l, Environment env);
Object apply(Object f, Object lvals, Environment env);
Object eval_list(Object largs, Environment env);

Object do_progn(Object lvals, Environment env) {
  if (null(cdr(lvals)))
    {
      return eval(car(lvals),env);
    }
  else
    {
      eval(car(lvals),env);
      return do_progn(cdr(lvals),env);
    }
}

Object do_if(Object l, Environment env) {
  Object test_part = cadr(l);
  Object then_part = caddr(l);
  Object else_part = cadddr(l);
  Object test_value = eval(test_part, env);
  if (null(test_value)) return eval(else_part, env);
  return eval(then_part, env);
}

Object do_andthen(Object lvals, Environment env) {
  Object part1 = car(lvals);
  Object part2 = cadr(lvals);
  if  (!null(eval(part1,env)))
    {
      return eval(part2,env);
    }
  else
    {
      return nil();
    }
}


Object eval(Object l, Environment env) {
  clog << "\teval: " << l << env << endl;

  if (null(l)) return l;
  if (numberp(l)) return l;
  if (stringp(l)) return l;
  if (symbolp(l)) return env.find_value(Object_to_string(l));
  assert(listp(l));
  Object f = car(l);
  if (symbolp(f)) {
    if (Object_to_string(f) == "lambda") return l;
    if (Object_to_string(f) == "quote") return cadr(l);
    if (Object_to_string(f) == "andthen") return do_andthen(cdr(l),env);
    if (Object_to_string(f) == "if") return do_if(l,env);
    if (Object_to_string(f) == "progn") return do_progn(cdr(l),env);
  }
  // It is a function applied to arguments
  Object vals = eval_list(cdr(l), env);
  return apply(f, vals, env);
}

Object eval_list(Object largs, Environment env) {
  if (null(largs)) return largs;
  return cons(eval(car(largs), env), eval_list(cdr(largs), env));
}


Object apply(Object f, Object lvals, Environment env) {
  clog << "\tapply: " << f << " " << lvals << env << endl;

  if (null(f)) throw Evaluation_Exception(f, env, "Cannot apply nil");
  if (numberp(f)) throw Evaluation_Exception(f, env, "Cannot apply a number");
  if (stringp(f)) throw Evaluation_Exception(f, env, "Cannot apply a string");
  if (symbolp(f)) {
	try{
		return handle_subr(f, lvals);
	}catch (Not_Subr){
		Object new_f = env.find_value(Object_to_string(f));
		return apply(new_f, lvals, env);
	}
  }
  if (Object_to_string(car(f)) == "lambda") {
    Object lpars = cadr(f);
    Object body = caddr(f);
    Environment new_env = env;
    new_env.extend_env(lpars, lvals);
    return eval(body, new_env);
  }
  assert(listp(f));
  throw Evaluation_Exception(f, env, "Cannot apply a list");
  assert(false);
}
