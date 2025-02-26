#include "./include/factory.h"
#include <stdlib.h>

SectorConnection connections[] = {
    {0, 1, {2, 4}, {2, 0}}, {1, 0, {2, 0}, {2, 4}}, // Conexão bidirecional entre 0 e 1
    {0, 3, {4, 2}, {0, 2}}, {3, 0, {0, 2}, {4, 2}}, // Conexão bidirecional entre 0 e 3
    {1, 2, {2, 4}, {2, 0}}, {2, 1, {2, 0}, {2, 4}}, // Conexão bidirecional entre 1 e 2
    {1, 4, {4, 2}, {0, 2}}, {4, 1, {0, 2}, {4, 2}}, // Conexão bidirecional entre 1 e 4
    {2, 5, {4, 2}, {0, 2}}, {5, 2, {0, 2}, {4, 2}}, // Conexão bidirecional entre 2 e 5
    {3, 4, {2, 4}, {2, 0}}, {4, 3, {2, 0}, {2, 4}}, // Conexão bidirecional entre 3 e 4
    {3, 6, {4, 2}, {0, 2}}, {6, 3, {0, 2}, {4, 2}}, // Conexão bidirecional entre 3 e 6
    {4, 5, {2, 4}, {2, 0}}, {5, 4, {2, 0}, {2, 4}}, // Conexão bidirecional entre 4 e 5
    {4, 7, {4, 2}, {0, 2}}, {7, 4, {0, 2}, {4, 2}}, // Conexão bidirecional entre 4 e 7
    {5, 8, {4, 2}, {0, 2}}, {8, 5, {0, 2}, {4, 2}}, // Conexão bidirecional entre 5 e 8
    {6, 7, {2, 4}, {2, 0}}, {7, 6, {2, 0}, {2, 4}}, // Conexão bidirecional entre 6 e 7
    {7, 8, {2, 4}, {2, 0}}, {8, 7, {2, 0}, {2, 4}}, // Conexão bidirecional entre 7 e 8
};
int num_connections = sizeof(connections) / sizeof(connections[0]);

const int dx[4] = {0, 0, -1, 1};
const int dy[4] = {-1, 1, 0, 0};

int atual_capacity = 0;

void draw_factory(Factory *factory, uint8_t *sector){
    RGB pixels[NUM_PIXELS];
        for (int i = 0; i < NUM_PIXELS; i++) {
            if(factory->sectors[*sector][i] == 0){
                pixels[i] = BLACK;
            } else if(factory->sectors[*sector][i] == 1){
                pixels[i] = RED;
            } else if(factory->sectors[*sector][i] == 2){
                pixels[i] = BLUE;
            } else if(factory->sectors[*sector][i] == 3){
                pixels[i] = GREEN;
            } else if(factory->sectors[*sector][i] == 4){
                pixels[i] = YELLOW;
            }
        }
        desenho_pio(pixels, pio0, 0);
}

bool verify_objective(Robot objectives[], Factory *factory, uint8_t *sector, bool delivered[]){
    for(int i = 0; i < NUM_LOADS; i++){
        if(objectives[i].sector == factory->robot.sector && objectives[i].position.x == factory->robot.position.x && objectives[i].position.y == factory->robot.position.y && !delivered[i] && (atual_capacity < LOAD_CAPACITY)){
            delivered[i] = true;
            return true;
        }
    }
    return false;
}

void move_up(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]){
    if(factory->sectors[*sector][coordenates_to_index(factory->robot.position.x - 1, factory->robot.position.y)] == 2){
        play_denied_sound();
    }
    else if(factory->robot.position.x > 0){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.position.x--;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
        draw_factory(factory, sector);
        if(verify_objective(objectives, factory, sector, delivered)){
            play_success_sound();
            atual_capacity++;
        }
    }
}

void move_down(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]){
    if(factory->sectors[*sector][coordenates_to_index(factory->robot.position.x + 1, factory->robot.position.y)] == 2){
        play_denied_sound();
    }
    else if(factory->robot.position.x < 4){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.position.x++;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
        draw_factory(factory, sector);
        if(verify_objective(objectives, factory, sector, delivered)){
            play_success_sound();
            atual_capacity++;
        } else if(factory->robot.position.x == 4 && factory->robot.position.y == 2 && *sector == 7 && atual_capacity > 0){
            play_delivery_sound();
            show_destination(factory);
            atual_capacity = 0;
        }
    }
}

void move_left(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]){
    if(factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y - 1)] == 2){
        draw_factory(factory, sector);
        play_denied_sound();
    }
    else if(factory->robot.position.y > 0){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.position.y--;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
        draw_factory(factory, sector);
        if(verify_objective(objectives, factory, sector, delivered)){
            play_success_sound();
            atual_capacity++;
        }
    }
}

