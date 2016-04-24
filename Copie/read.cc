#include "object.hh"
#include <stdio.h>

extern Object just_read;
extern "C" int yyparse();
extern "C" FILE *yyin;

Object read() {
	yyparse();
	return just_read;
}

int end_input(){
	return (feof(yyin));
}
