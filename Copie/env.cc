#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>
#include "env.hh"
#include "exception.hh"

using namespace std;

/********************/
/* Binding function */
/********************/


//!
//! Eseentially the same as in Scheme
//!
Binding::Binding(string _name, Object _value):
  name(_name), value(_value) {}

//!
//! Eseentially the same as in Scheme
//!

string  Binding::get_name() const {
  return name;
}

//!
//! Eseentially the same as in Scheme
//!

Object  Binding::get_value() const {
  return value;
}

//!
//! Eseentially the same as in Scheme
//!
void Binding::set_value(Object _value) {
  value = _value;
}

//!
//! Stores a binding in a cell_pair
//!
Object Binding::to_Object() {
  Object bound_name = new Cell();
  bound_name->make_cell_string(name);

  Object bound_object = new Cell();
  bound_object->make_cell_pair(bound_name, value);

  return bound_object;
}

//!
//! Recreates binding from suitable cell_pair
//!
Binding Object_to_binding(Object b) {
  return Binding(Object_to_string(car(b)), cdr(b));
}

/*************************/
/* Environment functions */
/*************************/

//!
//! Eseentially the same as in Scheme
//!
EnvBlock::EnvBlock():
  content(Binding("t", number_to_Object(1))), next(NULL) {}

//!
//! Eseentially the same as in Scheme
//!
EnvBlock::EnvBlock(Binding cont, EnvBlock* nex):
  content(cont), next(nex) {}

//!
//! Eseentially the same as in Scheme
//!
Binding* EnvBlock::get_content() {
  return &content;
}

//!
//! Eseentially the same as in Scheme
//!
EnvBlock* EnvBlock::get_next() {
  return next;
}

//!
//! Eseentially the same as in Scheme
//!
void EnvBlock::set_next(EnvBlock* nex) {
  next = nex;
}

//!
//! Eseentially the same as in Scheme
//!
Environment::Environment() {
  head  = NULL;
}

//!
//! Actual copy of the environment, not simple redirect towards the same head.
//!
Environment::Environment(const Environment & source) {
  head = NULL;
  EnvBlock* writing = head; //< We are modifying the block writing points to
  EnvBlock* reading = source.head; //< We are reading the blocks of the source one at a time
  while (reading)
    {
      EnvBlock* copy = new EnvBlock(*reading->get_content(),NULL);

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

//!
//! Let's free those now useless cells
//!
Environment::~Environment() {
  while (head)
    {
      EnvBlock* killed = head;
      head = head->get_next();
      delete killed;
    }
}

//!
//! Eseentially the same as in Scheme
//!
void Environment::add_new_binding(string name, Object value) {
  EnvBlock* new_head = new EnvBlock(Binding(name,value), head);
  head = new_head;
}

//!
//! Eseentially the same as in Scheme
//!
void Environment::set_new_binding(string name, Object value) {
  try
    {
      Binding* target = find_block(name)->get_content();
      target->set_value(value);
    }
  catch (No_Binding_Exception) {
    add_new_binding(name,value);
  }
}

//!
//! Eseentially the same as in Scheme
//!
void Environment::extend_env(Object lpars, Object lvals) {
  if (null(lpars) && null(lvals)) return;
  if (null(lpars) && !null(lvals)) throw Zipping_Exception(lvals, "Too many values");
  if (!null(lpars) && null(lvals)) throw Zipping_Exception(lpars, "Too many parameters");
  add_new_binding(Object_to_string(car(lpars)), car(lvals));
  extend_env(cdr(lpars), cdr(lvals));
}

//!
//! Eseentially the same as in Scheme
//!
EnvBlock* Environment::find_block(string name) {
  EnvBlock* searching = head;

  while (searching)
    {
      if (searching->get_content()->get_name() == name)
        {
          return searching;
        }
      else
        {
          searching = searching->get_next();
        }
    }
  throw No_Binding_Exception(name);
}

//!
//! Eseentially the same as in Scheme
//!
Object Environment::find_value(string name) {
  return find_block(name)->get_content()->get_value();
}

//!
//! Eseentially the same as in Scheme
//!
void Environment::print(ostream& s) {
  EnvBlock* printing = head;
  while (printing)
    {
      string visible = "( ... )";
      Object value = printing->get_content()->get_value();
      if (value->is_pair())
        {
          value = string_to_Object(visible);
        }
      s << printing->get_content()->get_name() << ": " << value << "; ";
      printing = printing->get_next();
    }
}

//!
//! Creates an object containing a list of pairs storing bindings
//!
Object Environment::to_Object() {
  EnvBlock* reading = head;

  Object env_obj = new Cell();
  env_obj->make_cell_pair(NULL,NULL);
  Object writing = env_obj;

  while (reading) {
    writing->make_cell_pair(reading->get_content()->to_Object(),writing->to_pair_item()); //< Storing binding

    Object new_env_obj = new Cell();
    new_env_obj->make_cell_pair(NULL,writing); //< new block of the list

    writing = new_env_obj;
    reading = reading->get_next();
  }

  if (writing->to_pair_next())
    writing = writing->to_pair_next(); //< We are storing too much unfortunately

  return env_obj;
}

//!
//! Creates an object containing "closure", body and the Object form of the environment
//!
Object Environment::make_closure(Object body) {
  Object tag = new Cell();
  tag->make_cell_string("closure");

  Object tag_block = new Cell();

  Object tail_block = new Cell();
  tail_block->make_cell_pair(body,to_Object());

  tag_block->make_cell_pair(tag,tail_block);

  return tag_block;
}

//!
//! Recreates environment from suitable object
//!
Environment Object_to_env(Object e) {
  Environment new_env = Environment();

  Object reading = e;

  while (reading)
    {
      Binding writing = Object_to_binding(car(reading));
      new_env.add_new_binding(writing.get_name(),writing.get_value());
      reading = cdr(reading);
    }

  return new_env;
}

//!
//! Eseentially the same as in Scheme
//!
ostream& operator << (ostream& s, Environment& env) {
  env.print(s);
  return s;
}
