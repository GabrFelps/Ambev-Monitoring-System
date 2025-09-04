const productionLines = [
  { id: "L001", name: "Linha Coca-Cola", status: "running", temperature: 4.2, production: 450, efficiency: 95 },
  { id: "L002", name: "Linha Sprite", status: "running", temperature: 3.8, production: 380, efficiency: 88 },
  { id: "L003", name: "Linha Fanta", status: "maintenance", temperature: 15.2, production: 0, efficiency: 0 }
];

function getStatusText(status) {
  switch (status) {
    case "running": return "Em Operação";
    case "stopped": return "Parada";
    case "maintenance": return "Manutenção";
    default: return "Desconhecido";
  }
}

function renderDashboard() {
  const totalProduction = productionLines.reduce((s, l) => s + l.production, 0);
  const running = productionLines.filter(l => l.status === "running");
  const avgEfficiency = running.length > 0 ? Math.round(running.reduce((s, l) => s + l.efficiency, 0) / running.length) : 0;
  const avgTemperature = running.length > 0 ? (running.reduce((s, l) => s + l.temperature, 0) / running.length).toFixed(1) : "--";

  document.getElementById("totalProduction").innerHTML = `${totalProduction} <span>garrafas/min</span>`;
  document.getElementById("averageEfficiency").textContent = `${avgEfficiency}%`;
  document.getElementById("averageTemperature").textContent = `${avgTemperature} °C`;

  const container = document.getElementById("productionLinesContainer");
  container.innerHTML = "";

  productionLines.forEach(line => {
    const div = document.createElement("div");
    div.className = "line-card";
    div.innerHTML = `
      <div class="line-header">
        <h3>${line.name}</h3>
        <span class="status ${line.status}">${getStatusText(line.status)}</span>
      </div>
      <p><i class="fas fa-thermometer-half"></i> Temp: ${line.temperature}°C</p>
      <p><i class="fas fa-tint"></i> Produção: ${line.production} garrafas/min</p>
      <p><i class="fas fa-bolt"></i> Eficiência: ${line.efficiency}%</p>
    `;
    container.appendChild(div);
  });
}

function addNewProductionLine() {
  const newId = `L${String(productionLines.length + 1).padStart(3, '0')}`;
  const newLine = { 
    id: newId, 
    name: "Nova Linha", 
    status: "running", 
    temperature: (Math.random() * 5 + 2).toFixed(1), // Exemplo de temperatura aleatória
    production: Math.floor(Math.random() * 200) + 200, // Exemplo de produção aleatória
    efficiency: Math.floor(Math.random() * 20) + 80 // Exemplo de eficiência aleatória
  };
  productionLines.push(newLine);
  renderDashboard();
}

document.getElementById("menuToggle").addEventListener("click", () => {
  document.getElementById("sidebar").classList.toggle("active");
});

document.getElementById("addButton").addEventListener("click", addNewProductionLine);

renderDashboard();