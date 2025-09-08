#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições do I2C (BitDogLab usa GP14 = SDA1 e GP15 = SCL1)
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15

// Endereço padrão do sensor TCS34725
#define TCS34725_ADDR 0x29
#define TCS34725_COMMAND_BIT 0x80

// Registradores importantes
#define TCS34725_ENABLE   0x00
#define TCS34725_ATIME    0x01
#define TCS34725_CONTROL  0x0F
#define TCS34725_ID       0x12
#define TCS34725_CDATAL   0x14

// Bits de controle
#define TCS34725_ENABLE_AEN   0x02
#define TCS34725_ENABLE_PON   0x01

// Função para escrever em um registrador
void tcs34725_write8(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = { TCS34725_COMMAND_BIT | reg, value };
    i2c_write_blocking(I2C_PORT, TCS34725_ADDR, buf, 2, false);
}

// Função para ler 16 bits de um registrador
uint16_t tcs34725_read16(uint8_t reg) {
    uint8_t buf[2];
    uint8_t cmd = TCS34725_COMMAND_BIT | reg;
    i2c_write_blocking(I2C_PORT, TCS34725_ADDR, &cmd, 1, true);
    i2c_read_blocking(I2C_PORT, TCS34725_ADDR, buf, 2, false);
    return (uint16_t)(buf[1] << 8 | buf[0]);
}

// Inicialização do sensor
void tcs34725_init() {
    // Alimenta o sensor
    tcs34725_write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
    sleep_ms(3);
    tcs34725_write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);

    // Tempo de integração (mais tempo = mais sensível)
    tcs34725_write8(TCS34725_ATIME, 0xD5); // ~101ms
    // Ganho (1x, 4x, 16x, 60x)
    tcs34725_write8(TCS34725_CONTROL, 0x01); // 4x
}

int main() {
    stdio_init_all();

    // Inicializa I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("🔍 Iniciando TCS34725...\n");

    // Inicializa o sensor
    tcs34725_init();

    while (true) {
        uint16_t c = tcs34725_read16(TCS34725_CDATAL);
        uint16_t r = tcs34725_read16(TCS34725_CDATAL + 2);
        uint16_t g = tcs34725_read16(TCS34725_CDATAL + 4);
        uint16_t b = tcs34725_read16(TCS34725_CDATAL + 6);

        printf("Clear=%u | R=%u | G=%u | B=%u\n", c, r, g, b);

        sleep_ms(1000);
    }
}
