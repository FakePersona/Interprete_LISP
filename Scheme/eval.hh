#pragma once

#include "object.hh"
#include "env.hh"
#include <stdexcept>

using namespace std;


bool numberp(Object l);
bool stringp(Object l);
bool symbolp(Object l);
bool listp(Object l);

Object cadr(Object l);
Object cddr(Object l);
Object caddr(Object l);
Object cdddr(Object l);
Object cadddr(Object l);

Object eval(Object l, Frame env);
Object apply(Object f, Object lvals, Frame env);
Object eval_list(Object largs, Frame env);
