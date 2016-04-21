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

Object Binding::to_Object() {
  Object bound_name = new Cell();
  bound_name->make_cell_string(name);

  Object bound_object = new Cell();
  bound_object->make_cell_pair(bound_name, value);

  return bound_object;
}

Binding Object_to_binding(Object b) {
  return Binding(Object_to_string(car(b)), cdr(b));
}

/* Frame functions */

EnvBlock::EnvBlock():
  content(Binding("t", number_to_Object(1))), next(NULL) {}

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

Frame::Frame() {
  head  = NULL;
  scope = NULL;
}

Frame::Frame(const Frame & source) { // Needs to be updated
  head = NULL;
  scope = NULL;
  if (source.scope)
    scope = new Frame(*source.scope);
  EnvBlock* writing = head;
  EnvBlock* reading = source.head;
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

Frame::~Frame() {
  while (head)
    {
      EnvBlock* killed = head;
      head = head->get_next();
      delete killed;
    }
  delete scope;
}

void Frame::add_new_binding(string name, Object value) {
  EnvBlock* new_head = new EnvBlock(Binding(name,value), head);
  head = new_head;
}

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

void Frame::extend_env(Object lpars, Object lvals) {
  if (null(lpars) && null(lvals)) return;
  if (null(lpars) && !null(lvals)) throw Zipping_Exception(lvals, "Too many values");
  if (!null(lpars) && null(lvals)) throw Zipping_Exception(lpars, "Too many parameters");
  add_new_binding(Object_to_string(car(lpars)), car(lvals));
  extend_env(cdr(lpars), cdr(lvals));
}

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
    scope->find_value(name);
  throw No_Binding_Exception(name);
}

void Frame::print(ostream& s) {
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
  if (scope)
    scope->print(s);
}

// Object Frame::to_Object() {
//   EnvBlock* reading = head;

//   Object env_obj = new Cell();
//   env_obj->make_cell_pair(NULL,NULL);
//   Object writing = env_obj;

//   while (reading) {
//     writing->make_cell_pair(reading->get_content()->to_Object(),writing->to_pair_item());

//     Object new_env_obj = new Cell();
//     new_env_obj->make_cell_pair(NULL,writing);

//     writing = new_env_obj;
//     reading = reading->get_next();
//   }

//   if (writing->to_pair_next())
//     writing = writing->to_pair_next();

//   return env_obj;
// }

// Object Frame::make_closure(Object body) {
//   Object tag = new Cell();
//   tag->make_cell_string("closure");

//   Object tag_block = new Cell();

//   Object tail_block = new Cell();
//   tail_block->make_cell_pair(body,to_Object());

//   tag_block->make_cell_pair(tag,tail_block);

//   return tag_block;
// }

Frame Object_to_env(Object e) {
  Frame new_env = Frame();

  Object reading = e;

  while (reading)
    {
      Binding writing = Object_to_binding(car(reading));
      new_env.add_new_binding(writing.get_name(),writing.get_value());
      reading = cdr(reading);
    }

  return new_env;
}

ostream& operator << (ostream& s, Frame& env) {
  env.print(s);
  return s;
}
