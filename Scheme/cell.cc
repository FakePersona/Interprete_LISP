/*! \file cell.cc */ 

#include <iostream>
#include <cassert>
#include <string>
#include <cstring> // For strdup
#include "cell.hh"

using namespace std;

void Cell::check() {}

/**********************/
/* Cell type checkers */
/**********************/

Cell::cell_sort Cell::get_sort() const {
  return sort;
}

bool Cell::is_number() const {
  return sort == NUMBER;
}
bool Cell::is_string() const {
  return sort == STRING;
}
bool Cell::is_symbol() const {
  return sort == SYMBOL;
}
bool Cell::is_pair() const {
  return sort == PAIR;
}

bool Cell::is_frame() const {
  return sort == FRAME;
}


bool Cell::is_subr() const {
  return sort == SUBR;
}

/********************/
/* Value converters */
/********************/

int Cell::to_number() const {
  assert(is_number());
  return value.as_number;
}

string Cell::to_string() const {
  assert(is_string());
  return string(value.as_string);
}

string Cell::to_symbol() const {
  assert(is_symbol());
  return string(value.as_symbol);
}

Cell *Cell::to_pair_item() const {
  assert(is_pair());
  return value.as_pair.item;
}

Cell *Cell::to_pair_next() const {
  assert(is_pair());
  return value.as_pair.next;
}

Frame* Cell::to_frame() const {
  assert(is_frame());
  return value.as_frame;
}

Cell*(*Cell::to_subr())(Cell*) {
  return value.as_subr;
}

Cell *Cell::nil() {
  return &cell_nil;
}

Cell::Cell() {
  make_cell_number(42); //For instance. Why not? :-)
  check();
}

/**************************/
/* Cell type transformers */
/**************************/

void Cell::make_cell_number(int a) {
  sort = NUMBER;
  value.as_number = a;
}

void Cell::make_cell_string(string s) {
  sort = STRING;
  char *p = strdup(s.c_str());	// Watch it! Allocated by malloc
  value.as_string = p;
}

void Cell::make_cell_symbol(string s) {
  sort = SYMBOL;
  char *p = strdup(s.c_str());	// Watch it! Allocated by malloc
  value.as_symbol = p;
}

void Cell::make_cell_pair(Cell* p, Cell* q) {
  sort = PAIR;
  cell_pair c;
  c.item = p;
  c.next = q;
  value.as_pair = c;
}

void Cell::make_cell_frame(Frame* f) {
  sort = FRAME;
  value.as_frame = f;
}

void Cell::make_cell_subr(Cell*(*sub)(Cell* lvals)) {
  sort = SUBR;
  value.as_subr = sub;
}

Cell Cell::cell_nil = Cell();

/******************/
/* Print function */
/******************/

static ostream& print_cell_pointer(ostream& s, const Cell *p);

//!
//! Used to process printing of lists
//!
static ostream& print_cell_pointer_aux(ostream& s, const Cell *p) {
  assert(p -> is_pair());
  for (const Cell *pp = p;; pp = pp -> to_pair_next()) {
    if (pp == Cell::nil()) break;
    if (pp->is_frame()) // Let's try not to get circulaar printing ...
      {
        s << "<env> " << flush;
        break;
      }
    print_cell_pointer(s, pp -> to_pair_item());
    if (pp -> to_pair_next() == Cell::nil()) break;
    s << " " << flush;
  }
  return s;
}

//!
//! Prints contents of an object
//!
static ostream& print_cell_pointer(ostream& s, const Cell *p) {
  if (p == Cell::nil()) return s << "nil" << flush;
  if (p -> is_number()) return s << p -> to_number() << flush;
  if (p -> is_string()) return s << p -> to_string() << flush;
  if (p -> is_symbol()) return s << p -> to_symbol() << flush;
  if (p -> is_subr()) return s;
  if (p -> is_pair()) {
    s << "(" << flush;
    print_cell_pointer_aux(s, p);
    s << ")" << flush;
    return s;
  }
  assert(false);
}

ostream& operator << (ostream& s, const Cell *p) {
  return print_cell_pointer(s, p);
}
