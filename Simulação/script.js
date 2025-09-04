// Seleção de elementos
const tank1 = document.getElementById("tank1");
const tank2 = document.getElementById("tank2");
const tank3 = document.getElementById("tank3");

const liquid1 = document.getElementById("liquid1");
const liquid2 = document.getElementById("liquid2");
const liquid3 = document.getElementById("liquid3");

const valve1Icon = document.getElementById("icon-valve1");
const valve2Icon = document.getElementById("icon-valve2");

const btnValve1 = document.getElementById("btn-valve1");
const btnValve2 = document.getElementById("btn-valve2");
const btnAuto = document.getElementById("btn-auto");
const btnStop = document.getElementById("btn-stop");

const statusMessage = document.getElementById("status-message");

// Estado das válvulas
let valve1Open = false;
let valve2Open = false;
let autoMode = false;
let intervalId = null;

// Função para atualizar a cor do ícone da válvula
function updateValveIcon(valveIcon, isOpen) {
  if (isOpen) {
    valveIcon.classList.add("open");
  } else {
    valveIcon.classList.remove("open");
  }
}

// Função para encher/esvaziar um tanque
function setLiquidHeight(liquid, percent) {
  liquid.style.height = percent + "%";
}

// Funções das válvulas
function toggleValve1() {
  valve1Open = !valve1Open;
  updateValveIcon(valve1Icon, valve1Open);
  statusMessage.textContent = valve1Open ? "💧 Válvula 1 aberta" : "🚫 Válvula 1 fechada";

  if (valve1Open) {
    fillTank(liquid1, liquid2);
  }
}

function toggleValve2() {
  valve2Open = !valve2Open;
  updateValveIcon(valve2Icon, valve2Open);
  statusMessage.textContent = valve2Open ? "💧 Válvula 2 aberta" : "🚫 Válvula 2 fechada";

  if (valve2Open) {
    fillTank(liquid2, liquid3);
  }
}

// Função para encher tanque de origem para destino
function fillTank(source, target) {
  let sourceHeight = parseInt(source.style.height) || 0;
  let targetHeight = parseInt(target.style.height) || 0;

  intervalId = setInterval(() => {
    if (sourceHeight > 0 && targetHeight < 100) {
      sourceHeight -= 1;
      targetHeight += 1;
      setLiquidHeight(source, sourceHeight);
      setLiquidHeight(target, targetHeight);
    } else {
      clearInterval(intervalId);
    }
  }, 50);
}

// Função automática
function startAuto() {
  autoMode = true;
  statusMessage.textContent = "🔄 Modo automático ativado";

  let phase = 0;
  let liquid1Height = 0;
  let liquid2Height = 0;
  let liquid3Height = 0;

  intervalId = setInterval(() => {
    switch (phase) {
      case 0: // Encher tanque 1
        if (liquid1Height < 100) liquid1Height++;
        else phase++;
        break;
      case 1: // Transferir tanque1 -> tanque2
        if (liquid1Height > 0 && liquid2Height < 100) {
          liquid1Height--;
          liquid2Height++;
        } else phase++;
        break;
      case 2: // Transferir tanque2 -> tanque3
        if (liquid2Height > 0 && liquid3Height < 100) {
          liquid2Height--;
          liquid3Height++;
        } else phase++;
        break;
      case 3: // Esvaziar todos
        if (liquid3Height > 0) liquid3Height--;
        else if (liquid2Height > 0) liquid2Height--;
        else if (liquid1Height > 0) liquid1Height--;
        else phase = 0; // repetir ciclo
        break;
    }

    setLiquidHeight(liquid1, liquid1Height);
    setLiquidHeight(liquid2, liquid2Height);
    setLiquidHeight(liquid3, liquid3Height);

  }, 50);
}

// Parada de emergência
function emergencyStop() {
  clearInterval(intervalId);
  autoMode = false;
  valve1Open = false;
  valve2Open = false;
  updateValveIcon(valve1Icon, false);
  updateValveIcon(valve2Icon, false);
  statusMessage.textContent = "⛔ Parada de emergência ativada!";
}

// Eventos dos botões
btnValve1.addEventListener("click", toggleValve1);
btnValve2.addEventListener("click", toggleValve2);
btnAuto.addEventListener("click", startAuto);
btnStop.addEventListener("click", emergencyStop);

// Inicialização dos tanques
setLiquidHeight(liquid1, 0);
setLiquidHeight(liquid2, 0);
setLiquidHeight(liquid3, 0);
