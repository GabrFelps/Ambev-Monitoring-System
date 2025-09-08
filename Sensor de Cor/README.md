# 🌊 Monitor de Nível de Lodo na Água (BitDogLab + Pico W + TCS34725)

Este projeto utiliza a **Raspberry Pi Pico W (BitDogLab)** em conjunto com o **sensor de cor TCS34725 (I2C)** para medir a turbidez da água (nível de lodo).  
Os dados coletados são exibidos em um **display OLED SSD1306** e o sistema aciona LEDs de status para indicar o nível de lodo.

---

## ⚡ Objetivo
- Detectar o nível de lodo na água de forma simples e confiável.  
- Mostrar os valores de R, G, B e Clear no display OLED.  
- Classificar a água em **Baixo, Médio ou Alto nível de lodo**.  

---

## 🛠️ Componentes
- Raspberry Pi Pico W (BitDogLab)  
- Sensor de cor **TCS34725**  
- Display OLED **SSD1306** (I2C)  
- 3 LEDs (verde, amarelo, vermelho)  
- Jumpers e protoboard  

---

## 🔌 Ligações principais
- **TCS34725 → Pico W**
  - VCC → 3V3  
  - GND → GND  
  - SDA → GP14  
  - SCL → GP15  

- **LEDs**
  - Verde → GP11  
  - Amarelo → GP12  
  - Vermelho → GP13  

---

## ▶️ Funcionamento
1. O sensor lê valores de cor e luminosidade.  
2. O código calcula a intensidade média.  
3. Dependendo do valor:
   - **Baixo lodo** → LED Verde  
   - **Médio lodo** → LED Amarelo  
   - **Alto lodo** → LED Vermelho  
4. Informações são mostradas no **OLED SSD1306**.  

---

✍️ Projeto inicial para monitoramento de qualidade da água usando **BitDogLab + Raspberry Pi Pico W**.
