#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"   
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "./include/button.h"
#include "./include/buzzer.h"
#include "./include/joystick.h"
#include "./include/led_rgb.h"
#include "./include/matrix.h"
#include "./include/ssd1306.h"
#include "./include/factory.h"

ssd1306_t ssd;
volatile uint32_t last_time_button_a = 0;
volatile uint32_t last_time_button_b = 0;
volatile uint32_t last_time_joystick_button = 0;
uint16_t joystick_x, joystick_y;
uint8_t sector = 0;
Factory factory = {
    .sectors[0] = {
        2, 2, 2, 2, 2,
        2, 1, 0, 0, 2,
        2, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[1] = {
        2, 2, 2, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[2] = {
        2, 2, 2, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 2,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[3] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        2, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[4] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[5] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 2,
        2, 0, 0, 0, 2,
        2, 2, 0, 2, 2
    },
    .sectors[6] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        2, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 2, 2, 2
    },
    .sectors[7] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 0,
        2, 0, 0, 0, 2,
        2, 2, 2, 2, 2
    },
    .sectors[8] = {
        2, 2, 0, 2, 2,
        2, 0, 0, 0, 2,
        0, 0, 0, 0, 2,
        2, 0, 0, 0, 2,
        2, 2, 2, 2, 2
    },
    .robot = {
        .position = {
            .x = 1,
            .y = 1
        },
        .sector = 0
    }
};

void draw_factory(){
    RGB pixels[NUM_PIXELS];
        for (int i = 0; i < NUM_PIXELS; i++) {
            if(factory.sectors[sector][i] == 0){
                pixels[i] = BLACK;
            } else if(factory.sectors[sector][i] == 1){
                pixels[i] = RED;
            } else if(factory.sectors[sector][i] == 2){
                pixels[i] = BLUE;
            } else if(factory.sectors[sector][i] == 3){
                pixels[i] = GREEN;
            }
        }
        desenho_pio(pixels, pio0, 0);
}

void move_up(){
    if(factory.sectors[sector][coordenates_to_index(factory.robot.position.x - 1, factory.robot.position.y)] == 2){
        play_denied_sound();
    }
    else if(factory.robot.position.x > 0){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.position.x--;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
    }
}

void move_down(){
    if(factory.sectors[sector][coordenates_to_index(factory.robot.position.x + 1, factory.robot.position.y)] == 2){
        play_denied_sound();
    }
    else if(factory.robot.position.x < 4){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.position.x++;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
    }
}

void move_left(){
    if(factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y - 1)] == 2){
        play_denied_sound();
    }
    else if(factory.robot.position.y > 0){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.position.y--;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
    }
}

void move_right(){
    if(factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y + 1)] == 2){
        play_denied_sound();
    }
    else if(factory.robot.position.y < 4){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.position.y++;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
    }
}

void change_to_right_sector(){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.sector = sector + 1;
        factory.robot.position.x = 2;
        factory.robot.position.y = 0;
        sector++;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
}

void change_to_left_sector(){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.sector = sector - 1;
        factory.robot.position.x = 2;
        factory.robot.position.y = 4;
        sector--;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
}

void change_to_up_sector(){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.sector = sector - 3;
        factory.robot.position.x = 4;
        factory.robot.position.y = 2;
        sector -= 3;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
}

void change_to_down_sector(){
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.sector = sector + 3;
        factory.robot.position.x = 0;
        factory.robot.position.y = 2;
        sector += 3;
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
}

void movimentation_in_sector_0(){
    if(joystick_x > 3000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 4){
            change_to_right_sector();
        }
        move_right();
    }
    else if(joystick_x < 1000){
        move_left();
    }
    else if(joystick_y > 3000){
        move_up();
    }
    else if(joystick_y < 1000){
        if(factory.robot.position.x == 4 && factory.robot.position.y == 2){
            change_to_down_sector();
        }
        else if(factory.robot.position.x < 4){
            move_down();
        }
    }
}

void movimentation_in_sector_1(){
    if(joystick_x > 3000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 4){
            change_to_right_sector();
        }
        move_right();
    }
    else if(joystick_x < 1000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 0){
            change_to_left_sector();
        }
        else if(factory.robot.position.y > 0){
            move_left();
        }
    }
    else if(joystick_y > 3000){
        move_up();
    }
    else if(joystick_y < 1000){
        if(factory.robot.position.x == 4 && factory.robot.position.y == 2){
            change_to_down_sector();
        }
        move_down();
    } 
}

void movimentation_in_sector_2(){
    if(joystick_x > 3000){
        move_right();
    }
    else if(joystick_x < 1000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 0){
            change_to_left_sector();
        }
        move_left();
    }
    else if(joystick_y > 3000){
        move_up();
    }
    else if(joystick_y < 1000){
        if(factory.robot.position.x == 4 && factory.robot.position.y == 2){
            change_to_down_sector();
        }
        move_down();
    }
}

