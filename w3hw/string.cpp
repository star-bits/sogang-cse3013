#include "string.h"

#include <string.h>

#include <iostream>
using namespace std;

// constructor
Str::Str(int leng) {  // constructs a Str object with length leng.
  len = leng;
  str = new char[len + 1];  // `leng+1` size of char memory blocks are
                            // allocated with `new` command at runtime
}
Str::Str(char* neyong) {    // constructs a Str object with content in neyong.
  len = strlen(neyong);     // len
  str = new char[len + 1];  // get the len of neyong and memory allocation
  strcpy(str, neyong);      // copy neyong into str
}

// destructor
Str::~Str() { delete[] str; }

int Str::length(void) {
  return len;
}  // function returns the length of the string.

char* Str::contents(void) {
  return str;
}  // function returns a pointer to the string content.

// function compares the string content with another Str object.
int Str::compare(class Str& a) { return strcmp(str, a.contents()); }
// function compares the string with another string.
int Str::compare(char* a) { return strcmp(str, a); }

// overload
void Str::operator=(char* a) {  // function assigns a string to this Str object.
  delete[] str;                 // deallocates pre-existing str
  len = strlen(a);
  str = new char[len + 1];
  strcpy(str, a);
}
void Str::operator=(
    class Str& a) {  // function assigns a Str object to this Str object.
  delete[] str;
  len = a.length();
  str = new char[len + 1];
  strcpy(str, a.contents());
}
