#include "mazegen.h"

void print_maze(t_maze *maze)
{
    for (int i = 0; maze->values[i]; i++)
    {
        if (i % maze->size.x == 0)
        {
            printf("\n");
        }
        printf("%c ", maze->values[i]);
    }
    printf("\n");
}

int generate_maze(t_maze *empty_maze)
{
    srand(time(NULL));
    
    int num_values = empty_maze->size.x * empty_maze->size.y;
    empty_maze->values = malloc(sizeof(char) + (num_values + 1));
    if (!empty_maze->values)
    {
        return 1;
    }
    // initialize empty map
    empty_maze->values[num_values] = '\0';
    for (int i = 0; i < num_values; i++)
    {
        empty_maze->values[i] = empty_maze->obstacle;
    }

    int *stack = malloc(sizeof(int) * (num_values + 1));
    if (!stack)
    {
        free(empty_maze->values);
        return 1;
    }
    stack[0] = -1; // -1 means end
    int stack_size = 0;
    int *visited = malloc(sizeof(int) * num_values);
    if (!visited)
    {
        free(empty_maze->values);
        free(stack);
        return 1;
    }
    for (int i = 0; i < num_values; i++)
    {
        visited[i] = 0;
    }
    // Choose the initial cell, mark it as visited and push it to the stack
    int start_idx = coord_to_idx(empty_maze->start_pos, empty_maze->size.x);
    visited[start_idx] = 1;
    stack[stack_size] = start_idx;
    stack[stack_size + 1] = -1;
    stack_size++;

    t_vector2 dirs[4] = {
        {0, -2}, // up
        {0, 2},  // down
        {-2, 0}, // left
        {2, 0}   // right
    };
    t_vector2 dirs_single[4] = {
        {0, -1}, // up
        {0, 1},  // down
        {-1, 0}, // left
        {1, 0}   // right
    };

    t_vector2 end_pos;
    int farthest_end_pos = -1;

    while (stack_size)
    {
        int current_cell = stack[stack_size - 1];
        t_vector2 current_cell_coords = idx_to_coord(current_cell, empty_maze->size.x);
        stack[stack_size - 1] = -1;
        stack_size--;
        
        int unvisited_neighbors = 0;
        for (int i = 0; i < 4; i++)
        {
            t_vector2 neighbor_coords = add_vectors(current_cell_coords, dirs[i]);
            if (!is_in_bounds(neighbor_coords, empty_maze->size))
            {
                continue;
            }
            int neighbor = coord_to_idx(neighbor_coords, empty_maze->size.x);
            if (!visited[neighbor])
            {
                unvisited_neighbors++;
            }
        }
        if (unvisited_neighbors > 0)
        {
            stack[stack_size] = current_cell;
            stack[stack_size + 1] = -1;
            stack_size++;

            int chosen_neighbor = rand() % unvisited_neighbors;
            int current_neighbor = 0;
            for (int i = 0; i < 4; i++)
            {
                t_vector2 neighbor_coords = add_vectors(current_cell_coords, dirs[i]);
                if (!is_in_bounds(neighbor_coords, empty_maze->size))
                {
                    continue;
                }
                int neighbor = coord_to_idx(neighbor_coords, empty_maze->size.x);
                if (!visited[neighbor])
                {
                    if (current_neighbor == chosen_neighbor)
                    {
                        empty_maze->values[current_cell] = empty_maze->empty;
                        empty_maze->values[neighbor] = empty_maze->empty;
                        t_vector2 middle_coords = add_vectors(current_cell_coords, dirs_single[i]);
                        int middle_idx = coord_to_idx(middle_coords, empty_maze->size.x);
                        empty_maze->values[middle_idx] = empty_maze->empty;
                        visited[neighbor] = 1;
                        stack[stack_size] = neighbor;
                        stack[stack_size + 1] = -1;
                        stack_size++;
                        break;
                    }
                    current_neighbor++;
                }
            }
        }
        else
        {
            int dist_from_start = distance(empty_maze->start_pos, current_cell_coords);
            if (farthest_end_pos == -1 || dist_from_start > farthest_end_pos)
            {
                farthest_end_pos = dist_from_start;
                end_pos = current_cell_coords;
            }
        }
    }

    empty_maze->end_pos = end_pos;

    free(stack);
    free(visited);

    return 0;
}

int solve_maze(t_maze *maze)
{
    t_dir *path = astar(maze->values, maze->size, maze->obstacle, maze->empty, maze->start_pos, maze->end_pos);
    if (!path)
    {
        return 1;
    }
    t_vector2 position = maze->start_pos;
    maze->values[coord_to_idx(position, maze->size.x)] = maze->start_char;
    for (int i = 0; path[i] != DIR_END; i++)
    {
        switch (path[i])
        {
        case UP:
            position.y--;
            break;
        case DOWN:
            position.y++;
            break;
        case LEFT:
            position.x--;
            break;
        case RIGHT:
            position.x++;
            break;
        
        default:
            break;
        }
        maze->values[coord_to_idx(position, maze->size.x)] = maze->path_char;
    }
    maze->values[coord_to_idx(position, maze->size.x)] = maze->end_char;
    free(path);
    return 0;
}

int is_maze_solvable(t_maze *maze)
{
    t_dir *path = astar(maze->values, maze->size, maze->obstacle, maze->empty, maze->start_pos, maze->end_pos);
    if (path)
    {
        free(path);
        return 1;
    }
    else
    {
        free(path);
        return 0;
    }
}

void free_maze(t_maze *maze)
{
    free(maze->values);
}