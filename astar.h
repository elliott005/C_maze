#ifndef ASTAR
#define ASTAR

#include <stdlib.h>
#include <stdio.h>

typedef struct s_vector2
{
    int x;
    int y;
} t_vector2;

typedef struct s_linked_point t_linked_point;

typedef struct s_point
{
    int heuristic;
    int cost;
    t_linked_point *previous;
    t_vector2 position;
} t_point;

typedef struct s_linked_point
{
    t_linked_point *next;
    t_point value;
} t_linked_point;

typedef enum e_dir
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DIR_END
} t_dir;

t_dir *astar(char *map, t_vector2 map_size, char obstacle, char empty, t_vector2 start, t_vector2 end);

t_linked_point *get_lowest_f(t_linked_point *first_open_point);
t_linked_point *remove_point(t_linked_point *first_point, t_linked_point *to_remove);
t_linked_point *add_point(t_linked_point *first_point, t_linked_point *to_add);
t_dir dir_to(t_vector2 from, t_vector2 to);
void free_linked_list(t_linked_point* first_point);
int distance(t_vector2 a, t_vector2 b);
t_vector2 add_vectors(t_vector2 a, t_vector2 b);
int coord_to_idx(t_vector2 coords, int size_x);
t_vector2 idx_to_coord(int idx, int size_x);
int is_in_bounds(t_vector2 coords, t_vector2 map_size);

#endif