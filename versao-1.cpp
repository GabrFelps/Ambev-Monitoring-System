#include <Arduino.h>

// Definição dos pinos (ajuste conforme sua ligação real)
#define SENSOR_NIVEL_SUP   25   // GPIO do Sensor de Nível Superior (SNS)
#define SENSOR_NIVEL_INF   26   // GPIO do Sensor de Nível Inferior (SNI)
#define RELE_VALVULA       27   // GPIO que controla o relé da válvula

// Estados possíveis da válvula
#define FECHADO 0
#define ABERTO  1

int estado_valvula = FECHADO; // Estado inicial da válvula

void setup() {
  Serial.begin(115200);

  // Configuração dos pinos
  pinMode(SENSOR_NIVEL_SUP, INPUT_PULLUP);  // Sensor superior
  pinMode(SENSOR_NIVEL_INF, INPUT_PULLUP);  // Sensor inferior
  pinMode(RELE_VALVULA, OUTPUT);

  // Garante que a válvula comece fechada
  digitalWrite(RELE_VALVULA, LOW);

  Serial.println("Sistema iniciado - Valvula FECHADA");
}

void loop() {
  // Leitura dos sensores
  bool sns = digitalRead(SENSOR_NIVEL_SUP) == LOW;  // ativo em nível baixo
  bool sni = digitalRead(SENSOR_NIVEL_INF) == LOW;  // ativo em nível baixo

  // ---------------- ESTADO FECHADO ----------------
  if (estado_valvula == FECHADO) {
    if (sns) { // Reservatório chegou no nível superior → encheu
      estado_valvula = ABERTO;
      digitalWrite(RELE_VALVULA, HIGH); // Abre a válvula
      Serial.println("SNS ATIVO → Valvula ABERTA (Esvaziando)");
    }
  }

  // ---------------- ESTADO ABERTO ----------------
  else if (estado_valvula == ABERTO) {
    if (!sni) { // Reservatório esvaziou até o nível inferior
      estado_valvula = FECHADO;
      digitalWrite(RELE_VALVULA, LOW); // Fecha a válvula
      Serial.println("SNI DESATIVADO → Valvula FECHADA (Fim do processo)");
    }
  }

  delay(200); // Pequeno delay para evitar leituras instáveis
}
