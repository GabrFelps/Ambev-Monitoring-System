#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"

/**************  MQTT (lwIP do Pico SDK)  ****************/
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/dns.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"

#define BUZZER_A 21
#define BUZZER_B 20

// ------------ Controle dos Buzzers ------------
void buzzer_init(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_config c = pwm_get_default_config();
    pwm_config_set_clkdiv(&c, 125.0f);
    pwm_init(slice, &c, true);

    pwm_set_gpio_level(pin, 0);
}

void buzzer_on(uint pin) {
    pwm_set_gpio_level(pin, 2000);
}

void buzzer_off(uint pin) {
    pwm_set_gpio_level(pin, 0);
}

void tocar_alerta_duplo() {
    for (int i = 0; i < 3; i++) {
        buzzer_on(BUZZER_A);
        buzzer_on(BUZZER_B);
        sleep_ms(280);
        buzzer_off(BUZZER_A);
        buzzer_off(BUZZER_B);
        sleep_ms(200);
    }
}

/************** MQTT CALLBACKS (LWIP) **************/
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    char payload[128];
    if (len >= sizeof(payload)) len = sizeof(payload) - 1;

    memcpy(payload, data, len);
    payload[len] = '\0';

    printf("MQTT payload: %s\n", payload);

    if (strstr(payload, "true")) {
        tocar_alerta_duplo();
    } else {
        buzzer_off(BUZZER_A);
        buzzer_off(BUZZER_B);
    }
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) {
    printf("MQTT tópico: %s (len=%u)\n", topic, (unsigned)tot_len);
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("Conectado ao broker MQTT.\n");

        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, NULL);

        err_t err = mqtt_subscribe(client, "bitdoglab/alerta", 0, NULL, NULL);
        printf("SUBSCRIBE retorno: %d\n", err);
    } else {
        printf("Falha ao conectar ao broker: %d\n", status);
    }
}

/************** Função principal **************/

int main() {
    stdio_init_all();

    // --------- UART Física ativada (GPIO 0 = TX, 1 = RX) ---------
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    printf("USB Serial e UART (GPIO0/1) ativados!\n");

    buzzer_init(BUZZER_A);
    buzzer_init(BUZZER_B);

    printf("Iniciando WiFi...\n");

    if (cyw43_arch_init()) {
        printf("Erro ao iniciar o WiFi.\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();

    int rc = cyw43_arch_wifi_connect_timeout_ms(
        "SEU_WIFI",
        "SUA_SENHA",
        CYW43_AUTH_WPA2_AES_PSK,
        30000
    );

    if (rc != 0) {
        printf("Falha ao conectar ao WiFi! Código: %d\n", rc);
        return -1;
    }

    printf("WiFi conectado!\n");

    // ---------- MQTT ----------
    ip_addr_t broker_ip;
    ipaddr_aton("192.168.1.150", &broker_ip);

    static struct mqtt_connect_client_info_t client_info = {
        .client_id = "bitdoglab_alert_client",
        .keep_alive = 60,
        .clean_session = 1,
    };

    mqtt_client_t *client = mqtt_client_new();
    mqtt_client_connect(client, &broker_ip, 1883, mqtt_connection_cb, NULL, &client_info);

    printf("Aguardando mensagens MQTT...\n");

    while (true) {
        sleep_ms(100);
    }

    return 0;
}
