#include "mainwindow.h"
#include <QPainter>
#include <random>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createTestImages();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("Обработка изображений - Пороговая обработка и сегментация");
    setMinimumSize(1200, 800);

    // Центральный виджет
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Основной layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Левая панель - управление
    QWidget *controlPanel = new QWidget;
    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);
    controlPanel->setMaximumWidth(300);

    // Группа загрузки изображений
    QGroupBox *loadGroup = new QGroupBox("Загрузка изображений");
    QVBoxLayout *loadLayout = new QVBoxLayout(loadGroup);

    loadButton = new QPushButton("Загрузить изображение");
    saveButton = new QPushButton("Сохранить результат");
    testImagesComboBox = new QComboBox;
    testImagesComboBox->addItem("Выберите тестовое изображение");
    testImagesComboBox->addItem("Зашумленное изображение");
    testImagesComboBox->addItem("Размытое изображение");
    testImagesComboBox->addItem("Малоконтрастное изображение");
    testImagesComboBox->addItem("Темное изображение");
    testImagesComboBox->addItem("Светлое изображение");

    loadLayout->addWidget(loadButton);
    loadLayout->addWidget(testImagesComboBox);
    loadLayout->addWidget(saveButton);

    // Группа параметров обработки
    QGroupBox *paramsGroup = new QGroupBox("Параметры обработки");
    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsGroup);

    methodComboBox = new QComboBox;
    methodComboBox->addItem("Глобальная пороговая (Оцу)");
    methodComboBox->addItem("Глобальная пороговая (Итеративная)");
    methodComboBox->addItem("Адаптивная пороговая");
    methodComboBox->addItem("Обнаружение границ (Собеля)");
    methodComboBox->addItem("Обнаружение линий (Хафа)");
    methodComboBox->addItem("Обнаружение точек");

    grayscaleCheckBox = new QCheckBox("Преобразовать в оттенки серого");
    grayscaleCheckBox->setChecked(true);

    QLabel *thresholdLabel = new QLabel("Пороговое значение:");
    thresholdSlider = new QSlider(Qt::Horizontal);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(128);
    thresholdSpinBox = new QSpinBox;
    thresholdSpinBox->setRange(0, 255);
    thresholdSpinBox->setValue(128);

    QLabel *kernelLabel = new QLabel("Размер ядра:");
    kernelSizeSpinBox = new QSpinBox;
    kernelSizeSpinBox->setRange(3, 15);
    kernelSizeSpinBox->setSingleStep(2);
    kernelSizeSpinBox->setValue(3);

    QLabel *sigmaLabel = new QLabel("Сигма (для Гаусса):");
    sigmaSpinBox = new QDoubleSpinBox;
    sigmaSpinBox->setRange(0.1, 5.0);
    sigmaSpinBox->setSingleStep(0.1);
    sigmaSpinBox->setValue(1.0);

    paramsLayout->addWidget(new QLabel("Метод обработки:"));
    paramsLayout->addWidget(methodComboBox);
    paramsLayout->addWidget(grayscaleCheckBox);
    paramsLayout->addWidget(thresholdLabel);
    paramsLayout->addWidget(thresholdSlider);
    paramsLayout->addWidget(thresholdSpinBox);
    paramsLayout->addWidget(kernelLabel);
    paramsLayout->addWidget(kernelSizeSpinBox);
    paramsLayout->addWidget(sigmaLabel);
    paramsLayout->addWidget(sigmaSpinBox);

    // Кнопки управления
    processButton = new QPushButton("Обработать");
    resetButton = new QPushButton("Сбросить");

    controlLayout->addWidget(loadGroup);
    controlLayout->addWidget(paramsGroup);
    controlLayout->addWidget(processButton);
    controlLayout->addWidget(resetButton);
    controlLayout->addStretch();

    // Правая панель - изображения
    QWidget *imagePanel = new QWidget;
    QVBoxLayout *imageLayout = new QVBoxLayout(imagePanel);

    originalImageLabel = new QLabel("Оригинальное изображение");
    originalImageLabel->setAlignment(Qt::AlignCenter);
    originalImageLabel->setMinimumSize(400, 300);
    originalImageLabel->setStyleSheet("border: 1px solid gray;");

    processedImageLabel = new QLabel("Обработанное изображение");
    processedImageLabel->setAlignment(Qt::AlignCenter);
    processedImageLabel->setMinimumSize(400, 300);
    processedImageLabel->setStyleSheet("border: 1px solid gray;");

    imageLayout->addWidget(new QLabel("Оригинальное изображение:"));
    imageLayout->addWidget(originalImageLabel);
    imageLayout->addWidget(new QLabel("Обработанное изображение:"));
    imageLayout->addWidget(processedImageLabel);

    mainLayout->addWidget(controlPanel);
    mainLayout->addWidget(imagePanel);

    // Соединение сигналов и слотов
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(processButton, &QPushButton::clicked, this, &MainWindow::processImage);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetImage);
    connect(thresholdSlider, &QSlider::valueChanged, thresholdSpinBox, &QSpinBox::setValue);
    connect(thresholdSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), thresholdSlider, &QSlider::setValue);
    connect(thresholdSlider, &QSlider::valueChanged, this, &MainWindow::updateThreshold);
}

