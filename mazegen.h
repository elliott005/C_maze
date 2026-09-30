#ifndef MAZE_GEN
#define MAZE_GEN

#include <time.h>

#include "astar.h"

typedef struct s_maze
{
    char *values;
    t_vector2 size;
    t_vector2 start_pos;
    t_vector2 end_pos;
    char obstacle;
    char empty;
    char path_char;
    char start_char;
    char end_char;
} t_maze;

int generate_maze(t_maze *empty_maze);
int solve_maze(t_maze *maze);
int is_maze_solvable(t_maze *maze);
void print_maze(t_maze *maze);
void free_maze(t_maze *maze);

#endif