void move_right(Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[]){
    if(factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y + 1)] == 2){
        play_denied_sound();
    }
    else if(factory->robot.position.y < 4){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.position.y++;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
        draw_factory(factory, sector);
        if(verify_objective(objectives, factory, sector, delivered)){
            play_success_sound();
            atual_capacity++;
        }
    }
}

void change_to_right_sector(Factory *factory, uint8_t *sector){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.sector = *sector + 1;
        factory->robot.position.x = 2;
        factory->robot.position.y = 0;
        (*sector)++;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
}

void change_to_left_sector(Factory *factory, uint8_t *sector){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.sector = *sector - 1;
        factory->robot.position.x = 2;
        factory->robot.position.y = 4;
        (*sector)--;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
}

void change_to_up_sector(Factory *factory, uint8_t *sector){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.sector = *sector - 3;
        factory->robot.position.x = 4;
        factory->robot.position.y = 2;
        (*sector) -= 3;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
}

void change_to_down_sector(Factory *factory, uint8_t *sector){
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.sector = *sector + 3;
        factory->robot.position.x = 0;
        factory->robot.position.y = 2;
        (*sector) += 3;
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
}

void movimentation_in_sector_0(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 4){
            change_to_right_sector(factory, sector);
        }
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        if(factory->robot.position.x == 4 && factory->robot.position.y == 2){
            change_to_down_sector(factory, sector);
        }
        else if(factory->robot.position.x < 4){
            move_down(factory, sector, delivered, objectives);
        }
    }
}

void movimentation_in_sector_1(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 4){
            change_to_right_sector(factory, sector);
        }
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 0){
            change_to_left_sector(factory, sector);
        }
        else if(factory->robot.position.y > 0){
            move_left(factory, sector, delivered, objectives);
        }
    }
    else if(joystick_y > 3000){
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        if(factory->robot.position.x == 4 && factory->robot.position.y == 2){
            change_to_down_sector(factory, sector);
        }
        move_down(factory, sector, delivered, objectives);
    } 
}

void movimentation_in_sector_2(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 0){
            change_to_left_sector(factory, sector);
        }
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        if(factory->robot.position.x == 4 && factory->robot.position.y == 2){
            change_to_down_sector(factory, sector);
        }
        move_down(factory, sector, delivered, objectives);
    }
}

void movimentation_in_sector_3(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 4){
            change_to_right_sector(factory, sector);
        }
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        if(factory->robot.position.x == 0 && factory->robot.position.y == 2){
            change_to_up_sector(factory, sector);
        }
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        if(factory->robot.position.x == 4 && factory->robot.position.y == 2){
            change_to_down_sector(factory, sector);
        }
        move_down(factory, sector, delivered, objectives);
    }
}

void movimentation_in_sector_4(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 4){
            change_to_right_sector(factory, sector);
        }
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 0){
            change_to_left_sector(factory, sector);
        }
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        if(factory->robot.position.x == 0 && factory->robot.position.y == 2){
            change_to_up_sector(factory, sector);
        }
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        if(factory->robot.position.x == 4 && factory->robot.position.y == 2){
            change_to_down_sector(factory, sector);
        }
        move_down(factory, sector, delivered, objectives);
    }
}

void movimentation_in_sector_5(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 0){
            change_to_left_sector(factory, sector);
        }
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        if(factory->robot.position.x == 0 && factory->robot.position.y == 2){
            change_to_up_sector(factory, sector);
        }
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        if(factory->robot.position.x == 4 && factory->robot.position.y == 2){
            change_to_down_sector(factory, sector);
        }
        move_down(factory, sector, delivered, objectives);
    }
}

void movimentation_in_sector_6(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 4){
            change_to_right_sector(factory, sector);
        }
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        if(factory->robot.position.x == 0 && factory->robot.position.y == 2){
            change_to_up_sector(factory, sector);
        }
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        move_down(factory, sector, delivered, objectives);
    }
}

void movimentation_in_sector_7(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 4){
            change_to_right_sector(factory, sector);
        }
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 0){
            change_to_left_sector(factory, sector);
        }
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        if(factory->robot.position.x == 0 && factory->robot.position.y == 2){
            change_to_up_sector(factory, sector);
        }
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        move_down(factory, sector, delivered, objectives);
    }
}