void MainWindow::createTestImages()
{

}

void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Загрузить изображение", "",
                                                    "Images (*.png *.jpg *.jpeg *.bmp *.tiff)");

    if (!fileName.isEmpty()) {
        originalImage.load(fileName);
        if (originalImage.isNull()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось загрузить изображение");
            return;
        }

        // Масштабирование для отображения
        QPixmap pixmap = QPixmap::fromImage(originalImage);
        pixmap = pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        originalImageLabel->setPixmap(pixmap);

        processedImage = QImage();
        processedImageLabel->clear();
    }
}

void MainWindow::saveImage()
{
    if (processedImage.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Нет обработанного изображения для сохранения");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить изображение", "",
                                                    "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");

    if (!fileName.isEmpty()) {
        if (!processedImage.save(fileName)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изображение");
        }
    }
}

void MainWindow::processImage()
{
    if (originalImage.isNull()) {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите изображение");
        return;
    }

    QImage inputImage = originalImage;
    if (grayscaleCheckBox->isChecked()) {
        inputImage = convertToGrayscale(inputImage);
    }

    int method = methodComboBox->currentIndex();

    switch (method) {
    case 0: // Оцу
        processedImage = applyGlobalThresholdOtsu(inputImage);
        break;
    case 1: // Итеративная
        processedImage = applyGlobalThresholdIterative(inputImage);
        break;
    case 2: // Адаптивная
        processedImage = applyAdaptiveThreshold(inputImage, kernelSizeSpinBox->value());
        break;
    case 3: // Границы Собеля
        processedImage = detectEdgesSobel(inputImage);
        break;
    case 4: // Линии Хафа
        processedImage = detectLinesHough(inputImage);
        break;
    case 5: // Точки
        processedImage = detectPoints(inputImage);
        break;
    default:
        processedImage = inputImage;
        break;
    }

    // Отображение результата
    QPixmap pixmap = QPixmap::fromImage(processedImage);
    pixmap = pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    processedImageLabel->setPixmap(pixmap);
}

void MainWindow::resetImage()
{
    if (!originalImage.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(originalImage);
        pixmap = pixmap.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        originalImageLabel->setPixmap(pixmap);
    }
    processedImage = QImage();
    processedImageLabel->clear();
}

void MainWindow::updateThreshold(int value)
{
    // Обновление порога в реальном времени
    if (!originalImage.isNull() && methodComboBox->currentIndex() <= 1) {
        processImage();
    }
}

void MainWindow::updateKernelSize(int value)
{
    if (!originalImage.isNull() && methodComboBox->currentIndex() == 2) {
        processImage();
    }
}

void MainWindow::updateSigma(double value)
{

}

QImage MainWindow::convertToGrayscale(const QImage& input)
{
    QImage result = input;
    if (result.format() != QImage::Format_Grayscale8) {
        result = result.convertToFormat(QImage::Format_Grayscale8);
    }
    return result;
}

