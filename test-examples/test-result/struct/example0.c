#include "../MarkerDefinition.h"
#include <assert.h>
#include <stddef.h> // for offsetof
#include <stdio.h>
#include <stdlib.h>

// ========== Basic struct definition ==========
struct Point {
  int x;
  int y;
};

// Typedef for convenience
typedef struct {
  double real;
  double imag;
} Complex;

// ========== Nested struct ==========
struct Rectangle {
  struct Point top_left;
  struct Point bottom_right;
};

// ========== Struct with bit fields ==========
struct Flags {
  unsigned int ready : 1; // 1 bit
  unsigned int error : 1;
  unsigned int mode : 2; // 2 bits
};

// ========== Union inside a struct ==========
union Value {
  int i;
  float f;
  char *s;
};

struct TaggedValue {
  enum { TYPE_INT, TYPE_FLOAT, TYPE_STRING } type;
  union Value value;
};

// ========== Self‑referential struct (linked list) ==========
typedef struct Node {
  int data;
  struct Node *next;
} Node;

// ========== Struct with array member ==========
struct Vector {
  int coords[3];
};

// ========== Struct with flexible array member (C99) ==========
struct Flexible {
  int length;
  double data[]; // flexible array – must be last member
};

// ========== Function that takes and returns structs ==========

// Pass by value (copy)
struct Point add_point(struct Point a, struct Point b) {
  codemarker_alpha(0); 
  struct Point result = {a.x + b.x, a.y + b.y};
  return result; // returns by value (copy)
}

// Pass by pointer (more efficient, modifies original)
void scale_point(struct Point *p, int factor) {
  p->x *= factor;
  p->y *= factor;
}

// ========== Using structs in different scopes ==========
int demo_scopes(void) {
  // Outer scope: declare and initialize a struct
  codemarker_alpha(1); 
  struct Point p1 = {10, 20};

  // Block scope (nested)
  {
    // Shadow outer p1? No, inner variable hides outer if same name.
    // Here we declare a new struct with same name, demonstrating shadowing.
    codemarker_alpha(2); 
    struct Point p1 = {5, 5}; // shadows outer p1
    p1.x += 3;                // modifies inner p1
    // Use p1 here; outer p1 still exists but inaccessible
  }
  // Back to outer scope – p1 is unchanged
  return p1.x + p1.y; // returns 30
}

// ========== Alignment and packing (implementation‑defined) ==========
#pragma pack(push, 1) // force 1‑byte alignment
struct Packed {
  char c;
  int i;
};
#pragma pack(pop)

// ========== Example with designated initializers ==========
void designated_initializer_example(void) {
  codemarker_alpha(3); 
  struct Point p = {.y = 5, .x = 2}; // order doesn't matter
  codemarker_alpha(4); 
  struct Rectangle r = {.top_left = {0, 0}, .bottom_right = {100, 100}};
  // Use them...
}

// ========== Main function demonstrating all features ==========
int main(void) {
  // 1. Basic struct usage
  codemarker_alpha(5); 
  struct Point a = {1, 2};
  codemarker_alpha(6); 
  struct Point b = {3, 4};
  codemarker_alpha(7); 
  struct Point sum = add_point(a, b);
  codemarker_alpha(8); 
  printf("Sum: (%d, %d)\n", sum.x, sum.y);

  codemarker_alpha(9); 
  scale_point(&a, 10);
  codemarker_alpha(10); 
  printf("Scaled a: (%d, %d)\n", a.x, a.y);

  // 2. Nested struct
  codemarker_alpha(11); 
  struct Rectangle rect = {{0, 0}, {10, 20}};
  codemarker_alpha(12); 
  printf("Rectangle: (%d,%d)-(%d,%d)\n", rect.top_left.x, rect.top_left.y,
         rect.bottom_right.x, rect.bottom_right.y);

  // 3. Bit fields
  codemarker_alpha(13); 
  struct Flags f = {1, 0, 2}; // ready=1, error=0, mode=2
  codemarker_alpha(14); 
  printf("Flags: ready=%u error=%u mode=%u\n", f.ready, f.error, f.mode);
  // Note: taking address of bit‑field members is not allowed.

  // 4. Union in struct
  codemarker_alpha(15); 
  struct TaggedValue tv;
  tv.type = TaggedValue::TYPE_INT;
  tv.value.i = 42;
  // later, depending on type, use correct union member

  // 5. Self‑referential (linked list)
  codemarker_alpha(16); 
  Node n1 = {1, NULL};
  codemarker_alpha(17); 
  Node n2 = {2, NULL};
  n1.next = &n2;
  codemarker_alpha(18); 
  printf("List: %d -> %d\n", n1.data, n1.next->data);

  // 6. Struct with array member
  codemarker_alpha(19); 
  struct Vector vec = {{1, 2, 3}};
  codemarker_alpha(20); 
  for (int i = 0; i < 3; i++) {
    codemarker_alpha(21); 
    printf("vec.coords[%d] = %d\n", i, vec.coords[i]);
  }

  // 7. Flexible array member – allocate with extra space
  codemarker_alpha(22); 
  struct Flexible *fp =
      (struct Flexible *)malloc(sizeof(struct Flexible) + 10 * sizeof(double));
  codemarker_alpha(23); 
  if (fp) {
    fp->length = 10;
    codemarker_alpha(24); 
    for (int i = 0; i < fp->length; i++) {
      fp->data[i] = i * 1.5;
    }
    // use fp...
    codemarker_alpha(25); 
    free(fp);
  }

  // 8. Demonstrate scopes
  codemarker_alpha(26); 
  int scoped_sum = demo_scopes();
  codemarker_alpha(27); 
  printf("demo_scopes returned %d\n", scoped_sum);

  // 9. Alignment and packing
  codemarker_alpha(28); 
  printf("Normal struct size: %zu\n", sizeof(struct Flags)); // likely 4
  codemarker_alpha(29); 
  printf("Packed struct size: %zu\n",
         sizeof(struct Packed)); // 5 (if packing works)
  printf("Offset of i in packed struct: %zu\n", offsetof(struct Packed, i));

  // 10. Designated initializers
  codemarker_alpha(30); 
  designated_initializer_example();

  // 11. Typedef’d struct
  codemarker_alpha(31); 
  Complex c1 = {1.0, 2.0};
  codemarker_alpha(32); 
  Complex c2 = {3.0, 4.0};
  // Complex arithmetic not shown, but demonstrates usage

  return 0;
}
