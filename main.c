#include "mazegen.h"

int main(int argc, char **argv)
{
    t_maze maze;
    maze.values = NULL;
    maze.size = (t_vector2){10, 10};
    maze.start_pos = (t_vector2){0, 0};
    maze.end_pos = (t_vector2){1, 1};
    maze.obstacle = 'x';
    maze.empty = ' ';
    maze.path_char = '-';
    maze.start_char = 'A';
    maze.end_char = 'B';

    if (argc == 3)
    {
        int x = atoi(argv[1]);
        int y = atoi(argv[2]);
        if (x == 0 || y == 0)
        {
            printf("Input must be 2 numbers");
            return 0;
        }
        maze.size = (t_vector2){x, y};
    }

    if (generate_maze(&maze))
    {
        // on fail
        printf("Maze gen failed!\n");
        return 0;
    }
    printf("maze:\n");
    print_maze(&maze);
    printf("\nsolved:\n");
    if (solve_maze(&maze))
    {
        // on fail
        printf("Maze solving failed! Maze was:\n");
        print_maze(&maze);
        free_maze(&maze);
        return 0;
    }
    print_maze(&maze);
    free_maze(&maze);
    return 0;
}