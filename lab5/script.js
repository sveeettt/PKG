const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

let scale = 5; 
const originX = canvas.width / 2; 
const originY = canvas.height / 2; 
let gridSpacing = scale * 10; 

let currentAlgorithm = 'midpoint';

// Обработчик колесика мыши для изменения масштаба
canvas.addEventListener('wheel', function(e) {
    e.preventDefault();
    
    const delta = e.deltaY > 0 ? -1 : 1;
    const newScale = Math.max(1, Math.min(20, scale + delta * 0.5));
    
    if (newScale !== scale) {
        scale = newScale;
        document.getElementById("scale").value = scale;
        gridSpacing = scale * 10;
        draw();
    }
});

function switchTab(algorithm) {
    document.querySelectorAll('.tab').forEach(tab => tab.classList.remove('active'));
    document.querySelectorAll('.tab-content').forEach(content => content.classList.remove('active'));
    
    if (algorithm === 'midpoint') {
        document.querySelector('.tab:nth-child(1)').classList.add('active');
        document.getElementById('midpoint-tab').classList.add('active');
        document.getElementById('midpoint-desc').style.display = 'block';
        document.getElementById('sutherland-hodgman-desc').style.display = 'none';
        currentAlgorithm = 'midpoint';
    } else {
        document.querySelector('.tab:nth-child(2)').classList.add('active');
        document.getElementById('sutherland-hodgman-tab').classList.add('active');
        document.getElementById('midpoint-desc').style.display = 'none';
        document.getElementById('sutherland-hodgman-desc').style.display = 'block';
        currentAlgorithm = 'sutherland-hodgman';
    }
    
    draw();
}

function updateScale() {
    scale = parseFloat(document.getElementById("scale").value);
    gridSpacing = scale * 10;
    draw();
}

function drawGrid() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Рисуем сетку
    ctx.strokeStyle = "#ddd";
    ctx.lineWidth = 0.5;
    for (let x = 0; x < canvas.width; x += gridSpacing) {
        ctx.beginPath();
        ctx.moveTo(x, 0);
        ctx.lineTo(x, canvas.height);
        ctx.stroke();
    }
    for (let y = 0; y < canvas.height; y += gridSpacing) {
        ctx.beginPath();
        ctx.moveTo(0, y);
        ctx.lineTo(canvas.width, y);
        ctx.stroke();
    }

    // Рисуем оси координат
    ctx.strokeStyle = "black";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(0, originY);
    ctx.lineTo(canvas.width, originY);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo(originX, 0);
    ctx.lineTo(originX, canvas.height);
    ctx.stroke();

    
    ctx.font = "18px Arial";
    ctx.fillStyle = "black";
    ctx.fillText("X", canvas.width - 20, originY - 10);
    ctx.fillText("Y", originX + 10, 20);
    ctx.fillText(`Масштаб: ${scale.toFixed(1)}x`, 10, 20);
}

function toCanvasX(x) {
    return originX + x * scale;
}

function toCanvasY(y) {
    return originY - y * scale;
}

function drawRectangle(xmin, ymin, xmax, ymax) {
    ctx.strokeStyle = "#00f";
    ctx.lineWidth = 2;
    ctx.strokeRect(toCanvasX(xmin), toCanvasY(ymax), (xmax - xmin) * scale, (ymax - ymin) * scale);
}

function drawSegment(x1, y1, x2, y2, color) {
    ctx.strokeStyle = color;
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(toCanvasX(x1), toCanvasY(y1));
    ctx.lineTo(toCanvasX(x2), toCanvasY(y2));
    ctx.stroke();
}

function drawPolygon(points, color, lineWidth = 2) {
    if (points.length < 2) return;
    
    ctx.strokeStyle = color;
    ctx.lineWidth = lineWidth;
    ctx.beginPath();
    ctx.moveTo(toCanvasX(points[0][0]), toCanvasY(points[0][1]));
    for (let i = 1; i < points.length; i++) {
        ctx.lineTo(toCanvasX(points[i][0]), toCanvasY(points[i][1]));
    }
    ctx.closePath();
    ctx.stroke();
}

function fillPolygon(points, color) {
    if (points.length < 3) return;
    
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.moveTo(toCanvasX(points[0][0]), toCanvasY(points[0][1]));
    for (let i = 1; i < points.length; i++) {
        ctx.lineTo(toCanvasX(points[i][0]), toCanvasY(points[i][1]));
    }
    ctx.closePath();
    ctx.fill();
}

// Алгоритм средней точки
function getRegionCode(x, y, xmin, ymin, xmax, ymax) {
    let code = 0;
    if (x < xmin) code |= 1;
    if (x > xmax) code |= 2;
    if (y < ymin) code |= 4;
    if (y > ymax) code |= 8;
    return code;
}

