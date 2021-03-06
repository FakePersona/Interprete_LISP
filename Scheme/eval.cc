/*! \file eval.cc */ 

#include <stdexcept>
#include <string>
#include <cassert>
#include "eval.hh"
#include "toplevel.hh"
#include "subr.hh"
#include "exception.hh"

using namespace std;

/*****************************/
/* Basic Object manipulators */
/*****************************/

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

/****************************/
/* Pertaining to evaluation */
/****************************/

Object eval(Object l, Environment env);
Object apply(Object f, Object lvals, Environment env);
Object eval_list(Object largs, Environment env);

/* Auxiliarry functions called in evaluation */

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

Object do_if(Object lvals, Environment env) {
  Object test_part = car(lvals);
  Object then_part = cadr(lvals);
  Object else_part = caddr(lvals);
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

Object do_cond(Object lvals, Environment env) {
  if (null(lvals))
    {
      return nil();
    }
  else
    {
      Object test_part = car(car(lvals));
      Object then_part = cadr(car(lvals));
      if (!null(eval(test_part,env)))
        {
          return eval(then_part,env);
        }
      else
        {
          return do_cond(cdr(lvals), env);
        }
    }
}

/* Actual evaluating functions */

//!
//! This function does the actual evaluating of object l in Environment env
//!
Object eval_aux(Object l, Environment env) {

  if (null(l)) return l;
  if (numberp(l)) return l;
  if (stringp(l)) return l;
  if (symbolp(l)) return env.find_value(Object_to_string(l));
  assert(listp(l));
  Object f = car(l);
  if (symbolp(f)) {
    if (Object_to_string(f) == "lambda")
      {
        return env.make_closure(l);; // We are capturing the environment
      }
    if (Object_to_string(f) == "quote") return cadr(l);
    if (Object_to_string(f) == "andthen") return do_andthen(cdr(l),env);
    if (Object_to_string(f) == "if") return do_if(cdr(l),env);
    if (Object_to_string(f) == "cond") return do_cond(cdr(l),env);
    if (Object_to_string(f) == "progn") return do_progn(cdr(l),env);
    if (Object_to_string(f) == "debug") {
      debug = true;
      return nil();
    }
    if (Object_to_string(f) == "printenv") {
      clog << env << endl;
      return nil();
    }

  }
  // It is a function applied to arguments
  Object vals = eval_list(cdr(l), env);
  return apply(f, vals, env);
}

//!
//! Wrapper functions mainly in charge of tracing evaluation if necessary.
//!
Object eval(Object l, Environment env) {
  if (!debug)
    {
      return eval_aux(l,env);
    }
  else
    {
      int i;
      string header = "";
      for (i=0;i<(counter + 1);i++)
        {
          header +="\t";
        }
      clog << header << counter << " ----> " << l << "\t" << env << endl;
      counter++;

      Object new_l = eval_aux(l,env);
      counter--;
      header = "";
      for (i=0;i<(counter + 1);i++)
        {
          header +="\t";
        }
      clog << header << counter << " <---- " << new_l << "\t" << env << endl;
      return new_l;
    }
}

//!
//! Evaluates the elements of the list
//!
Object eval_list(Object largs, Environment env) {
  if (null(largs)) return largs;
  return cons(eval(car(largs), env), eval_list(cdr(largs), env));
}

/*********************/
/* Apllying function */
/*********************/

//!
//! Tries to apply f to lvals in Environment env
//!
Object apply(Object f, Object lvals, Environment env) {

  if (null(f)) throw Evaluation_Exception(f, env, "Cannot apply nil");
  if (numberp(f)) throw Evaluation_Exception(f, env, "Cannot apply a number");
  if (stringp(f)) throw Evaluation_Exception(f, env, "Cannot apply a string");
  if (symbolp(f)) {
		Object new_f = env.find_value(Object_to_string(f));
    if (new_f->is_subr()) // f needs to be handled separately if it is a subr
      return (new_f->to_subr())(lvals);
		return apply(new_f, lvals, env);
  }
  if (Object_to_string(car(f)) == "lambda") {
    Object lpars = cadr(f);
    Object body = caddr(f);
    Frame* new_frame = new Frame(env.get_observing()); // we cannot branch directly on the captured frame
    Environment new_env = Environment(new_frame);
    new_env.extend_env(lpars, lvals);
    return eval(body, new_env);
  }
  if (Object_to_string(car(f)) == "closure") {
    Environment new_env = Object_to_env(f);
    Object body = cadr(f);
    return apply(body, lvals, new_env);
  }
  assert(listp(f));
  throw Evaluation_Exception(f, env, "Cannot apply a list");
  assert(false);
}
