#include "./include/buzzer.h"

// Inicializa o buzzer no pino especificado
void buzzer_init(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
    gpio_put(gpio, 0);
}

// Inicializa todos os buzzers
void buzzer_init_all(){
    buzzer_init(BUZZER_A_PIN);
    buzzer_init(BUZZER_B_PIN);
}

// Configura o PWM no pino do buzzer com uma frequência especificada
void set_buzzer_frequency(uint pin, uint frequency) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (frequency * 4096)); // Calcula divisor do clock

    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Inicializa com duty cycle 0 (sem som)
}

// Função para tocar o buzzer por um tempo especificado (em milissegundos)
void play_buzzer(uint pin, uint frequency, uint duration_ms) {

    set_buzzer_frequency(pin, frequency);   
    pwm_set_gpio_level(pin, 32768);           
    sleep_ms(duration_ms);                   
    pwm_set_gpio_level(pin, 0);              
}

// Função para tocar um som de negação
void play_denied_sound(){
    red();
    play_buzzer(BUZZER_A_PIN, MI, 100);
    black();
    sleep_ms(50);
    red();
    play_buzzer(BUZZER_A_PIN, MI, 100);
    black();
}

// Função para tocar um som de sucesso
void play_success_sound(){
    green();
    play_buzzer(BUZZER_A_PIN, SOL, 100);
    black();
    sleep_ms(50);
    green();
    play_buzzer(BUZZER_A_PIN, SOL, 100);
    black();
}

// Função para tocar uma música de inicialização
void play_delivery_sound() {
    yellow();
    play_buzzer(BUZZER_A_PIN, DO, 150);
    black();
    sleep_ms(50);
    yellow();
    play_buzzer(BUZZER_A_PIN, RE, 150);
    black();
    sleep_ms(50);
    yellow();
    play_buzzer(BUZZER_A_PIN, MI, 150);
    black();
    sleep_ms(50);
    yellow();
    play_buzzer(BUZZER_A_PIN, SOL, 200);
    black();
}