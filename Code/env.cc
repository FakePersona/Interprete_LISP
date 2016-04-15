#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>
#include "env.hh"

using namespace std;

Binding::Binding(string _name, Object _value):
  name(_name), value(_value) {}
string  Binding::get_name() const {
  return name;
}
Object  Binding::get_value() const {
  return value;
}

class No_Binding_Exception: public runtime_error {
private:
  string name;
public:
  No_Binding_Exception(string _name): runtime_error("No binding for name: " + _name) {
    name = _name;
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
  virtual ~Zipping_Exception() throw () {}
};

Environment::Environment():
  content(Binding("t", number_to_Object(1))), next(NULL) {}

Environment::Environment(Binding cont, Environment* nex):
  content(cont), next(nex)   {}


void Environment::add_new_binding(string name, Object value) {
  Environment* old_next = next;
  Environment new_env = Environment(content, old_next);
  next = &new_env;
  content = Binding(name,value);
}

void Environment::extend_env(Object lpars, Object lvals) {
  if (null(lpars) && null(lvals)) return;
  if (null(lpars) && !null(lvals)) throw Zipping_Exception(lvals, "Too many values");
  if (!null(lpars) && null(lvals)) throw Zipping_Exception(lpars, "Too many parameters");
  add_new_binding(Object_to_string(car(lpars)), car(lvals));
  extend_env(cdr(lpars), cdr(lvals));
}
Object Environment::find_value(string name) {
  if (content.get_name() == name)
    {
      return content.get_value();
    }
  else
    {
      if (next == NULL) throw No_Binding_Exception(name);
      return next->find_value(name);
    }
}

void Environment::print(ostream& s) {
  s << content.get_name() << ": " << content.get_value() << "; ";
  s << content.get_name() << ": " << content.get_value() << "; ";
  if (next)
    next->print(s);
}

ostream& operator << (ostream& s, Environment& env) {
  env.print(s);
  return s;
}
