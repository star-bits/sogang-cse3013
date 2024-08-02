#include <string.h>

#include <iostream>

#include "string.h"
using namespace std;

int main(void) {
  Str a("I'm a girl");   // initialzes a Str object `a` with "I'm a girl"
  cout << a.contents();  // prints `a`
  a = "I'm a boy\n";     // assigns "I'm a boy\n" to `a`
  cout << a.contents();  // prints `a`
  cout << a.compare("I'm a a") << endl;  // compares `a` with "I'm a a"
  return 0;
}

// g++ main.cpp string.cpp