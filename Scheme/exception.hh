/*! \file exception.hh */ 

#include "object.hh"
#include "env.hh"
#include "eval.hh"
#include "read.hh"
#include "toplevel.hh"
#include <stdio.h>
#include <stdexcept>

/** Environment Exceptions*/

class No_Binding_Exception: public runtime_error {
private:
  string name;
public:
  No_Binding_Exception(string _name): runtime_error("No binding for name: " + _name) {
    name = _name;
  }
  void message() {
    cout << "No binding for name: " <<  name << endl;
  }
  virtual ~No_Binding_Exception() throw () {}
};

class Zipping_Exception: public runtime_error {
private:
  string message;
  Object lobjs;
public:
  Zipping_Exception(Object _lobjs, string _message): runtime_error("Zipping exception: " + _message) {
    message = _message;
    lobjs = _lobjs;
    clog << message << ": " << lobjs << endl;
  }
  void error_message() {
    cout << message << ": " << lobjs  << endl;
  }
  virtual ~Zipping_Exception() throw () {}
};


/** Subr exceptions*/

class Not_Subr: public runtime_error{
	public:
		Not_Subr():runtime_error("Subroutine error") {}
};

Object handle_subr(Object f,Object lvals);


/** Eval exceptions*/

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
  void error_message() {
    cout << "Evaluation error: " + message << endl;
  }
  virtual ~Evaluation_Exception() throw () {}
};


/** toplevel exceptions*/

class Continue_Directive: public runtime_error {
public:
  Continue_Directive():
    runtime_error("Directive error") {
  }
};
