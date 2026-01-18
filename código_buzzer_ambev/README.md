🚨 Guia de Configuração: Alarme Remoto BitDogLab + Node-RED
Este guia explica como importar e configurar o sistema de alarme que utiliza comunicação serial via USB para disparar sinais sonoros em uma placa RP2040 (BitDogLab).

1. Pré-requisitos de Software
Antes de importar o arquivo, certifique-se de ter instalado:

Node.js (Versão LTS recomendada).

Node-RED instalado globalmente via npm.

Firmware na Placa: A BitDogLab deve estar carregada com o código C que reconhece os comandos "ON" e "OFF".

2. Como Importar o Fluxo (JSON)
Abra o Node-RED no seu navegador (geralmente em http://localhost:1880).

No canto superior direito, clique no Menu (ícone de três linhas).

Selecione a opção Import.

Clique em select a file to import e selecione o arquivo .json recebido (ou cole o código JSON na caixa de texto).

Clique no botão vermelho Import.

3. Instalação de Dependências
Se o fluxo aparecer com nós sinalizados com um triângulo vermelho ou "unknown", você precisa instalar o módulo de comunicação serial:

Vá no Menu -> Manage palette.

Clique na aba Install.

Procure por: node-red-node-serialport.

Clique em Install.

4. Configuração da Conexão Serial
Como a porta USB (COM) varia de um computador para outro, você deve ajustar a conexão:

Dê um clique duplo no nó de saída serial (geralmente nomeado como COM3 ou Serial Out).

Clique no ícone do lápis para editar a configuração da porta.

Serial Port: Verifique no seu "Gerenciador de Dispositivos" (Windows) qual porta a placa assumiu e altere aqui.

Baud Rate: Deve ser obrigatoriamente 115200.

Aba Output: Certifique-se de que no campo "Add character to output messages" esteja preenchido com \n.

Clique em Update e depois em Done.

5. Ativação e Teste
Clique no botão vermelho Deploy no canto superior direito do Node-RED.

Verifique se o status embaixo do nó serial mudou para um quadrado verde escrito "connected".

Comandos:

Clique no botão azul do nó ON: A placa deve disparar o som nos GPIOs 21 e 0.

Clique no botão azul do nó OFF: O som deve parar imediatamente.

Monitoramento: Observe a aba Debug (ícone da barata à direita) para ver as mensagens de confirmação enviadas pela placa.

6. Observações de Hardware
Frequência: O alarme está configurado para 3000Hz.

Segurança: O alarme também pode ser desligado fisicamente pressionando os botões nos GPIOs 5 ou 6 da placa.