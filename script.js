// Simulação de dados (substitua isso pela lógica real de obtenção de dados)
function simulateTemperatureData() {
    return 25 + Math.random() * 10; // Temperatura entre 25°C e 35°C
}

// Configuração do gráfico
const ctx = document.getElementById('temperatureChart').getContext('2d');
const chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Temperatura (°C)',
            data: [],
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.1
        }]
    },
    options: {
        responsive: true,
        scales: {
            y: {
                beginAtZero: false
            }
        }
    }
});

// Função para atualizar os dados
function updateData() {
    const temperature = simulateTemperatureData();
    document.getElementById('temp-value').textContent = `${temperature.toFixed(1)}°C`;

    const now = new Date();
    chart.data.labels.push(now.toLocaleTimeString());
    chart.data.datasets[0].data.push(temperature);

    // Manter apenas os últimos 10 pontos de dados
    if (chart.data.labels.length > 10) {
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
    }

    chart.update();
}

// Atualizar dados a cada 5 segundos
setInterval(updateData, 5000);

// Primeira atualização imediata
updateData();
