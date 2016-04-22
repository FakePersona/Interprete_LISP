#pragma once

#include <iostream>
#include <string>

using namespace std;

class Frame;

class Cell {
private:
  enum cell_sort {NUMBER, STRING, SYMBOL, PAIR, FRAME};
  cell_sort sort;

  struct cell_pair {
    Cell *item;
    Cell *next;
  };

  union cell_value {
    int as_number;
    char *as_string;
    char *as_symbol;
    cell_pair as_pair;
    Frame* as_frame;
  };

  cell_value value;
  static Cell cell_nil;
  void check();

  cell_sort get_sort() const;

public:
  Cell();

  bool is_number() const;
  bool is_string() const;
  bool is_symbol() const;
  bool is_pair() const;
  bool is_frame() const;

  int to_number() const;
  string to_string() const;
  string to_symbol() const;
  Cell *to_pair_item() const;
  Cell *to_pair_next() const;
  Frame* to_frame() const;

  static Cell *nil();

  void make_cell_number(int a);
  void make_cell_string(string s);
  void make_cell_symbol(string s);
  void make_cell_pair(Cell* p, Cell* q);
  void make_cell_frame(Frame* f);
};

ostream& operator << (ostream& s, const Cell *p);

