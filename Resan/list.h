/** \file list.h

  \brief A generic linked list implementation.

*/

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "util.h"


typedef struct _list_t list_t;

typedef struct _iter_t iter_t;

typedef struct _distance_label_t distance_label_t;

struct _distance_label_t
{
  void *label;
  list_t *path;
  list_t *path_edges;
  char *arrival_time;
  int dist;
};

/**
  Create an empty list.
*/
list_t *list_new();

/**
  Add an item to a list (at the end);
*/
void list_add(list_t *, void *);

/**
  Calculate the length of the list.

  \returns The list's length.
*/
int list_len(list_t *);

/**
  Access the nth member of a list. Zero-based.

  `*res` will be assigned to the list element at this location if the
  list is long enough, to `NULL` otherwise.

  \returns `true` if the list is long enough, `false` otherwise.
*/
bool list_nth(list_t *, int n, void **res);

/**
  Test whether a certain element exists.

  \param comp The \ref comparator_t to search with.

  \param elt The element to search for.

  \returns `true` if there is at least one element in the list that,
  compared using the \ref comparator_t, compares equal to `elt`.
*/
bool list_has(list_t *, comparator_t comp, void *elt);

/**
  Iterate over a list.

  Execute the function once for each element in the list and in order.
*/
void list_foreach(list_t *, void_fun_t);

/**
  Create an iterator to a list. Needs to be freed using \ref
  iter_free.

  \sa http://en.wikipedia.org/wiki/Iterator
*/
iter_t *iter(list_t *);

/**
  Advance the iterator by one step.

  \warning Must only be called if \ref iter_done returns `false`.

  \sa iter_done
*/
void iter_next(iter_t *);

/**
  Test whether the iterator has additional elements.

  \returns `true` if the iterator has additional elements, `false`
  otherwise.
*/
bool iter_done(iter_t *);

/**
  Access the current element.

  \returns The current element.
*/
void *iter_get(iter_t *);

/**
  Free the memory allocated by the iterator.
*/
void iter_free(iter_t *);

/**
  Make a shallow clone of a list.

  This operation is *shallow*, it will *not* clone the elements, just
  the list's spine.

  \returns A shallow clone of the list.
*/
list_t *list_clone(list_t *);


/**
   Adds a timetable for each station on the route of a certain busline.

   \param line The busline 

   \param time The starttime for the busline.


 */
void list_add_timetable(void *, list_t *, char *start, int line, char *time);


/**
   Test function
 */
void print_timetable(list_t *);

/**
Eget
 */
//void *list_last_element (list_t *);

/**
   Finds the next departure time for the supplied bus at the supplied station.

   \param from_node_el The station where we are at

   \param line The busline

   \param start_time The time we arrive at the station

   \returns next departure for the given busline.

 */
char *list_next_dep_time(list_t *, char *from_node_el,char *to_node_el, int line, char *start_time);

/**
   Finds the quickest line beetween two stations.

   
 */
int list_quickest_line(list_t *,char *,char *, char *);

/**
Eget
 */
//bool list_is_edge_visited(list_t *, comparator_t, void *);

/**
A function to free memory allocated in the struct distancelabels. Frees all the distancelabels in the list_t except the supplied distancelabel.

\param comp \ref comparator_t to search with.

\param dl supplied distancelabel

 */
void free_distancelabels(comparator_t comp, list_t *, distance_label_t *dl);

/**
   Frees the memory of distance label.
 */
void free_dl(distance_label_t *);

/**
  Frees all memory that the list allocated.
*/
void list_free();

#endif // LIST_H