function midpointClip(x1, y1, x2, y2, xmin, ymin, xmax, ymax) {
    let code1 = getRegionCode(x1, y1, xmin, ymin, xmax, ymax);
    let code2 = getRegionCode(x2, y2, xmin, ymin, xmax, ymax);
    
    if (code1 === 0 && code2 === 0) {
        drawSegment(x1, y1, x2, y2, 'green');
        return;
    }
    
    if ((code1 & code2) !== 0) {
        return;
    }
    
    if (Math.abs(x1 - x2) < 0.1 && Math.abs(y1 - y2) < 0.1) {
        return;
    }
    
    let xm = (x1 + x2) / 2;
    let ym = (y1 + y2) / 2;
    
    midpointClip(x1, y1, xm, ym, xmin, ymin, xmax, ymax);
    midpointClip(xm, ym, x2, y2, xmin, ymin, xmax, ymax);
}

//алгоритм Сазерленда-Ходжмана
function sutherlandHodgmanClip(subjectPolygon, clipPolygon) {
    if (subjectPolygon.length < 3) return [];
    
    let outputList = [...subjectPolygon];
    
    for (let i = 0; i < clipPolygon.length; i++) {
        const inputList = outputList;
        outputList = [];
        
        const A = clipPolygon[i];
        const B = clipPolygon[(i + 1) % clipPolygon.length];
        
        for (let j = 0; j < inputList.length; j++) {
            const P = inputList[j];
            const Q = inputList[(j + 1) % inputList.length];
            
            const Pinside = isInside(P, A, B);
            const Qinside = isInside(Q, A, B);
            
            if (Pinside) {
                outputList.push(P);
            }
            
            if (Pinside !== Qinside) {
                const intersection = computeIntersection(P, Q, A, B);
                if (intersection) {
                    outputList.push(intersection);
                }
            }
        }
        
        if (outputList.length === 0) break;
    }
    
    return outputList;
}

function isInside(point, A, B) {
   
    const crossProduct = (B[0] - A[0]) * (point[1] - A[1]) - (B[1] - A[1]) * (point[0] - A[0]);
    return crossProduct <= 0; 

function computeIntersection(P, Q, A, B) {
    const x1 = P[0], y1 = P[1];
    const x2 = Q[0], y2 = Q[1];
    const x3 = A[0], y3 = A[1];
    const x4 = B[0], y4 = B[1];
    
    const denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    
    if (Math.abs(denominator) < 1e-10) {
        return null; 
    }
    
    const t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    const u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;
    
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        return [
            x1 + t * (x2 - x1),
            y1 + t * (y2 - y1)
        ];
    }
    
    return null;
}

function parsePolygon(polygonStr) {
    const coords = polygonStr.trim().split(/\s+/).map(Number);
    const points = [];
    for (let i = 0; i < coords.length; i += 2) {
        if (i + 1 < coords.length) {
            points.push([coords[i], coords[i + 1]]);
        }
    }
    return points;
}

function draw() {
    drawGrid();

    if (currentAlgorithm === 'midpoint') {
        
        const windowInput = document.getElementById("window").value.split(" ").map(Number);
        const [xmin, ymin, xmax, ymax] = windowInput;

        drawRectangle(xmin, ymin, xmax, ymax);

        const segmentsInput = document.getElementById("segments").value.trim().split("\n");
        segmentsInput.forEach(segment => {
            const [x1, y1, x2, y2] = segment.split(" ").map(Number);
            drawSegment(x1, y1, x2, y2, 'red'); 
            midpointClip(x1, y1, x2, y2, xmin, ymin, xmax, ymax);
        });
    } else {
        // Алгоритм Сазерленда-Ходжмана
        const clipPolygonInput = document.getElementById("clipPolygon").value.trim().split("\n").map(line => line.split(" ").map(Number));
        const subjectPolygonsInput = document.getElementById("subjectPolygons").value.trim().split("\n");
        
        // Рисуем отсекающий многоугольник
        drawPolygon(clipPolygonInput, "blue", 3);
        
        
        subjectPolygonsInput.forEach(polygonStr => {
            if (polygonStr.trim() === "") return;
            
            const subjectPolygon = parsePolygon(polygonStr);
            
            if (subjectPolygon.length < 3) return;
            
            
            drawPolygon(subjectPolygon, "red", 2);
            
            
            const clippedPolygon = sutherlandHodgmanClip(subjectPolygon, clipPolygonInput);
            
            // Рисуем отсеченный многоугольник зеленым
            if (clippedPolygon.length >= 3) {
                fillPolygon(clippedPolygon, "rgba(0, 255, 0, 0.6)");
                drawPolygon(clippedPolygon, "green", 3);
            }
        });
    }
}

function clearCanvas() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    drawGrid();
    

    if (currentAlgorithm === 'midpoint') {
        document.getElementById("segments").value = "-60 -60 60 60\n-80 0 80 0\n0 -80 0 80\n-70 -40 70 40\n-50 -70 50 70";
        document.getElementById("window").value = "-50 -50 50 50";
    } else {
        document.getElementById("clipPolygon").value = "-40 -40\n-40 40\n40 40\n40 -40";
        document.getElementById("subjectPolygons").value = "-60 -60 -60 60 60 60 60 -60\n-50 0 0 50 50 0 0 -50\n-70 -30 -30 70 50 -20\n20 20 20 60 60 60 60 20";
    }
}

// Инициализация
draw();
document.getElementById('sutherland-hodgman-desc').style.display = 'none';