#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>
#include "env.hh"

using namespace std;

/********************/
/* Binding function */
/********************/

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

/* Environment functions */

EnvBlock::EnvBlock():
  content(Binding("t", number_to_Object(1))), next(NULL) {}

EnvBlock::EnvBlock(Binding cont, EnvBlock* nex):
  content(cont), next(nex) {}

Binding EnvBlock::get_content() {
  return content;
}

EnvBlock* EnvBlock::get_next() {
  return next;
}

void EnvBlock::set_next(EnvBlock* nex) {
  next = nex;
}

Environment::Environment() {
  head  = NULL;
}

Environment::Environment(const Environment & source) {
  head = NULL;
  EnvBlock* writing = head;
  EnvBlock* reading = source.head;
  while (reading)
    {
      EnvBlock* copy = new EnvBlock(reading->get_content(),NULL);

      if (copy != NULL)
        {
          if (head == NULL)
            head = copy;
          else
            {
              writing->set_next(copy);
            }
          writing = copy;
          reading = reading->get_next();
        }
      else
        {
          reading = NULL;
        }
    }

}

Environment::~Environment() {
  while (head)
    {
      EnvBlock* killed = head;
      head = head->get_next();
      delete killed;
    }
}

void Environment::add_new_binding(string name, Object value) {
  EnvBlock* new_head = new EnvBlock(Binding(name,value), head);
  head = new_head;
}

void Environment::extend_env(Object lpars, Object lvals) {
  if (null(lpars) && null(lvals)) return;
  if (null(lpars) && !null(lvals)) throw Zipping_Exception(lvals, "Too many values");
  if (!null(lpars) && null(lvals)) throw Zipping_Exception(lpars, "Too many parameters");
  add_new_binding(Object_to_string(car(lpars)), car(lvals));
  extend_env(cdr(lpars), cdr(lvals));
}
Object Environment::find_value(string name) {
  EnvBlock* searching = head;

  while (searching)
    {
      if (searching == NULL)
        {
          throw No_Binding_Exception(name);
        }
      if (searching->get_content().get_name() == name)
        {
          return searching->get_content().get_value();
        }
      else
        {
          searching = searching->get_next();
        }
    }
}

void Environment::print(ostream& s) {
  EnvBlock* printing = head;
  while (printing)
    {
      s << printing->get_content().get_name() << ": " << printing->get_content().get_value() << "; ";
      printing = printing->get_next();
    }
}

ostream& operator << (ostream& s, Environment& env) {
  env.print(s);
  return s;
}