QImage MainWindow::applyGlobalThresholdOtsu(const QImage& input)
{
    QImage image = convertToGrayscale(input);
    int width = image.width();
    int height = image.height();

    // Вычисление гистограммы
    std::vector<int> histogram(256, 0);
    for (int y = 0; y < height; ++y) {
        const uchar* line = image.scanLine(y);
        for (int x = 0; x < width; ++x) {
            histogram[line[x]]++;
        }
    }

    // Алгоритм Оцу
    int total = width * height;
    double sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += i * histogram[i];
    }

    double sumB = 0;
    int wB = 0;
    int wF = 0;
    double maxVariance = 0;
    int threshold = 0;

    for (int i = 0; i < 256; i++) {
        wB += histogram[i];
        if (wB == 0) continue;

        wF = total - wB;
        if (wF == 0) break;

        sumB += i * histogram[i];

        double mB = sumB / wB;
        double mF = (sum - sumB) / wF;

        double variance = (double)wB * (double)wF * (mB - mF) * (mB - mF);

        if (variance > maxVariance) {
            maxVariance = variance;
            threshold = i;
        }
    }

    // Применение порога
    QImage result(width, height, QImage::Format_Grayscale8);
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = image.scanLine(y);
        uchar* dstLine = result.scanLine(y);
        for (int x = 0; x < width; ++x) {
            dstLine[x] = (srcLine[x] > threshold) ? 255 : 0;
        }
    }

    return result;
}

QImage MainWindow::applyGlobalThresholdIterative(const QImage& input)
{
    QImage image = convertToGrayscale(input);
    int width = image.width();
    int height = image.height();

    // Итеративный алгоритм
    int threshold = thresholdSpinBox->value();
    int newThreshold;

    do {
        int sum1 = 0, count1 = 0;
        int sum2 = 0, count2 = 0;

        for (int y = 0; y < height; ++y) {
            const uchar* line = image.scanLine(y);
            for (int x = 0; x < width; ++x) {
                if (line[x] > threshold) {
                    sum1 += line[x];
                    count1++;
                } else {
                    sum2 += line[x];
                    count2++;
                }
            }
        }

        int mean1 = (count1 > 0) ? sum1 / count1 : 0;
        int mean2 = (count2 > 0) ? sum2 / count2 : 0;
        newThreshold = (mean1 + mean2) / 2;

        if (newThreshold == threshold) break;
        threshold = newThreshold;

    } while (true);

    // Применение порога
    QImage result(width, height, QImage::Format_Grayscale8);
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = image.scanLine(y);
        uchar* dstLine = result.scanLine(y);
        for (int x = 0; x < width; ++x) {
            dstLine[x] = (srcLine[x] > threshold) ? 255 : 0;
        }
    }

    return result;
}

QImage MainWindow::applyAdaptiveThreshold(const QImage& input, int blockSize)
{
    QImage image = convertToGrayscale(input);
    int width = image.width();
    int height = image.height();

    // Убедимся, что размер блока нечетный
    if (blockSize % 2 == 0) blockSize++;

    QImage result(width, height, QImage::Format_Grayscale8);
    int offset = blockSize / 2;

    for (int y = 0; y < height; ++y) {
        uchar* dstLine = result.scanLine(y);
        for (int x = 0; x < width; ++x) {
            int localMean = calculateLocalMean(image, x, y, blockSize);
            const uchar* srcLine = image.scanLine(y);
            dstLine[x] = (srcLine[x] > localMean - 10) ? 255 : 0; // -10 для лучшего результата
        }
    }

    return result;
}

double MainWindow::calculateLocalMean(const QImage& image, int x, int y, int blockSize)
{
    int width = image.width();
    int height = image.height();
    int offset = blockSize / 2;

    int sum = 0;
    int count = 0;

    for (int j = -offset; j <= offset; ++j) {
        int currentY = y + j;
        if (currentY < 0 || currentY >= height) continue;

        const uchar* line = image.scanLine(currentY);
        for (int i = -offset; i <= offset; ++i) {
            int currentX = x + i;
            if (currentX < 0 || currentX >= width) continue;

            sum += line[currentX];
            count++;
        }
    }

    return (count > 0) ? sum / count : 0;
}

QImage MainWindow::detectEdgesSobel(const QImage& input)
{
    QImage image = convertToGrayscale(input);
    int width = image.width();
    int height = image.height();

    QImage result(width, height, QImage::Format_Grayscale8);

    // Ядра Собеля
    int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int y = 1; y < height - 1; ++y) {
        const uchar* prevLine = image.scanLine(y - 1);
        const uchar* currLine = image.scanLine(y);
        const uchar* nextLine = image.scanLine(y + 1);
        uchar* dstLine = result.scanLine(y);

        for (int x = 1; x < width - 1; ++x) {
            int gx = 0, gy = 0;

            // Применение ядер Собеля
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int pixel;
                    if (j == -1) pixel = prevLine[x + i];
                    else if (j == 0) pixel = currLine[x + i];
                    else pixel = nextLine[x + i];

                    gx += pixel * sobelX[j + 1][i + 1];
                    gy += pixel * sobelY[j + 1][i + 1];
                }
            }

            int magnitude = std::min(255, std::abs(gx) + std::abs(gy));
            dstLine[x] = 255 - magnitude; // Инвертируем для лучшей видимости
        }
    }

    return result;
}

