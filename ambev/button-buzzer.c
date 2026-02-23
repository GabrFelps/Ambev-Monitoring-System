#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

// ================= CONFIGURAÇÕES DE REDE =================
#define WIFI_SSID "trojan"
#define WIFI_PASS "14052025"
// IP do seu PC (Verifique no 'ipconfig' se mudou ao usar o Hotspot)
#define MQTT_SERVER_IP "10.23.206.27" 

// Pinos da BitDogLab
#define BUZZER_A_PIN 21
#define BUZZER_B_PIN 0
#define ALARM_FREQUENCY 3000
#define PWM_WRAP_VAL 4095

mqtt_client_t *mqtt_client;

// ================= CONTROLE DO ÁUDIO (PWM) =================
void pwm_init_alarm(uint pin, uint frequency) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();

    // Cálculo da frequência: $divider = \frac{f_{sys}}{f_{target} \times PWM\_WRAP\_VAL}$
    float divider = (float)clock_get_hz(clk_sys) / (frequency * PWM_WRAP_VAL);

    pwm_config_set_clkdiv(&config, divider);
    pwm_set_wrap(slice, PWM_WRAP_VAL);
    pwm_init(slice, &config, true);
    pwm_set_gpio_level(pin, 0); // Começa desligado
}

void set_alarm(bool on) {
    uint32_t level = on ? (PWM_WRAP_VAL / 2) : 0;
    pwm_set_gpio_level(BUZZER_A_PIN, level);
    pwm_set_gpio_level(BUZZER_B_PIN, level);
    printf("🔊 [AMBEV] Alarme: %s\n", on ? "LIGADO" : "DESLIGADO");
}

// ================= CALLBACKS DO PROTOCOLO MQTT =================
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    // Apenas log de monitoramento
}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    char msg[50];
    if (len >= sizeof(msg)) len = sizeof(msg) - 1;
    memcpy(msg, data, len);
    msg[len] = '\0';

    printf("📩 Mensagem Node-RED: %s\n", msg);

    if (strcmp(msg, "ON") == 0) set_alarm(true);
    else if (strcmp(msg, "OFF") == 0) set_alarm(false);
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("✅ Conectado ao Broker MQTT da Ambev!\n");
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);
        mqtt_subscribe(client, "ambev/alarme", 0, NULL, NULL);
        printf("📡 Inscrito no topico: ambev/alarme\n");
    } else {
        printf("❌ Falha na conexao MQTT. Status: %d\n", status);
    }
}

// ================= INICIALIZAÇÃO E LOOP PRINCIPAL =================
void connect_mqtt() {
    ip_addr_t broker_ip;
    ipaddr_aton(MQTT_SERVER_IP, &broker_ip);
    mqtt_client = mqtt_client_new();

    struct mqtt_connect_client_info_t ci = {
        .client_id = "PicoW_Alisson_TCC",
        .keep_alive = 60
    };

    mqtt_client_connect(mqtt_client, &broker_ip, 1883, mqtt_connection_cb, NULL, &ci);
}

int main() {
    stdio_init_all();
    sleep_ms(3000); // Tempo para abrir o Serial Monitor

    printf("🚀 Iniciando BitDogLab em Teresina...\n");

    // Configuração para o Brasil garante melhores canais de Wi-Fi
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL)) {
        printf("❌ Erro de Hardware Wi-Fi\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    printf("🛰️ Buscando rede: %s...\n", WIFI_SSID);

    // Timeout de 45s e Mixed PSK para resolver erros -1 e -2
    int wifi = cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_MIXED_PSK, 45000);

    if (wifi != 0) {
        printf("❌ Falha Wi-Fi. Erro: %d (Verifique se o celular esta em 2.4GHz)\n", wifi);
        return 1;
    }

    printf("✅ Wi-Fi Conectado!\n");
    printf("📶 IP Local da Placa: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));

    // Prepara o Alarme
    pwm_init_alarm(BUZZER_A_PIN, ALARM_FREQUENCY);
    pwm_init_alarm(BUZZER_B_PIN, ALARM_FREQUENCY);

    printf("🔗 Conectando ao Broker no IP: %s\n", MQTT_SERVER_IP);
    connect_mqtt();

    while (true) {
        // Necessário para manter a pilha de rede e MQTT rodando
        cyw43_arch_poll(); 
        sleep_ms(10);
    }

    return 0;
}