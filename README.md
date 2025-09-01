# Ambev-Monitoring-System
### Reademe do nosso projeto

Titulo: Automação de valvulas dos sensores 

Descrição:

Este projeto tem como objetivo solucionar um problema relacionado ao enchimento de um tanque em uma empresa do setor de bebidas. A proposta do protótipo consiste na utilização de duas válvulas: uma válvula superior, responsável por permitir a entrada do líquido quando o recipiente está vazio, e uma válvula inferior, que é acionada logo após o fechamento da superior para liberar o líquido. O processo se repete de forma cíclica, permitindo que a válvula superior seja novamente aberta quando o nível do reservatório atinge um ponto baixo, garantindo o enchimento contínuo e eficiente do sistema.

### Componentes ultilizados

## Componentes Utilizados

- **ESP32** – Microcontrolador responsável pela lógica de controle.
- **Fonte chaveada 12V** – Alimenta o relé e a válvula solenoide.
- **Fonte ajustável (3,3V ~ 12V)** – Alimenta o ESP32.
- **Relé 12V (1 canal)** – Atua como chave eletrônica para ligar/desligar a válvula.
- **Válvula solenoide 12V** – Controla a passagem do líquido.
- **Sensores de nível (superior e inferior)** – Detectam a quantidade de líquido no tanque.
- **Diodo 1N4007** – Utilizado como **diodo de flyback**, para proteger o circuito contra picos de tensão gerados pela válvula.

### Esquema eletrônico

No projeto eletrico primeiro temos o uso da fonte

## 1) Alimentação e referências

1. **Fonte 12 V (chaveada)**
    - **+12 V →** duas ligações:
        - **VCC** do **módulo de relé 12 V** (alimenta a bobina do relé).
        - **COM** do contato de potência do relé (será a “entrada” que vai para a válvula quando o relé fechar).
    - **GND (–) →** barramento de terra comum do sistema.
2. **ESP32**
    - Alimente o ESP32 com **3,3 V** (pela sua **fonte ajustável** ou pela USB).
    - **Atenção:** una os **terras** de tudo: **GND da fonte 12 V**, **GND da fonte 3,3 V/ESP32** e **GND do relé**. Sem **terra comum** o sinal de controle não referencia e o relé pode não acionar.

> Resumo da etapa: todos os GNDs juntos; 12 V só para relé e válvula; 3,3 V só para o ESP32.
> 

---

## 2) Módulo de relé (controle)

O módulo tem normalmente **VCC, GND, IN** e os contatos **COM, NO, NC**.

- **VCC (do relé) ← +12 V**
- **GND (do relé) ← GND comum**
- **IN (do relé) ← GPIO do ESP32** (ex.: **GPIO23**; use o que preferir no código)

> Observação: na maioria dos módulos de relé 12 V, o IN é “ativo em nível baixo” (LOW liga o relé, HIGH desliga). Se o seu for diferente, ajuste a lógica no código.
> 

**Contatos de potência (lado da válvula):**

- **COM ← +12 V** (já ligado acima)
- **NO → positivo da válvula** (use NO para que a válvula fique **fechada** quando o relé está **desligado**)
- **NC** não é usado.

---

## 3) Válvula solenoide 12 V e diodo de flyback

- **Positivo da válvula ← NO do relé**
- **Negativo da válvula ← GND da fonte 12 V**

**Diodo 1N4007 (flyback) em paralelo com a válvula:**

- **Cátodo (faixa) → lado positivo da válvula**
- **Ânodo → lado negativo (GND) da válvula**

> Função: suprimir o pico de tensão quando a válvula desenergiza, protegendo relé e ESP32.
> 

---

## 4) Sensores de nível (superior e inferior)

No desenho eles são **contatos secos** (tipo boia/reed). Ligue como **entrada com pull-up**:

Para **cada sensor**:

- **Um terminal → GND comum**
- **Outro terminal → um GPIO do ESP32**
    - Ex.: **Sensor inferior → GPIO18**
    - **Sensor superior → GPIO19**

No código, habilite **INPUT_PULLUP** (ou `pinMode(..., INPUT_PULLUP)` na IDE Arduino).

Assim, o pino ficará **alto (1)** quando o sensor estiver **aberto** e **baixo (0)** quando o sensor **fechar** para o GND.

> Se seus sensores forem eletrônicos (PNP/NPN), adapte:
> 
> 
> – **NPN**: saída do sensor no GPIO e **coletor aberto** para GND (com pull-up).
> 
> – **PNP**: saída no GPIO e **emissor em +3,3 V** (use divisor/opt acoplamento para não exceder 3,3 V).
> 

---

## 5) Sequência de operação (lógica)

1. **Inicialização**
    - Configura GPIO do **relé** como **saída** (estado inicial **desligado** → HIGH se o relé for ativo em LOW).
    - Configura GPIOs dos **sensores** como **INPUT_PULLUP**.
2. **Decisão de enchimento**
    - **Se o sensor inferior indicar “baixo nível”** (pino em **LOW**, contato fechado):
        - **Liga o relé** → a válvula **abre** → o tanque começa a encher.
    - **Enquanto enche**, monitore o sensor superior.
    - **Quando o sensor superior ativar** (pino **LOW**):
        - **Desliga o relé** → a válvula **fecha**.
3. **Ciclagem**
    - O ciclo se repete: quando o nível cair e **o inferior abrir → depois fechar** novamente, a automação volta a **abrir** a válvula.
4. **Condições de segurança recomendadas**
    - **Timeout**: se demorou demais para o superior acionar, **desligue** a válvula e sinalize falha.
    - **Inconsistência**: se o superior está ativo e o inferior inativo por muito tempo, registre erro (verifique sensores).
    - **Partida**: ao ligar o sistema, mantenha a válvula **fechada** até ler um estado válido dos sensores.

---

## 6) Tabela-resumo de ligações

| De | Para | Observações |
| --- | --- | --- |
| Fonte 12 V (+) | VCC do relé | Alimenta bobina do relé |
| Fonte 12 V (–) | GND comum | Terra do sistema |
| Fonte 12 V (+) | COM do relé | Entrada do contato de potência |
| NO do relé | + da válvula | Saída para a válvula |
| – da válvula | GND 12 V | Retorno |
| Diodo 1N4007 (faixa) | + da válvula | Flyback |
| Diodo 1N4007 (sem faixa) | – da válvula | Flyback |
| Fonte 3,3 V | 3V3 do ESP32 | Alimentação do ESP32 (ou use USB) |
| GND 3,3 V | GND comum | Unificar terras |
| IN do relé | GPIO do ESP32 | Ex.: GPIO23 |
| Sensor inferior (pino 1) | GND | Contato seco |
| Sensor inferior (pino 2) | GPIO do ESP32 | Ex.: GPIO18 (INPUT_PULLUP) |
| Sensor superior (pino 1) | GND | Contato seco |
| Sensor superior (pino 2) | GPIO do ESP32 | Ex.: GPIO19 (INPUT_PULLUP) |