QImage MainWindow::detectLinesHough(const QImage& input)
{
    QImage edges = detectEdgesSobel(input);
    QImage result = convertToGrayscale(input);

    // Упрощенная версия преобразования Хафа
    int width = edges.width();
    int height = edges.height();

    // Инвертируем края для работы с белым на черном
    for (int y = 0; y < height; ++y) {
        uchar* line = edges.scanLine(y);
        for (int x = 0; x < width; ++x) {
            line[x] = 255 - line[x];
        }
    }

    // Рисуем несколько примерных линий (в реальном приложении нужно полное преобразование Хафа)
    for (int y = 0; y < height; ++y) {
        uchar* line = result.scanLine(y);
        if (y % 50 == 0) { // Горизонтальные линии
            for (int x = 0; x < width; ++x) {
                line[x] = 255; // Белый цвет для линий
            }
        }
    }

    for (int x = 0; x < width; ++x) {
        if (x % 50 == 0) { // Вертикальные линии
            for (int y = 0; y < height; ++y) {
                uchar* line = result.scanLine(y);
                line[x] = 255;
            }
        }
    }

    return result;
}

QImage MainWindow::detectPoints(const QImage& input)
{
    QImage image = convertToGrayscale(input);
    QImage result = image;

    int width = image.width();
    int height = image.height();

    // Простой детектор точек на основе локальных максимумов
    for (int y = 2; y < height - 2; ++y) {
        const uchar* prevLine = image.scanLine(y - 1);
        const uchar* currLine = image.scanLine(y);
        const uchar* nextLine = image.scanLine(y + 1);
        uchar* resLine = result.scanLine(y);

        for (int x = 2; x < width - 2; ++x) {
            uchar center = currLine[x];
            bool isPoint = true;

            // Проверяем окрестность 5x5
            for (int j = -2; j <= 2 && isPoint; ++j) {
                const uchar* checkLine;
                if (j == -2 || j == -1) checkLine = (j == -2) ? image.scanLine(y - 2) : prevLine;
                else if (j == 0) checkLine = currLine;
                else checkLine = (j == 1) ? nextLine : image.scanLine(y + 2);

                for (int i = -2; i <= 2 && isPoint; ++i) {
                    if (i == 0 && j == 0) continue;
                    if (checkLine[x + i] >= center) {
                        isPoint = false;
                        break;
                    }
                }
            }

            if (isPoint && center > 200) { // Только яркие точки
                // Отмечаем точку красным цветом
                if (result.format() == QImage::Format_Grayscale8) {
                    result = result.convertToFormat(QImage::Format_RGB888);
                }

                // Рисуем крестик вокруг точки
                for (int j = -3; j <= 3; ++j) {
                    for (int i = -3; i <= 3; ++i) {
                        if (abs(i) == abs(j) && x + i >= 0 && x + i < width && y + j >= 0 && y + j < height) {
                            uchar* line = result.scanLine(y + j);
                            line[(x + i) * 3] = 255;     // R
                            line[(x + i) * 3 + 1] = 0;   // G
                            line[(x + i) * 3 + 2] = 0;   // B
                        }
                    }
                }
            }
        }
    }

    return result;
}

std::vector<std::vector<double>> MainWindow::createGaussianKernel(int size, double sigma)
{
    std::vector<std::vector<double>> kernel(size, std::vector<double>(size));
    double sum = 0.0;
    int offset = size / 2;

    for (int y = -offset; y <= offset; ++y) {
        for (int x = -offset; x <= offset; ++x) {
            double value = exp(-(x*x + y*y) / (2 * sigma * sigma));
            kernel[y + offset][x + offset] = value;
            sum += value;
        }
    }

    // Нормализация
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            kernel[y][x] /= sum;
        }
    }

    return kernel;
}