void movimentation_in_sector_8(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, bool delivered[], Robot objectives[]){
    if(joystick_x > 3000){
        move_right(factory, sector, delivered, objectives);
    }
    else if(joystick_x < 1000){
        if(factory->robot.position.x == 2 && factory->robot.position.y == 0){
            change_to_left_sector(factory, sector);
        }
        move_left(factory, sector, delivered, objectives);
    }
    else if(joystick_y > 3000){
        if(factory->robot.position.x == 0 && factory->robot.position.y == 2){
            change_to_up_sector(factory, sector);
        }
        move_up(factory, sector, delivered, objectives);
    }
    else if(joystick_y < 1000){
        move_down(factory, sector, delivered, objectives);
    }
}

int get_missing_deliverables(Factory *factory){
    int missing_deliverables = 0;
    for(int i = 0; i < NUM_SECTORS; i++){
        for(int j = 0; j < NUM_PIXELS; j++){
            if(factory->sectors[i][j] == 3){
                missing_deliverables++;
            }
        }
    }
    return missing_deliverables;
}

void manual_mode_movimentation(Factory *factory, uint8_t *sector, uint16_t joystick_x, uint16_t joystick_y, ssd1306_t *ssd, bool delivered[], Robot objectives[]){
    reading_joystick(&joystick_x, &joystick_y);
    draw_factory(factory, sector);
    navigation_status(ssd, *sector, atual_capacity, get_missing_deliverables(factory));
    show_destination(factory);
    show_delivarables(factory, *sector, delivered, objectives);
    switch (factory->robot.sector){
        case 0:
            movimentation_in_sector_0(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 1:
            movimentation_in_sector_1(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 2:
            movimentation_in_sector_2(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 3:
            movimentation_in_sector_3(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 4:
            movimentation_in_sector_4(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 5:
            movimentation_in_sector_5(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 6:
            movimentation_in_sector_6(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 7:
            movimentation_in_sector_7(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
        case 8:
            movimentation_in_sector_8(factory, sector, joystick_x, joystick_y, delivered, objectives);
            break;
    }
    sleep_ms(200);
}

void automatic_mode_movimentation(Robot* path, int path_length, Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[], ssd1306_t *ssd){ 
    for (int i = 0; i < path_length; i++) {   
        factory->sectors[*sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 0;
        factory->robot.position.x = path[i].position.x;
        factory->robot.position.y = path[i].position.y;
        factory->robot.sector = path[i].sector;
        factory->sectors[factory->robot.sector][coordenates_to_index(factory->robot.position.x, factory->robot.position.y)] = 1;
        *sector = factory->robot.sector;
        show_delivarables(factory, *sector, delivered, objectives);
        show_destination(factory);
        draw_factory(factory, sector);
        navigation_status(ssd, *sector, atual_capacity, get_missing_deliverables(factory));
        if(i < path_length - 1){
            sleep_ms(500);
        }
    }
}

void find_path(Robot goal, Factory *factory, uint8_t *sector, bool delivered[], Robot objectives[], ssd1306_t *ssd) {
    Robot start = {factory->robot.sector, {factory->robot.position.x, factory->robot.position.y}};
    Robot *came_from = NULL;
    Robot *path = NULL;
    int path_length = 0;

    if (bfs(factory, start, goal, &came_from)) {
        reconstruct_path(came_from, start, goal, &path, &path_length);
        automatic_mode_movimentation(path, path_length, factory, sector, delivered, objectives, ssd);
    }else{
        printf("Caminho não encontrado\n");
    }
}

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

void reconstruct_path(Robot *came_from, Robot start, Robot goal, Robot **path, int *path_length) {
    *path_length = 0;

    Robot at = goal;
    while (!(at.sector == start.sector && at.position.x == start.position.x && at.position.y == start.position.y)) {
        (*path_length)++;
        at = came_from[at.sector * COLS * ROWS + at.position.x * COLS + at.position.y];
    }
    (*path_length)++; 

    *path = (Robot *)malloc((*path_length) * sizeof(Robot));
    if (*path == NULL) {
        printf("Erro: Falha ao alocar memória para o caminho.\n");
        return;
    }

    at = goal;
    for (int i = (*path_length) - 1; i >= 0; i--) {
        (*path)[i] = at;
        at = came_from[at.sector * COLS * ROWS + at.position.x * COLS + at.position.y];
    }

    free(came_from);
}

// Mapeia setores para coordenadas globais na fábrica 3x3 setores
void get_global_coordinates(Robot coordinate, int *globalX, int *globalY) {
    int sectorX = coordinate.sector % 3;  // Coluna do setor (0 a 2)
    int sectorY = coordinate.sector / 3;  // Linha do setor (0 a 2)

    *globalX = sectorX * 5 + coordinate.position.x;
    *globalY = sectorY * 5 + coordinate.position.y;
}

// Calcula a distância Manhattan entre dois pontos (considerando setores)
int manhattan_distance(Robot a, Robot b) {
    int globalAx, globalAy, globalBx, globalBy;
    
    // Converter para coordenadas globais
    get_global_coordinates(a, &globalAx, &globalAy);
    get_global_coordinates(b, &globalBx, &globalBy);

    int distance = abs(globalAx - globalBx) + abs(globalAy - globalBy);

    if (a.sector != b.sector) {
        distance += 5;
    }

    return distance;
}

void randomize_objectives(Robot objectives[], Factory *factory) {
    srand(to_ms_since_boot(get_absolute_time()));
    // Posições permitidas para alocação dos objetivos
    int valid_positions[] = {12};
    int num_valid_positions = sizeof(valid_positions) / sizeof(valid_positions[0]);

    for (int i = 0; i < NUM_LOADS; i++) {
        int sector, index, x, y;
        do {
            sector = rand() % NUM_SECTORS; // Sorteia um setor
            index = valid_positions[rand() % num_valid_positions]; // Sorteia uma posição válida dentro do setor
            
            // Converte o índice 1D para coordenadas (x, y)
            x = index / COLS;
            y = index % COLS;
            
        } while (factory->sectors[sector][index] != 0); // Garante que a posição está livre
        
        // Atribuir valores ao objetivo
        objectives[i].sector = sector;
        objectives[i].position.x = x;
        objectives[i].position.y = y;
        factory->sectors[sector][index] = 3; // Marca o objetivo no setor
    }
}

void show_destination(Factory *factory) {
    if(factory->sectors[7][22]!=1)
            factory->sectors[7][22]=4;
}

void calculate_distances(int distances[], Robot objectives[], Factory *factory) {
    for (int i = 0; i < NUM_LOADS; i++) {
        distances[i] = manhattan_distance(factory->robot, objectives[i]);
    }
}

void insertion_sort(int arr[], int n, bool aux[], Robot objectives[]) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        bool key_aux = aux[i];
        Robot key_obj = objectives[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            aux[j + 1] = aux[j];
            objectives[j + 1] = objectives[j];
            j--;
        }

        arr[j + 1] = key;
        aux[j + 1] = key_aux;
        objectives[j + 1] = key_obj;
    }
}

    //Calcula a distância de cada objetivo para o robô
    //Ordena os objetivos de acordo com a distância
    //O robô se move para o objetivo mais próximo
    //O robô tem a capacidade de carregar 3 objetivos
    //Quando o robô se posiciona em um objetivo, ele é marcado como entregue em delivered[]
    //Quando o robô chega na capacidade máxima, ele vai para a posição 22 do setor 7
    //Chegando lá, o robô zera a capacidade e volta a procurar os objetivos mais próximos
    //O robô repete o processo até entregar todos os objetivos

bool verify_delivered(bool delivered[], int n) {
    for (int i = 0; i < n; i++) {
        if (!delivered[i]) {
            return false;
        }
    }
    return true;
}

void show_delivarables(Factory *factory, uint8_t sector, bool delivered[], Robot objectives[]) {
    for (int i = 0; i < NUM_LOADS; i++) {
        if (!delivered[i] && factory->sectors[objectives[i].sector][objectives[i].position.x * COLS + objectives[i].position.y] != 1) {
            factory->sectors[objectives[i].sector][objectives[i].position.x * COLS + objectives[i].position.y] = 3;
        }
    }
}
    
void solve_capacitated_vrp(Factory *factory, Robot objectives[], int distances[], bool delivered[], uint8_t *sector, ssd1306_t *ssd) {
    int carried_loads = 0;
    Robot destination = {7, {4, 2}}; 
    while (1) {
        for (int i = 0; i < NUM_LOADS; i++) {
            calculate_distances(distances, objectives, factory);
            insertion_sort(distances, NUM_LOADS, delivered, objectives);

            if (!delivered[i]) {
                find_path(objectives[i], factory, sector, delivered, objectives, ssd);
                play_success_sound();
                delivered[i] = true;
                carried_loads++;
                atual_capacity = carried_loads;
            }
            if (carried_loads == LOAD_CAPACITY) {
                find_path(destination, factory, sector, delivered, objectives, ssd);
                play_delivery_sound();
                carried_loads = 0;
                atual_capacity = carried_loads;
            }
        }
        if(verify_delivered(delivered, NUM_LOADS)){
            find_path(destination, factory, sector, delivered, objectives, ssd);
            play_delivery_sound();
            break;
        }
    }
    atual_capacity = 0;
}

void reset_delivered(bool delivered[]) {
    for (int i = 0; i < NUM_LOADS; i++) {
        delivered[i] = false;
    }
    atual_capacity = 0;
}



    

