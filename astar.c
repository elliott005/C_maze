#include "astar.h"

t_dir *astar(char *map, t_vector2 map_size, char obstacle, char empty, t_vector2 start, t_vector2 end)
{
    t_linked_point *first_open_point = malloc(sizeof(t_linked_point) * 1);
    if (!first_open_point)
    {
        printf("first_open_point malloc fail!\n");
        return NULL;
    }
    first_open_point->next = NULL;
    first_open_point->value.heuristic = distance(start, end);
    first_open_point->value.cost = 0;
    first_open_point->value.previous = NULL;
    first_open_point->value.position = start;

    t_vector2 dirs[4] = {
        {0, -1}, // up
        {0, 1},  // down
        {-1, 0}, // left
        {1, 0}   // right
    };

    t_linked_point *first_closed_point = malloc(sizeof(t_linked_point) * 1);
    if (!first_closed_point)
    {
        free(first_open_point);
        printf("first_closed_point malloc fail!\n");
        return NULL;
    }
    first_closed_point->value.cost = -1; // when cost is -1 it means the linked point is null

    while (first_open_point)
    {
        t_linked_point *current = get_lowest_f(first_open_point);
        if (!distance(current->value.position, end))
        {
            int path_len = 0;
            for (t_linked_point *i = current; i->value.previous; path_len++, i = i->value.previous);
            t_dir *path = malloc(sizeof(t_dir) * (path_len + 1));
            if (path)
            {
                path[path_len] = DIR_END;
                int idx = path_len - 1;
                for (t_linked_point *i = current; i->value.previous; i = i->value.previous, idx--)
                {
                    path[idx] = dir_to(i->value.previous->value.position, i->value.position);
                }
            }
            free_linked_list(first_open_point);
            free_linked_list(first_closed_point);
            return path;
        }
        for (int i = 0; i < 4; i++)
        {
            t_vector2 neighbor_pos = add_vectors(current->value.position, dirs[i]);
            if (!is_in_bounds(neighbor_pos, map_size) || map[coord_to_idx(neighbor_pos, map_size.x)] != empty)
            {
                continue;
            }
            int explored = 0;
            for (t_linked_point *i = first_closed_point; i != NULL; i = i->next)
            {
                if (i->value.cost == -1)
                {
                    break;
                }
                if (!distance(i->value.position, neighbor_pos))
                {
                    explored = 1;
                    break;
                }
            }
            for (t_linked_point *i = first_open_point; i != NULL && !explored; i = i->next)
            {
                if (i->value.cost == -1)
                {
                    break;
                }
                if (!distance(i->value.position, neighbor_pos))
                {
                    explored = 1;
                    break;
                }
            }
            if (explored)
            {
                continue;
            }
            t_linked_point *neighbor = malloc(sizeof(t_linked_point) * 1);
            if (!neighbor)
            {
                printf("neighbor malloc fail!\n");
                free_linked_list(first_open_point);
                free_linked_list(first_closed_point);
                return NULL;
            }
            neighbor->value.heuristic = distance(neighbor_pos, end);
            neighbor->value.cost = current->value.cost + 1;
            neighbor->value.previous = current;
            neighbor->value.position = neighbor_pos;
            neighbor->next = NULL;
            first_open_point = add_point(first_open_point, neighbor);
        }
        first_open_point = remove_point(first_open_point, current);
        first_closed_point = add_point(first_closed_point, current);
    }
    printf("no path found!\n");
    free_linked_list(first_open_point);
    free_linked_list(first_closed_point);
    return NULL;
}

t_linked_point *get_lowest_f(t_linked_point *first_open_point)
{
    t_linked_point *lowest_value = first_open_point;
    t_linked_point *current = first_open_point;
    while (current)
    {
        if (lowest_value->value.heuristic + lowest_value->value.cost > current->value.heuristic + current->value.cost)
        {
            lowest_value = current;
        }
        /* else if (lowest_value->value.heuristic + lowest_value->value.cost == current->value.heuristic + current->value.cost)
        {
            if (lowest_value->value.heuristic > current->value.heuristic)
            {
                lowest_value = current;
            }
        } */
        current = current->next;
    }
    return lowest_value;
}

t_linked_point *remove_point(t_linked_point *first_point, t_linked_point *to_remove)
{
    if (first_point == to_remove)
    {
        t_linked_point *next = first_point->next;
        to_remove->next = NULL;
        return next;
    }
    t_linked_point *last = first_point;
    while (last->next != to_remove)
    {
        last = last->next;
    }
    last->next = last->next->next;
    to_remove->next = NULL;
    return first_point;
}

t_linked_point *add_point(t_linked_point *first_point, t_linked_point *to_add)
{
    if (first_point->value.cost == -1) // first element is NULL
    {
        free(first_point);
        return to_add;
    }
    t_linked_point *last = first_point;
    while (last->next)
    {
        last = last->next;
    }
    last->next = to_add;
    return first_point;
}

t_dir dir_to(t_vector2 from, t_vector2 to)
{
    if (from.y > to.y)
        return UP;
    if (from.y < to.y)
        return DOWN;
    if (from.x > to.x)
        return LEFT;
    return RIGHT;
}

void free_linked_list(t_linked_point* first_point)
{
    while (first_point)
    {
        t_linked_point *previous = first_point;
        first_point = first_point->next;
        free(previous);
    }
}

int distance(t_vector2 a, t_vector2 b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

t_vector2 add_vectors(t_vector2 a, t_vector2 b)
{
    return (t_vector2){a.x + b.x, a.y + b.y};
}

int coord_to_idx(t_vector2 coords, int size_x)
{
    return coords.x + coords.y * size_x;
}

t_vector2 idx_to_coord(int idx, int size_x)
{
    t_vector2 vec;
    vec.x = idx % size_x;
    vec.y = idx / size_x;
    return vec;
}

int is_in_bounds(t_vector2 coords, t_vector2 map_size)
{
    if (coords.x < 0 || coords.x > map_size.x - 1)
        return 0;
    if (coords.y < 0 || coords.y > map_size.y - 1)
        return 0;
    return 1;
}