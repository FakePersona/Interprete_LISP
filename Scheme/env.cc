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
//! Creates a binding linking _name to _object
//!
Binding::Binding(string _name, Object _value):
  name(_name), value(_value) {}



string  Binding::get_name() const {
  return name;
}
Object  Binding::get_value() const {
  return value;
}

void Binding::set_value(Object _value) {
  value = _value;
}

/**********************/
/* EnvBlock functions */
/**********************/

//!
//! Creates an EnvBlock containing a token binding
//!
EnvBlock::EnvBlock():
  content(Binding("t", number_to_Object(1))), next(NULL) {}

//!
//! Creates an EnvBlock containing the binding of cont to nex
//!
EnvBlock::EnvBlock(Binding cont, EnvBlock* nex):
  content(cont), next(nex) {}

Binding* EnvBlock::get_content() {
  return &content;
}

EnvBlock* EnvBlock::get_next() {
  return next;
}

void EnvBlock::set_next(EnvBlock* nex) {
  next = nex;
}

/*******************/
/* Frame functions */
/*******************/

//!
//! Creates a frame with NULL for head and scope
//!
Frame::Frame() {
  head  = NULL;
  scope = NULL;
}

//!
//! Creates an empty frame with _scope as its scope
//!
Frame::Frame(Frame* _scope) {
  head = NULL;
  scope = _scope;
}

//!
//! makes a new EnvBlock containing the binding and pointing towards the current head of the frame the neww head
//!
void Frame::add_new_binding(string name, Object value) {
  EnvBlock* new_head = new EnvBlock(Binding(name,value), head);
  head = new_head;
}

//!
//! Looks at the current frame.
//! If there is no binding to the specified name in the current frame, add the bidning to the frame
//! Modifies it otherwise
//!
void Frame::set_new_binding(string name, Object value) {
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
//! Sets new bindings to the frame
//!
void Frame::extend_env(Object lpars, Object lvals) {
  if (null(lpars) && null(lvals)) return;
  if (null(lpars) && !null(lvals)) throw Zipping_Exception(lvals, "Too many values");
  if (!null(lpars) && null(lvals)) throw Zipping_Exception(lpars, "Too many parameters");
  set_new_binding(Object_to_string(car(lpars)), car(lvals));
  extend_env(cdr(lpars), cdr(lvals));
}

//!
//! Finds the EnvBlock in the current frame containing the binding pertaining to name
//!
EnvBlock* Frame::find_block(string name) {
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
//! Looks through the Envblocks of the frame and those of the following frames to find the binding of name
//!
Object Frame::find_value(string name) {
  EnvBlock* searching = head;

  while (searching)
    {
      if (searching->get_content()->get_name() == name)
        {
          return searching->get_content()->get_value();
        }
      else
        {
          searching = searching->get_next();
        }
    }
  if (scope)
    {
      return scope->find_value(name);
    }
  else
    throw No_Binding_Exception(name);
}

//!
//! Prints the contents of the Frame
//!
void Frame::print(ostream& s) {
  EnvBlock* printing = head;
  while (printing)
    {
      Object value = printing->get_content()->get_value();
      if (!value->is_subr())
        s << printing->get_content()->get_name() << ": " << value << "; ";
      printing = printing->get_next();
    }
  if (scope)
    {
      s << " >> ";
      scope->print(s);
    }
}

/*************************/
/* Environment functions */
/*************************/

//!
//! Creates a new Environment observing an empty frame (dynamically allocated)
//!
Environment::Environment() {
  observing = new Frame();
}

//!
//! Creates a new Environment observing the frame located at obs
//!
Environment::Environment(Frame* obs) {
  observing = obs;
}

//!
//! Returns the pointer of the Frame the environment is observing
//!
Frame* Environment::get_observing() {
  return observing;
}

//!
//! Dynamically binds in the observed frame
//!
void Environment::add_new_binding(string name, Object value) {
  observing->add_new_binding(name, value);
}

//!
//! Lexically binds in the observed frame
//!
void Environment::set_new_binding(string name, Object value) {
  observing->set_new_binding(name, value);
}

//!
//! Extends the observed frame
//!
void Environment::extend_env(Object lpars, Object lvals) {
  observing->extend_env(lpars, lvals);
}

//!
//! Looks for the value bound to name starting from the observed frame
//!
Object Environment::find_value(string name) {
  return observing->find_value(name);
}

//!
//! prints the content of the observed frame and its successors
//!
void Environment::print(ostream& s) {
  observing->print(s);
}

//!
//! Returns an object storing the adress of the observed frame
//!
Object Environment::to_Object() {
  Object frame_cell = new Cell();
  frame_cell->make_cell_frame(observing);

  return frame_cell;
}

//!
//! Creates a closure from the observed frame and the body
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
//! Creates a new environement observing the frame indicated in the closure
//!
Environment Object_to_env(Object e) {
  Environment new_env = Environment(e->to_pair_next()->to_pair_next()->to_frame());

  return new_env;
}

ostream& operator << (ostream& s, Environment& env) {
  env.print(s);
  return s;
}
