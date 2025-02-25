#include "./include/factory.h"
#include <stdlib.h>

SectorConnection connections[] = {
    {0, 1, {2, 4}, {2, 0}}, {0, 3, {4, 2}, {0, 2}},
    {1, 2, {2, 4}, {2, 0}}, {1, 4, {4, 2}, {0, 2}},
    {2, 5, {4, 2}, {0, 2}}, {3, 4, {2, 4}, {2, 0}},
    {3, 6, {4, 2}, {0, 2}}, {4, 5, {2, 4}, {2, 0}},
    {4, 7, {4, 2}, {0, 2}}, {5, 8, {4, 2}, {0, 2}},
    {6, 7, {2, 4}, {2, 0}}, {7, 8, {2, 4}, {2, 0}},
};
int num_connections = sizeof(connections) / sizeof(connections[0]);

const int dx[4] = {0, 0, -1, 1};
const int dy[4] = {-1, 1, 0, 0};

bool is_valid_position(int x, int y) {
    return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

bool bfs(Factory *factory, Robot start, Robot goal, Robot **came_from) {
    bool *visited = (bool *)calloc(NUM_SECTORS * COLS * ROWS, sizeof(bool));
    if (!visited) {
        printf("Erro: Falha ao alocar memória para visited\n");
        return false;
    }

    *came_from = (Robot *)malloc(NUM_SECTORS * COLS * ROWS * sizeof(Robot));
    if (!(*came_from)) {
        printf("Erro: Falha ao alocar memória para came_from\n");
        free(visited);
        return false;
    }

    Robot *queue = (Robot *)malloc(NUM_SECTORS * COLS * ROWS * sizeof(Robot));
    if (!queue) {
        printf("Erro: Falha ao alocar memória para queue\n");
        free(visited);
        free(*came_from);
        return false;
    }

    int front = 0, back = 0;
    queue[back++] = start;
    visited[start.sector * COLS * ROWS + start.position.y * COLS + start.position.x] = true;
    (*came_from)[start.sector * COLS * ROWS + start.position.y * COLS + start.position.x] = start;

    while (front < back) {
        Robot current = queue[front++];

        if (current.sector == goal.sector && 
            current.position.x == goal.position.x && 
            current.position.y == goal.position.y) {
            free(queue);
            free(visited);
            return true;
        }

        // Movimentos dentro do setor
        for (int i = 0; i < 4; i++) {
            int nx = current.position.x + dx[i];
            int ny = current.position.y + dy[i];

            int index = current.sector * COLS * ROWS + nx * COLS + ny;
            
            if (is_valid_position(nx, ny) &&
                factory->sectors[current.sector][nx * COLS + ny] != 2 &&
                !visited[index]) {

                queue[back++] = (Robot){current.sector, {nx, ny}};
                visited[index] = true;
                (*came_from)[index] = current;
            }
        }

        // Movimentos entre setores
        for (int i = 0; i < num_connections; i++) {
            if (connections[i].from_sector == current.sector &&
                connections[i].from_pos.x == current.position.x &&
                connections[i].from_pos.y == current.position.y) {
                
                int next_sector = connections[i].to_sector;
                Position next_pos = connections[i].to_pos;
                int index = next_sector * COLS * ROWS + next_pos.x * COLS + next_pos.y;

                if (!visited[index] &&
                    factory->sectors[next_sector][next_pos.x * COLS + next_pos.y] != 2) {

                    queue[back++] = (Robot){next_sector, next_pos};
                    visited[index] = true;
                    (*came_from)[index] = current;
                }
            }
        }
    }

    free(queue);
    free(visited);
    free(*came_from);
    return false;
}
/*
void reconstruct_path(Robot *came_from, Robot start, Robot goal) {
    Robot path[NUM_SECTORS * COLS * ROWS];
    int length = 0;

    Robot at = goal;
    while (!(at.sector == start.sector && at.position.x == start.position.x && at.position.y == start.position.y)) {
        path[length++] = at;
        at = came_from[at.sector * COLS * ROWS + at.position.x * COLS + at.position.y];
    }
    path[length++] = start;

    for (int i = length - 1; i >= 0; i--) {
        printf("Setor: %d, Posicao: (%d, %d)\n", path[i].sector, path[i].position.x, path[i].position.y);
    }
    free(came_from);
}
*/
void reconstruct_path(Robot *came_from, Robot start, Robot goal, Robot **path, int *path_length) {
    *path_length = 0;

    // Primeiro, determine o tamanho do caminho para alocar memória
    Robot at = goal;
    while (!(at.sector == start.sector && at.position.x == start.position.x && at.position.y == start.position.y)) {
        (*path_length)++;
        at = came_from[at.sector * COLS * ROWS + at.position.x * COLS + at.position.y];
    }
    (*path_length)++; // Contabiliza o ponto inicial

    // Aloca memória para o caminho
    *path = (Robot *)malloc((*path_length) * sizeof(Robot));
    if (*path == NULL) {
        printf("Erro: Falha ao alocar memória para o caminho.\n");
        return;
    }

    // Preenche o caminho na ordem correta
    at = goal;
    for (int i = (*path_length) - 1; i >= 0; i--) {
        (*path)[i] = at;
        at = came_from[at.sector * COLS * ROWS + at.position.x * COLS + at.position.y];
    }

    // Exibe o caminho encontrado
    for (int i = 0; i < *path_length; i++) {
        printf("Setor: %d, Posicao: (%d, %d)\n", (*path)[i].sector, (*path)[i].position.x, (*path)[i].position.y);
    }

    free(came_from);
}

