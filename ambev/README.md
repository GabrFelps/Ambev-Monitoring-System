# 🚨 Guia de Configuração

## Alarme Remoto BitDogLab + Node-RED

Este guia explica como **importar e configurar** o sistema de alarme que utiliza **comunicação serial via USB** para disparar sinais sonoros em uma placa **RP2040 (BitDogLab)**.

---

## 🛠️ 1. Pré-requisitos de Software

Antes de iniciar, certifique-se de que o seu ambiente possui:

* ✅ **Node.js** (versão LTS instalada)
* ✅ **Node-RED** (instalado globalmente via npm)
* ✅ **Firmware na placa BitDogLab**

  * Código em C carregado
  * Processamento de comandos via `scanf`

---

## 📥 2. Como Importar o Fluxo (JSON)

1. Abra o Node-RED no navegador:

   ```
   http://localhost:1880
   ```
2. Clique no **Menu** (ícone de três linhas no canto superior direito).
3. Selecione **Import**.
4. Clique em **select a file to import** ou cole o código JSON.
5. Clique no botão vermelho **Import**.

---

## 🧩 3. Instalação de Dependências

Se aparecer um triângulo vermelho nos nós (`unknown`):

1. Vá em:
   **Menu → Manage palette**

2. Clique na aba **Install**

3. Pesquise por:

   ```
   node-red-node-serialport
   ```

4. Clique em **Install**

---

## ⚙️ 4. Configuração da Conexão Serial

A porta USB varia conforme o computador.

### Passo a passo:

1. Dê **duplo clique** no nó de saída serial (ex: `COM3` ou `Serial Out`)

2. Clique no **ícone do lápis** ✏️ para editar

3. Configure:

   * **Serial Port:** selecione a porta correta
   * **Baud Rate:** `115200`

4. Vá até a aba **Output** e configure:

   ```
   Add character to output messages: \n
   ```

   (necessário para o `scanf` funcionar corretamente)

5. Clique em **Update** → **Done**

---

## 🧪 5. Ativação e Testes de Comando

1. Clique em **Deploy** (botão vermelho no canto superior direito)

### Comandos disponíveis:

| Botão   | Ação                                           |
| ------- | ---------------------------------------------- |
| **ON**  | Envia `"ON"` e ativa os buzzers (GPIO 21 e 0)  |
| **OFF** | Envia `"OFF"` e desliga o alarme imediatamente |

### Monitoramento

* Utilize a aba **Debug** 🐞
* Mensagens exibidas, por exemplo:

```
🔊 STATUS: ALARME LIGADO
```

---

## ⚠️ 6. Especificações de Hardware

Configurações utilizadas no firmware C:

| Recurso        | Identificação | Detalhes                         |
| -------------- | ------------- | -------------------------------- |
| Buzzer A       | GPIO 21       | Ativado via PWM                  |
| Buzzer B       | GPIO 0        | Ativado via PWM                  |
| Botões físicos | GPIO 5 e 6    | Desligamento manual de segurança |
| Frequência     | 3000 Hz       | Frequência de alerta do sistema  |

---