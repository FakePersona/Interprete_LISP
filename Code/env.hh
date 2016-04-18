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
};

class EnvBlock {
private:
  Binding content;
  EnvBlock* next;
public:
  EnvBlock();
  EnvBlock(Binding cont, EnvBlock* nex);
  Binding get_content();
  EnvBlock* get_next();
  void set_next(EnvBlock* nex);
};

class Environment {
private:
  EnvBlock* head;
public:
  Environment();
  Environment(const Environment & source);
  ~Environment();
  void add_end(Binding data);
  void add_new_binding(string name, Object value);
  void set_new_binding(string name, Object value);
  void extend_env(Object lpars, Object lvals);
  Binding find_binding(string name);
  Object find_value(string name);
  void print(ostream& s);
};

ostream& operator << (ostream& s, Environment& env);
