#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// --- Definições de Hardware ---
#define BUZZER_A_PIN 21
#define BUZZER_B_PIN 0
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// --- Configuração do Alerta ---
#define ALARM_FREQUENCY 3000
#define PWM_WRAP_VAL 4095

// Variável de estado
bool alarm_active = false; 

// --- Funções de Controle de PWM ---
void pwm_init_alarm(uint pin, uint frequency) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();
    float divider = (float)clock_get_hz(clk_sys) / (frequency * PWM_WRAP_VAL);
    pwm_config_set_clkdiv(&config, divider);

    pwm_set_wrap(slice_num, PWM_WRAP_VAL);
    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(pin, 0); 
}

void start_alarm_tone(uint pin) {
    pwm_set_gpio_level(pin, PWM_WRAP_VAL / 2);
}

void stop_alarm_tone(uint pin) {
    pwm_set_gpio_level(pin, 0);
}

int main() {
    stdio_init_all();
    sleep_ms(2000); 

    // --- Inicialização ---
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    pwm_init_alarm(BUZZER_A_PIN, ALARM_FREQUENCY);
    pwm_init_alarm(BUZZER_B_PIN, ALARM_FREQUENCY);

    char cmd[20]; // Aumentado o buffer para segurança

    while (true) {
        // 🔌 LEITURA DE COMANDOS SERIAL
        if (scanf("%19s", cmd) == 1) {
            
            // LOG DE DEBUG: Isso aparecerá no seu Node-RED
            printf("Recebido pela placa: [%s]\n", cmd); 

            if (strcmp(cmd, "ON") == 0) {
                start_alarm_tone(BUZZER_A_PIN);
                start_alarm_tone(BUZZER_B_PIN);
                alarm_active = true;
                printf("🔊 STATUS: ALARME LIGADO\n");
            } 
            else if (strcmp(cmd, "OFF") == 0) {
                stop_alarm_tone(BUZZER_A_PIN);
                stop_alarm_tone(BUZZER_B_PIN);
                alarm_active = false;
                printf("🔇 STATUS: ALARME DESLIGADO\n");
            } else {
                printf("⚠️ Comando desconhecido: %s\n", cmd);
            }
        }

        // 🔘 BOTÕES FÍSICOS
        if (alarm_active) {
            if (!gpio_get(BUTTON_A_PIN) || !gpio_get(BUTTON_B_PIN)) {
                stop_alarm_tone(BUZZER_A_PIN);
                stop_alarm_tone(BUZZER_B_PIN);
                alarm_active = false;
                printf("🛑 ALARME INTERROMPIDO PELO BOTAO\n");
            }
        }

        sleep_ms(50); 
    }
    return 0;
}