void movimentation_in_sector_3(){
    if(joystick_x > 3000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 4){
            change_to_right_sector();
        }
        move_right();
    }
    else if(joystick_x < 1000){
        move_left();
    }
    else if(joystick_y > 3000){
        if(factory.robot.position.x == 0 && factory.robot.position.y == 2){
            change_to_up_sector();
        }
        move_up();
    }
    else if(joystick_y < 1000){
        if(factory.robot.position.x == 4 && factory.robot.position.y == 2){
            change_to_down_sector();
        }
        move_down();
    }
}

void movimentation_in_sector_4(){
    if(joystick_x > 3000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 4){
            change_to_right_sector();
        }
        move_right();
    }
    else if(joystick_x < 1000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 0){
            change_to_left_sector();
        }
        move_left();
    }
    else if(joystick_y > 3000){
        if(factory.robot.position.x == 0 && factory.robot.position.y == 2){
            change_to_up_sector();
        }
        move_up();
    }
    else if(joystick_y < 1000){
        if(factory.robot.position.x == 4 && factory.robot.position.y == 2){
            change_to_down_sector();
        }
        move_down();
    }
}

void movimentation_in_sector_5(){
    if(joystick_x > 3000){
        move_right();
    }
    else if(joystick_x < 1000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 0){
            change_to_left_sector();
        }
        move_left();
    }
    else if(joystick_y > 3000){
        if(factory.robot.position.x == 0 && factory.robot.position.y == 2){
            change_to_up_sector();
        }
        move_up();
    }
    else if(joystick_y < 1000){
        if(factory.robot.position.x == 4 && factory.robot.position.y == 2){
            change_to_down_sector();
        }
        move_down();
    }
}

void movimentation_in_sector_6(){
    if(joystick_x > 3000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 4){
            change_to_right_sector();
        }
        move_right();
    }
    else if(joystick_x < 1000){
        move_left();
    }
    else if(joystick_y > 3000){
        if(factory.robot.position.x == 0 && factory.robot.position.y == 2){
            change_to_up_sector();
        }
        move_up();
    }
    else if(joystick_y < 1000){
        move_down();
    }
}

void movimentation_in_sector_7(){
    if(joystick_x > 3000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 4){
            change_to_right_sector();
        }
        move_right();
    }
    else if(joystick_x < 1000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 0){
            change_to_left_sector();
        }
        move_left();
    }
    else if(joystick_y > 3000){
        if(factory.robot.position.x == 0 && factory.robot.position.y == 2){
            change_to_up_sector();
        }
        move_up();
    }
    else if(joystick_y < 1000){
        move_down();
    }
}

void movimentation_in_sector_8(){
    if(joystick_x > 3000){
        move_right();
    }
    else if(joystick_x < 1000){
        if(factory.robot.position.x == 2 && factory.robot.position.y == 0){
            change_to_left_sector();
        }
        move_left();
    }
    else if(joystick_y > 3000){
        if(factory.robot.position.x == 0 && factory.robot.position.y == 2){
            change_to_up_sector();
        }
        move_up();
    }
    else if(joystick_y < 1000){
        move_down();
    }
}

void manual_mode_movimentation(){
    reading_joystick(&joystick_x, &joystick_y);
    draw_factory();

    switch (factory.robot.sector){
        case 0:
            movimentation_in_sector_0();
            break;
        case 1:
            movimentation_in_sector_1();
            break;
        case 2:
            movimentation_in_sector_2();
            break;
        case 3:
            movimentation_in_sector_3();
            break;
        case 4:
            movimentation_in_sector_4();
            break;
        case 5:
            movimentation_in_sector_5();
            break;
        case 6:
            movimentation_in_sector_6();
            break;
        case 7:
            movimentation_in_sector_7();
            break;
        case 8:
            movimentation_in_sector_8();
            break;
    }
    sleep_ms(200);
}

void automatic_mode_movimentation(Robot* path, int path_length) {
    for (int i = 0; i < path_length; i++) {
        // Atualiza o setor apenas se mudar
        factory.sectors[sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 0;
        factory.robot.position.x = path[i].position.x;
        factory.robot.position.y = path[i].position.y;
        factory.robot.sector = path[i].sector;
        factory.sectors[factory.robot.sector][coordenates_to_index(factory.robot.position.x, factory.robot.position.y)] = 1;
        sector = factory.robot.sector;
        draw_factory();
        if(i < path_length - 1){
            sleep_ms(500);
        }
    }
    play_success_sound();
}

void find_path(Robot goal){
    Robot start = {factory.robot.sector, {factory.robot.position.x, factory.robot.position.y}};
    Robot *came_from = NULL;
    Robot *path = NULL;
    int path_length = 0;

    if (bfs(&factory, start, goal, &came_from)) {
        printf("Caminho encontrado:\n");
        reconstruct_path(came_from, start, goal, &path, &path_length);
        automatic_mode_movimentation(path, path_length);
    } else {
        printf("Nenhum caminho encontrado.\n");
    }
}

int main(){
    stdio_init_all();
    //button_init_all();
    buzzer_init_all();
    //led_init_all();
    matrix_init();
    joystick_init();
    matrix_init();
    display_init(&ssd);
    sleep_ms(10000);
    Robot goal = {8, {2, 2}};
    find_path(goal);
    while (true) {
        draw_factory();
        manual_mode_movimentation();    
        
    }
    return 0;
}