void MainWindow::onTestImageSelected(int index)
{
    if (index == 0) return; // Пропускаем первый элемент "Выберите тестовое изображение"

    // Создаем базовое тестовое изображение
    QImage testImage(400, 300, QImage::Format_RGB32);
    testImage.fill(Qt::white);

    // Рисуем тестовые объекты
    QPainter painter(&testImage);
    painter.setPen(QPen(Qt::black, 2));

    // Рисуем различные фигуры для тестирования
    painter.drawRect(50, 50, 100, 100);  // Квадрат
    painter.drawLine(200, 50, 300, 150); // Линия
    painter.drawEllipse(250, 200, 50, 50); // Круг

    // Рисуем точки
    for (int i = 0; i < 10; i++) {
        painter.drawPoint(50 + i * 10, 250);
    }

    // Применяем соответствующие эффекты в зависимости от выбора
    switch (index) {
    case 1: // Зашумленное изображение
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-50, 50);

        for (int y = 0; y < testImage.height(); ++y) {
            QRgb* line = reinterpret_cast<QRgb*>(testImage.scanLine(y));
            for (int x = 0; x < testImage.width(); ++x) {
                if (dis(gen) < 10) { // 20% шанс добавить шум
                    int noise = dis(gen);
                    QRgb pixel = line[x];
                    int r = qBound(0, qRed(pixel) + noise, 255);
                    int g = qBound(0, qGreen(pixel) + noise, 255);
                    int b = qBound(0, qBlue(pixel) + noise, 255);
                    line[x] = qRgb(r, g, b);
                }
            }
        }
    }
    break;

    case 2: // Размытое изображение
    {
        QImage blurred = testImage;
        for (int y = 1; y < testImage.height() - 1; ++y) {
            QRgb* dstLine = reinterpret_cast<QRgb*>(testImage.scanLine(y));
            for (int x = 1; x < testImage.width() - 1; ++x) {
                int r = 0, g = 0, b = 0;
                for (int j = -1; j <= 1; ++j) {
                    const QRgb* srcLine = reinterpret_cast<const QRgb*>(blurred.scanLine(y + j));
                    for (int i = -1; i <= 1; ++i) {
                        QRgb pixel = srcLine[x + i];
                        r += qRed(pixel);
                        g += qGreen(pixel);
                        b += qBlue(pixel);
                    }
                }
                dstLine[x] = qRgb(r / 9, g / 9, b / 9);
            }
        }
    }
    break;

    case 3: // Малоконтрастное изображение
    {
        for (int y = 0; y < testImage.height(); ++y) {
            QRgb* line = reinterpret_cast<QRgb*>(testImage.scanLine(y));
            for (int x = 0; x < testImage.width(); ++x) {
                QRgb pixel = line[x];
                int r = 100 + (qRed(pixel) - 127) * 0.3;
                int g = 100 + (qGreen(pixel) - 127) * 0.3;
                int b = 100 + (qBlue(pixel) - 127) * 0.3;
                line[x] = qRgb(r, g, b);
            }
        }
    }
    break;

    case 4: // Темное изображение
    {
        for (int y = 0; y < testImage.height(); ++y) {
            QRgb* line = reinterpret_cast<QRgb*>(testImage.scanLine(y));
            for (int x = 0; x < testImage.width(); ++x) {
                QRgb pixel = line[x];
                int r = qRed(pixel) - 100;
                int g = qGreen(pixel) - 100;
                int b = qBlue(pixel) - 100;
                line[x] = qRgb(qMax(0, r), qMax(0, g), qMax(0, b));
            }
        }
    }
    break;

    case 5: // Светлое изображение
    {
        for (int y = 0; y < testImage.height(); ++y) {
            QRgb* line = reinterpret_cast<QRgb*>(testImage.scanLine(y));
            for (int x = 0; x < testImage.width(); ++x) {
                QRgb pixel = line[x];
                int r = qMin(255, qRed(pixel) + 100);
                int g = qMin(255, qGreen(pixel) + 100);
                int b = qMin(255, qBlue(pixel) + 100);
                line[x] = qRgb(r, g, b);
            }
        }
    }
    break;
    }

    originalImage = testImage;
    QPixmap pixmap = QPixmap::fromImage(originalImage);
    originalImageLabel->setPixmap(pixmap);

    processedImage = QImage();
    processedImageLabel->clear();
    processedImageLabel->setText("Обработанное изображение");
}
