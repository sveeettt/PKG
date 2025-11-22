// Вершины и грани для буквы "К"
const vertices = [
    // Вертикальная часть буквы
    [-40, -80, 10], [-40, 80, 10],   // передняя грань
    [-40, -80, -10], [-40, 80, -10], // задняя грань
    
    // Верхняя наклонная часть
    [-40, 20, 10], [40, 80, 10],     // передняя грань
    [-40, 20, -10], [40, 80, -10],   // задняя грань
    
    // Нижняя наклонная часть
    [-40, -20, 10], [40, -80, 10],   // передняя грань
    [-40, -20, -10], [40, -80, -10], // задняя грань
    
    // Соединения между гранями
    [-40, -80, 10], [-40, -80, -10], // нижнее соединение
    [-40, 80, 10], [-40, 80, -10],   // верхнее соединение
    [40, 80, 10], [40, 80, -10],     // верхнее правое соединение
    [40, -80, 10], [40, -80, -10],   // нижнее правое соединение
    [-40, 20, 10], [-40, 20, -10],   // среднее левое соединение
];

// Получаем элементы canvas и контексты
const mainCanvas = document.getElementById('main-canvas');
const projectionsCanvas = document.getElementById('projections-canvas');
const mainCtx = mainCanvas.getContext('2d');
const projectionsCtx = projectionsCanvas.getContext('2d');

// Устанавливаем размеры canvas
function setupCanvas() {
    const containerWidth = mainCanvas.parentElement.clientWidth;
    mainCanvas.width = containerWidth;
    mainCanvas.height = 300;
    
    projectionsCanvas.width = containerWidth;
    projectionsCanvas.height = 300;
}

// Настройка элементов управления
const scaleSlider = document.getElementById('scale');
const translateXSlider = document.getElementById('translate-x');
const translateYSlider = document.getElementById('translate-y');
const translateZSlider = document.getElementById('translate-z');
const rotateXSlider = document.getElementById('rotate-x');
const rotateYSlider = document.getElementById('rotate-y');
const rotateZSlider = document.getElementById('rotate-z');
const resetBtn = document.getElementById('reset-btn');
const autoRotateBtn = document.getElementById('auto-rotate-btn');

// Отображение значений слайдеров
document.getElementById('scale-value').textContent = scaleSlider.value;
document.getElementById('translate-x-value').textContent = translateXSlider.value;
document.getElementById('translate-y-value').textContent = translateYSlider.value;
document.getElementById('translate-z-value').textContent = translateZSlider.value;
document.getElementById('rotate-x-value').textContent = rotateXSlider.value + '°';
document.getElementById('rotate-y-value').textContent = rotateYSlider.value + '°';
document.getElementById('rotate-z-value').textContent = rotateZSlider.value + '°';

// Обновление значений слайдеров
scaleSlider.addEventListener('input', () => {
    document.getElementById('scale-value').textContent = scaleSlider.value;
    render();
});
translateXSlider.addEventListener('input', () => {
    document.getElementById('translate-x-value').textContent = translateXSlider.value;
    render();
});
translateYSlider.addEventListener('input', () => {
    document.getElementById('translate-y-value').textContent = translateYSlider.value;
    render();
});
translateZSlider.addEventListener('input', () => {
    document.getElementById('translate-z-value').textContent = translateZSlider.value;
    render();
});
rotateXSlider.addEventListener('input', () => {
    document.getElementById('rotate-x-value').textContent = rotateXSlider.value + '°';
    render();
});
rotateYSlider.addEventListener('input', () => {
    document.getElementById('rotate-y-value').textContent = rotateYSlider.value + '°';
    render();
});
rotateZSlider.addEventListener('input', () => {
    document.getElementById('rotate-z-value').textContent = rotateZSlider.value + '°';
    render();
});

// Сброс преобразований
resetBtn.addEventListener('click', () => {
    scaleSlider.value = 1;
    translateXSlider.value = 0;
    translateYSlider.value = 0;
    translateZSlider.value = 0;
    rotateXSlider.value = 0;
    rotateYSlider.value = 0;
    rotateZSlider.value = 0;
    
    document.getElementById('scale-value').textContent = scaleSlider.value;
    document.getElementById('translate-x-value').textContent = translateXSlider.value;
    document.getElementById('translate-y-value').textContent = translateYSlider.value;
    document.getElementById('translate-z-value').textContent = translateZSlider.value;
    document.getElementById('rotate-x-value').textContent = rotateXSlider.value + '°';
    document.getElementById('rotate-y-value').textContent = rotateYSlider.value + '°';
    document.getElementById('rotate-z-value').textContent = rotateZSlider.value + '°';
    
    render();
});

// Автоповорот
let autoRotate = false;
autoRotateBtn.addEventListener('click', () => {
    autoRotate = !autoRotate;
    autoRotateBtn.textContent = autoRotate ? "Остановить автоповорот" : "Автоповорот";
    
    if (autoRotate) {
        autoRotateAnimation();
    }
});

