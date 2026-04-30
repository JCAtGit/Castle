#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Simple doubly linked list implementation.
 * Inspired by the Linux kernel list.h, but simplified.
 */

struct list_head {
  struct list_head *next, *prev;
};

/* Initialise a list head */
#define INIT_LIST_HEAD(name) {&(name), &(name)}

/* Insert a new entry between two known consecutive entries */
static inline void __list_add(struct list_head *new, struct list_head *prev,
                              struct list_head *next) {
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}

/* Add a new entry at the head of the list */
#define list_add(new, head) __list_add(new, (head), (head)->next)

/*
 * Container of – given a pointer to a member of a struct,
 * return the pointer to the containing struct.
 */
#define container_of(ptr, type, member)                                        \
  ((type *)((char *)(ptr) - offsetof(type, member)))

/*
 * Get the struct entry from a list head pointer.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/*
 * Iterate over a list. 'pos' is a pointer to the current 'type' struct,
 * 'head' is the list head, 'member' is the name of the list_head inside 'type'.
 */
#define list_for_each_entry(pos, head, member)                                 \
  for (pos = list_entry((head)->next, typeof(*pos), member);                   \
       &pos->member != (head);                                                 \
       pos = list_entry(pos->member.next, typeof(*pos), member))

/* ----------------------------------------------------------------------
 * Our own FREE macro – safely frees a pointer and sets it to NULL.
 * This matches typical project‑specific behaviour.
 */
#define FREE(ptr)                                                              \
  do {                                                                         \
    free(ptr);                                                                 \
    (ptr) = NULL;                                                              \
  } while (0)

/* ----------------------------------------------------------------------
 * Example structure: a "blob" with a string member that needs freeing.
 */
struct blob_descriptor {
  char *solid_sort_name;
  int some_data;
  struct list_head write_blobs_list; /* list linkage */
};

int main(void) {
  /* List head – initially empty */
  struct list_head blob_list = INIT_LIST_HEAD(blob_list);

  /* Create a few blobs */
  struct blob_descriptor blob1 = {.solid_sort_name = strdup("alpha"),
                                  .some_data = 1};
  struct blob_descriptor blob2 = {.solid_sort_name = strdup("beta"),
                                  .some_data = 2};
  struct blob_descriptor blob3 = {.solid_sort_name = strdup("gamma"),
                                  .some_data = 3};

  /* Add them to the list */
  list_add(&blob1.write_blobs_list, &blob_list);
  list_add(&blob2.write_blobs_list, &blob_list);
  list_add(&blob3.write_blobs_list, &blob_list);

  /* ---- This is the exact pattern from the question ---- */
  struct blob_descriptor *blob;
  list_for_each_entry(blob, &blob_list, write_blobs_list) {
    FREE(blob->solid_sort_name);
  }
  /* ------------------------------------------------------ */

  /* Verify that the pointers are now NULL */
  printf("After FREE:\n");
  printf("blob1.solid_sort_name = %s\n",
         blob1.solid_sort_name ? blob1.solid_sort_name : "(null)");
  printf("blob2.solid_sort_name = %s\n",
         blob2.solid_sort_name ? blob2.solid_sort_name : "(null)");
  printf("blob3.solid_sort_name = %s\n",
         blob3.solid_sort_name ? blob3.solid_sort_name : "(null)");

  /* Note: The list nodes themselves are not freed in this example,
   * but that would be a separate cleanup step using list_del() etc.
   */
  return 0;
}
