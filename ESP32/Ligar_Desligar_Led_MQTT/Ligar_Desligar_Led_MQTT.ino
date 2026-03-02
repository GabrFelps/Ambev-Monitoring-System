#include <WiFi.h>
#include <PubSubClient.h>

// ===== DADOS DA SUA REDE =====
const char* ssid = "nome";
const char* password = "senha";

// ===== IP DO SEU NOTEBOOK (BROKER MQTT) =====
const char* mqtt_server = "192.168.x.x";

// ===== OBJETOS =====
WiFiClient espClient;
PubSubClient client(espClient);

// ===== PINO DO LED =====
#define LED_PIN 2

// ===== CONECTAR WIFI =====
void setup_wifi() {
  delay(10);
  Serial.println("Conectando ao WiFi...");
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP da ESP32: ");
  Serial.println(WiFi.localIP());
}

// ===== RECEBER MENSAGENS MQTT =====
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Mensagem recebida: ");
  Serial.println(message);

  if (message == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED LIGADO");
    client.publish("esp32/estado", "LED LIGADO");
  } 
  else if (message == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED DESLIGADO");
    client.publish("esp32/estado", "LED DESLIGADO");
  }
}

// ===== RECONECTAR MQTT =====
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("Conectado ao broker MQTT!");

      // Publica status ao conectar
      client.publish("esp32/status", "ESP32 conectada com sucesso!");

      // Inscreve no tópico do LED
      client.subscribe("esp32/led");

    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // começa desligado

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ===== LOOP =====
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}