function autoRotateAnimation() {
    if (!autoRotate) return;
    
    rotateXSlider.value = (parseInt(rotateXSlider.value) + 1) % 360;
    rotateYSlider.value = (parseInt(rotateYSlider.value) + 1.5) % 360;
    rotateZSlider.value = (parseInt(rotateZSlider.value) + 0.5) % 360;
    
    document.getElementById('rotate-x-value').textContent = rotateXSlider.value + '°';
    document.getElementById('rotate-y-value').textContent = rotateYSlider.value + '°';
    document.getElementById('rotate-z-value').textContent = rotateZSlider.value + '°';
    
    render();
    setTimeout(autoRotateAnimation, 50);
}

// Матричные операции
function multiplyMatrixVector(matrix, vector) {
    const result = [0, 0, 0, 0];
    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) {
            result[i] += matrix[i][j] * (j < 3 ? vector[j] : 1);
        }
    }
    return [result[0], result[1], result[2]];
}

function multiplyMatrices(a, b) {
    const result = [
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0],
        [0, 0, 0, 0]
    ];
    
    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) {
            for (let k = 0; k < 4; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    
    return result;
}

// Создание матриц преобразования
function createScaleMatrix(sx, sy, sz) {
    return [
        [sx, 0, 0, 0],
        [0, sy, 0, 0],
        [0, 0, sz, 0],
        [0, 0, 0, 1]
    ];
}

function createTranslationMatrix(tx, ty, tz) {
    return [
        [1, 0, 0, tx],
        [0, 1, 0, ty],
        [0, 0, 1, tz],
        [0, 0, 0, 1]
    ];
}

function createRotationXMatrix(angle) {
    const rad = angle * Math.PI / 180;
    const cos = Math.cos(rad);
    const sin = Math.sin(rad);
    
    return [
        [1, 0, 0, 0],
        [0, cos, -sin, 0],
        [0, sin, cos, 0],
        [0, 0, 0, 1]
    ];
}

function createRotationYMatrix(angle) {
    const rad = angle * Math.PI / 180;
    const cos = Math.cos(rad);
    const sin = Math.sin(rad);
    
    return [
        [cos, 0, sin, 0],
        [0, 1, 0, 0],
        [-sin, 0, cos, 0],
        [0, 0, 0, 1]
    ];
}

function createRotationZMatrix(angle) {
    const rad = angle * Math.PI / 180;
    const cos = Math.cos(rad);
    const sin = Math.sin(rad);
    
    return [
        [cos, -sin, 0, 0],
        [sin, cos, 0, 0],
        [0, 0, 1, 0],
        [0, 0, 0, 1]
    ];
}

// Функция рендеринга
function render() {
    // Получаем значения преобразований
    const scale = parseFloat(scaleSlider.value);
    const tx = parseInt(translateXSlider.value);
    const ty = parseInt(translateYSlider.value);
    const tz = parseInt(translateZSlider.value);
    const rx = parseInt(rotateXSlider.value);
    const ry = parseInt(rotateYSlider.value);
    const rz = parseInt(rotateZSlider.value);
    
    // Создаем матрицы преобразования
    const scaleMatrix = createScaleMatrix(scale, scale, scale);
    const translationMatrix = createTranslationMatrix(tx, ty, tz);
    const rotationXMatrix = createRotationXMatrix(rx);
    const rotationYMatrix = createRotationYMatrix(ry);
    const rotationZMatrix = createRotationZMatrix(rz);
    
    // Объединяем матрицы преобразования
    let transformationMatrix = multiplyMatrices(scaleMatrix, rotationXMatrix);
    transformationMatrix = multiplyMatrices(transformationMatrix, rotationYMatrix);
    transformationMatrix = multiplyMatrices(transformationMatrix, rotationZMatrix);
    transformationMatrix = multiplyMatrices(transformationMatrix, translationMatrix);
    
    // Отображаем матрицу преобразования
    displayTransformationMatrix(transformationMatrix);
    
    // Очищаем холсты
    mainCtx.clearRect(0, 0, mainCanvas.width, mainCanvas.height);
    projectionsCtx.clearRect(0, 0, projectionsCanvas.width, projectionsCanvas.height);
    
    // Рисуем 3D объект
    draw3DObject(mainCtx, mainCanvas.width, mainCanvas.height, transformationMatrix);
    
    // Рисуем проекции
    drawProjections(projectionsCtx, projectionsCanvas.width, projectionsCanvas.height, transformationMatrix);
}

// Отображение матрицы преобразования
function displayTransformationMatrix(matrix) {
    let output = "";
    for (let i = 0; i < 4; i++) {
        output += "[ ";
        for (let j = 0; j < 4; j++) {
            output += matrix[i][j].toFixed(2);
            if (j < 3) output += ", ";
        }
        output += " ]\n";
    }
    document.getElementById('matrix-output').textContent = output;
}

// Рисование 3D объекта
function draw3DObject(ctx, width, height, transformationMatrix) {
    // Центр холста
    const centerX = width / 2;
    const centerY = height / 2;
    
    // Рисуем оси координат
    drawCoordinateSystem(ctx, centerX, centerY);
    
    // Применяем преобразования к вершинам
    const transformedVertices = vertices.map(vertex => 
        multiplyMatrixVector(transformationMatrix, vertex)
    );
    
    // Рисуем линии
    ctx.strokeStyle = '#fdbb2d';
    ctx.lineWidth = 3;
    ctx.beginPath();
    
    for (let i = 0; i < transformedVertices.length; i += 2) {
        const v1 = transformedVertices[i];
        const v2 = transformedVertices[i+1];
        
        // Простая перспективная проекция
        const scale = 200 / (200 + v1[2]);
        const x1 = centerX + v1[0] * scale;
        const y1 = centerY - v1[1] * scale;
        
        const scale2 = 200 / (200 + v2[2]);
        const x2 = centerX + v2[0] * scale2;
        const y2 = centerY - v2[1] * scale2;
        
        ctx.moveTo(x1, y1);
        ctx.lineTo(x2, y2);
    }
    
    ctx.stroke();
}

// Рисование системы координат
function drawCoordinateSystem(ctx, centerX, centerY) {
    // Ось X (красная)
    ctx.strokeStyle = '#ff4444';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(centerX, centerY);
    ctx.lineTo(centerX + 100, centerY);
    ctx.stroke();
    
    // Ось Y (зеленая)
    ctx.strokeStyle = '#44ff44';
    ctx.beginPath();
    ctx.moveTo(centerX, centerY);
    ctx.lineTo(centerX, centerY - 100);
    ctx.stroke();
    
    // Ось Z (синяя)
    ctx.strokeStyle = '#4444ff';
    ctx.beginPath();
    ctx.moveTo(centerX, centerY);
    ctx.lineTo(centerX - 70, centerY + 70);
    ctx.stroke();
    
    // Подписи осей
    ctx.fillStyle = '#ff4444';
    ctx.fillText('X', centerX + 105, centerY + 15);
    ctx.fillStyle = '#44ff44';
    ctx.fillText('Y', centerX - 15, centerY - 105);
    ctx.fillStyle = '#4444ff';
    ctx.fillText('Z', centerX - 85, centerY + 85);
}

// Рисование проекций
function drawProjections(ctx, width, height, transformationMatrix) {
    const sectionWidth = width / 3;
    
    // Применяем преобразования к вершинам
    const transformedVertices = vertices.map(vertex => 
        multiplyMatrixVector(transformationMatrix, vertex)
    );
    
    // Проекция Oxy (сверху)
    drawProjection(ctx, 0, 0, sectionWidth, height, transformedVertices, 
                  (v) => [v[0], -v[1]], "Oxy");
    
    // Проекция Oxz (спереди)
    drawProjection(ctx, sectionWidth, 0, sectionWidth, height, transformedVertices, 
                  (v) => [v[0], -v[2]], "Oxz");
    
    // Проекция Oyz (сбоку)
    drawProjection(ctx, sectionWidth * 2, 0, sectionWidth, height, transformedVertices, 
                  (v) => [v[1], -v[2]], "Oyz");
}

// Рисование одной проекции
function drawProjection(ctx, x, y, width, height, vertices, projectionFunc, label) {
    const centerX = x + width / 2;
    const centerY = y + height / 2;
    const scale = 1.5;
    
    // Рисуем рамку для проекции
    ctx.strokeStyle = 'rgba(255, 255, 255, 0.3)';
    ctx.strokeRect(x, y, width, height);
    
    // Рисуем оси координат для проекции
    ctx.strokeStyle = '#ffffff';
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(x + 10, centerY);
    ctx.lineTo(x + width - 10, centerY);
    ctx.moveTo(centerX, y + 10);
    ctx.lineTo(centerX, y + height - 10);
    ctx.stroke();
    
    // Подпись проекции
    ctx.fillStyle = '#fdbb2d';
    ctx.fillText(label, x + 10, y + 20);
    
    // Рисуем проекцию буквы
    ctx.strokeStyle = '#fdbb2d';
    ctx.lineWidth = 2;
    ctx.beginPath();
    
    for (let i = 0; i < vertices.length; i += 2) {
        const v1 = vertices[i];
        const v2 = vertices[i+1];
        
        const [x1, y1] = projectionFunc(v1);
        const [x2, y2] = projectionFunc(v2);
        
        ctx.moveTo(centerX + x1 * scale, centerY + y1 * scale);
        ctx.lineTo(centerX + x2 * scale, centerY + y2 * scale);
    }
    
    ctx.stroke();
}

// Инициализация при загрузке страницы
window.addEventListener('load', () => {
    setupCanvas();
    render();
});

// Обработка изменения размера окна
window.addEventListener('resize', () => {
    setupCanvas();
    render();
});