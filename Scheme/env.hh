#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "object.hh"

using namespace std;

class Binding {
private:
  string name;
  Object value;
public:
  Binding(string _name, Object _value);
  string get_name() const;
  Object get_value() const;
  void set_value(Object _value);
  Object to_Object();
};

Binding Object_to_binding(Object b);

class EnvBlock {
private:
  Binding content;
  EnvBlock* next;
public:
  EnvBlock();
  EnvBlock(Binding cont, EnvBlock* nex);
  Binding* get_content();
  EnvBlock* get_next();
  void set_next(EnvBlock* nex);
};

class Frame {
private:
  EnvBlock* head;
  Frame* scope;
public:
  Frame();
  Frame(Frame* _scope);
  Frame(const Frame & source);
  ~Frame();
  void add_new_binding(string name, Object value);
  void set_new_binding(string name, Object value);
  void extend_env(Object lpars, Object lvals);
  EnvBlock* find_block(string name);
  Object find_value(string name);
  void print(ostream& s);
  Object to_Object();
  Object make_closure(Object body);
};

Frame Object_to_env(Object e);

class Environment {
private:
  Frame* observing;

public:
  Environment();
  Environment(Frame* obs);
  Frame* get_observing();
  void set_new_binding(string name, Object value);
  void extend_env(Object lpars, Object lvals);
  Object find_value(string name);
  void print(ostream& s);
};

ostream& operator << (ostream& s, Environment& env);
