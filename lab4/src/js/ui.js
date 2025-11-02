// Дополнительные функции для интерфейса
function drawSelectedAlgorithm() {
    const algorithm = document.getElementById('algorithmSelect').value;
    
    switch(algorithm) {
        case 'step':
            drawStepByStep();
            break;
        case 'dda':
            drawDDA();
            break;
        case 'bresenham':
            drawBresenham();
            break;
        case 'castle':
            drawCastlePitteway();
            break;
        case 'circle':
            drawBresenhamCircle();
            break;
    }
}

function clearCanvas() {
    drawGrid();
    document.getElementById('timing').textContent = 'Холст очищен';
    lastDrawnFunction = null;
    lastDrawnParams = null;
}

// Обновление интерфейса при выборе алгоритма
document.getElementById('algorithmSelect').addEventListener('change', function() {
    const isCircle = this.value === 'circle';
    document.getElementById('radiusLabel').style.display = isCircle ? 'block' : 'none';
});

// Инициализация при загрузке
document.addEventListener('DOMContentLoaded', function() {
    drawGrid();
    document.getElementById('scaleValue').textContent = `Масштаб: ${scale.toFixed(1)}`;
});