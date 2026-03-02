# 🚀 ESP32 + MQTT + Node-RED  
Controle de LED via WiFi usando MQTT

Este projeto permite controlar um LED conectado na ESP32 através do Node-RED usando protocolo MQTT.

---

# 📦 Tecnologias usadas

- ESP32  
- WiFi  
- MQTT  
- Node-RED  
- Biblioteca PubSubClient  

---

# ⚙️ Requisitos

Antes de começar, você precisa:

- Arduino IDE instalada  
- Placa ESP32 configurada na IDE  
- Biblioteca **PubSubClient** instalada  
- Node-RED instalado  
- Broker MQTT rodando (ex: Mosquitto)  

---

# 🔧 Configuração da ESP32

No código Arduino, configure:

```
const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA_WIFI";
const char* mqtt_server = "IP_DO_SEU_NOTEBOOK";
```

Exemplo:

```
const char* ssid = "trojan";
const char* password = "m2er4ad45";
const char* mqtt_server = "192.168.0.5";
```

---

# 📡 Tópicos MQTT utilizados

| Função | Tópico |
|--------|--------|
| Receber comando do LED | esp32/led |
| Enviar status do LED | esp32/status |

---

# 💡 Como funciona

1. ESP32 conecta no WiFi  
2. ESP32 conecta no broker MQTT  
3. Node-RED envia:
   - "ON" → LED liga
   - "OFF" → LED desliga
4. ESP32 envia feedback:
   - "LED LIGADO"
   - "LED DESLIGADO"
   - "ESP32 conectada com sucesso!"

---

# 🔄 Importar Fluxo no Node-RED

Este projeto inclui um arquivo:

Fluxo_Ligar_LED.json

### Como importar:

1. Abra o Node-RED  
2. Clique no menu ☰ (canto superior direito)  
3. Clique em **Import**  
4. Selecione o arquivo `Fluxo_Ligar_LED.json`  
5. Clique em **Import**  
6. Clique em **Deploy**  

Pronto ✅  
Agora você pode ligar e desligar o LED pelo Node-RED e visualizar o status no Debug.

---

# 📁 Estrutura do Projeto

```
Ligar_Desligar/
│
├── Ligar_Desligar.ino
├── Fluxo_Ligar_LED.json
└── README.md
```

---

# 👨‍💻 Autor

Projeto para aprendizado de IoT com ESP32 + MQTT + Node-RED.
