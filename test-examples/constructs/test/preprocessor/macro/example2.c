#include "../../MarkerDefinition.h"
#define EMPTY
#define EMPTY_FUNC(x)

#define INNER(x) (x + 1)
#define OUTER(y) INNER(y)
int z = OUTER(5);
int f = OUTER(INNER(5));
int x = OUTER(INNER(OUTER(INNER(5))));

int main() {
  int a = 42;
  EMPTY;           // expands to nothing
  EMPTY_FUNC(123); // expands to nothing

#define LONGIFY(decl) long decl
  LONGIFY(int b = 5); // expands to: long int a = 5;

  return a;
}
