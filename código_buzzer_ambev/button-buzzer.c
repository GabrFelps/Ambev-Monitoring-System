#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// --- Definições de Hardware ---
#define BUZZER_A_PIN 21             // GPIO conectado ao Buzzer A na BitDogLab [cite: 965, 1083]
#define BUZZER_B_PIN 0              // GPIO conectado ao Buzzer B na BitDogLab 
#define BUTTON_A_PIN 5              // GPIO conectado ao Botão A na BitDogLab
#define BUTTON_B_PIN 6              // GPIO conectado ao Botão B na BitDogLab

// --- Configuração do Alerta ---
#define ALARM_FREQUENCY 3000      // 3000 Hz para um tom de alerta claro
#define PWM_WRAP_VAL 4095         // Valor máximo do contador para resolução de 12 bits

// Variável de estado para controlar se o alarme está ativo
bool alarm_active = true;

// --- Funções de Controle de PWM ---

// Inicializa o PWM no pino do buzzer com a frequência alta (ALARM_FREQUENCY)
void pwm_init_alarm(uint pin, uint frequency) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    
    pwm_config config = pwm_get_default_config();
    
    // Calcula o divisor de clock para atingir a frequência desejada
    float divider = (float)clock_get_hz(clk_sys) / (frequency * PWM_WRAP_VAL);
    pwm_config_set_clkdiv(&config, divider); 
    
    pwm_init(slice_num, &config, true);
    
    // Configura o valor máximo do contador (WRAP/TOP)
    pwm_set_wrap(slice_num, PWM_WRAP_VAL);

    // Começa desligado
    pwm_set_gpio_level(pin, 0); 
}

// Liga o tom de alerta (50% de Duty Cycle)
void start_alarm_tone(uint pin) {
    uint level = PWM_WRAP_VAL / 2;
    pwm_set_gpio_level(pin, level);
}

// Desliga o tom de alerta
void stop_alarm_tone(uint pin) {
    pwm_set_gpio_level(pin, 0);
}


int main() {
    stdio_init_all();
    printf("Sistema de Alarme Duplo. Pressione o Botão A ou B para desligar.\n");

    // 1. Configuração dos Botões A (GPIO 5) e B (GPIO 6)
    // Botão A
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN); 
    // Botão B
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN); 

    // 2. Configuração dos Buzzers A (GPIO 21) e B (GPIO 0)
    pwm_init_alarm(BUZZER_A_PIN, ALARM_FREQUENCY);
    pwm_init_alarm(BUZZER_B_PIN, ALARM_FREQUENCY);

    // 3. Lógica do Alarme
    // Liga os dois buzzers no início
    start_alarm_tone(BUZZER_A_PIN); 
    start_alarm_tone(BUZZER_B_PIN); 

    while (true) {
        // Verifica se o alarme ainda está ativo
        if (alarm_active) {
            // Se o Botão A (GPIO 5) ou o Botão B (GPIO 6) for pressionado (nível lógico LOW)
            if (gpio_get(BUTTON_A_PIN) == false || gpio_get(BUTTON_B_PIN) == false) { 
                
                // Desliga ambos os buzzers
                stop_alarm_tone(BUZZER_A_PIN);
                stop_alarm_tone(BUZZER_B_PIN);
                
                alarm_active = false;
                printf("Alarme Desligado. Pressione BOOTSEL para reiniciar.\n");
            }
        }
        
        sleep_ms(50); // Delay para evitar leituras inconsistentes
    }

    return 0;
}