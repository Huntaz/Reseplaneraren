#include "network.h"
#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "util.h"

struct _network_t
{
    graph_t *g;
};

typedef struct _edge_t edge_t;
struct _edge_t
{
    int duration;
    line_no line;
};

#define BUFSIZE 1000

bool graph_check_exist(network_t *n, char* from, char* to)
{
  if(graph_check_node_exist(n->g, from, to))return true;

  else return false;
}

void trim_leading_space(char *dest, const char *src)
{
    assert(dest);
    assert(src);
    while (isspace(*src))
        {
            ++src;
        }
    strcpy(dest, src);
}

void printEdge(void *from, void *to, void *label)
{
    edge_t *e = label;
    printf("#%2i:%30s --(%2imin)--> %s\n", e->line, (char *)from, e->duration,
           (char *)to);
    // assert(false);
}

bool check_time(char *time)
{
  int hour,min;
  if(sscanf(time, "%d:%d", &hour, &min))return true;
  return false;
}

void timetable_parse(network_t *netw,FILE *file) // Egen funktion
{
    assert(file);

    char buffer[BUFSIZE];
    while (fgets(buffer, BUFSIZE, file))
        {
	  char bus_start[BUFSIZE];

	  int bus_line;
	  char bus_time[BUFSIZE];
	  int matches = sscanf(buffer, "%i, %32[^,], %32[^,]",&bus_line, bus_start, bus_time);
	  if(!(matches == 3))continue;
	  if(!check_time(bus_time))continue;


	  int bus_line_dup = bus_line;
	  char *bus_start_dup = strdup(bus_start);
	  char *bus_time_dup = strdup(bus_time);
	 

	  graph_add_timetable(netw->g, bus_start_dup, bus_line_dup, bus_time_dup);
	}
    fclose(file);
    //graph_print_timetable(netw->g);
}




network_t *network_parse(FILE *file)
{
    assert(file);
    network_t *netw = calloc(1, sizeof(struct _network_t));
    netw->g = graph_new(streq);

    char buffer[BUFSIZE];
    while (fgets(buffer, BUFSIZE, file))
        {
	  char bus_from[BUFSIZE];
	  char bus_to[BUFSIZE];
	  edge_t *e = malloc(sizeof(struct _edge_t));

	  int matches = sscanf(buffer, "%i, %32[^,], %32[^,], %i", &(e->line), bus_from, bus_to, &(e->duration));

	  if(!(matches == 4))continue;
	  char *bus_from_dup = strdup(bus_from);
	  char *bus_to_dup = strdup(bus_to);
	  graph_add_node(netw->g, bus_from_dup);
	  graph_add_node(netw->g, bus_to_dup);
	  graph_add_edge(netw->g, bus_from_dup, bus_to_dup, e);
	}

    fclose(file);
    timetable_parse(netw,fopen("start.txt","r"));

    return netw;
}





void network_print(network_t *n)
{
    assert(n);
    graph_foreach_edge(n->g, printEdge);
}

distance_label_t *network_find_travels(network_t *n, char *time, char *from, char *to)
{
    assert(n && from && to);
    if (streq(from, to))
        {
	  distance_label_t *dl = calloc(1, sizeof(distance_label_t));
            return dl;
        }
    distance_label_t *path = graph_find_path(n->g, time, from, to);
    return path;
}

bool network_comp_line (void *label, int line) //EGEN!!!
{
  edge_t *edge = label;
  if (edge->line == line)
    {
      return true;
    }
  return false;
}

int network_get_line (void *label)
{
  edge_t *edge = label;
  return edge->line;
}

int network_get_dur(void *label)
{
  edge_t *temp_dur = label;
  return temp_dur->duration;
}

void free_edge_label(void *from, void *to, void *label)
{
    free(label);
}

void network_free(network_t *n)
{
    graph_foreach_edge(n->g, free_edge_label);
    graph_foreach_node(n->g, free);
    graph_free(n->g);
    free(n);
}
