#include "mainwindow.h"
#include "ColorTransformer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isUpdating(false)
{
    setupUI();
    connectSignals();

    isUpdating = true;
    hueSpin->setValue(0);
    saturationSpin->setValue(0);
    valueSpin->setValue(50);
    isUpdating = false;
    updateFromHSV();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);


    colorDisplay = new QLabel(this);
    colorDisplay->setFrameStyle(QFrame::Box);
    colorDisplay->setMinimumHeight(100);
    colorDisplay->setStyleSheet("background-color: #808080;");
    mainLayout->addWidget(colorDisplay);


    paletteButton = new QPushButton("Выбрать из палитры", this);
    mainLayout->addWidget(paletteButton);


    QGroupBox *hsvGroup = new QGroupBox("HSV Model", this);
    QFormLayout *hsvLayout = new QFormLayout(hsvGroup);

    hueSpin = new QDoubleSpinBox(this);
    hueSpin->setRange(0, 360);
    hueSpin->setDecimals(1);
    saturationSpin = new QDoubleSpinBox(this);
    saturationSpin->setRange(0, 100);
    saturationSpin->setDecimals(1);
    valueSpin = new QDoubleSpinBox(this);
    valueSpin->setRange(0, 100);
    valueSpin->setDecimals(1);

    hsvLayout->addRow("H (0-360°):", hueSpin);
    hsvLayout->addRow("S (0-100%):", saturationSpin);
    hsvLayout->addRow("V (0-100%):", valueSpin);


    QHBoxLayout *hsvSliderLayout = new QHBoxLayout();
    hueSlider = new QSlider(Qt::Horizontal, this);
    hueSlider->setRange(0, 3600);
    saturationSlider = new QSlider(Qt::Horizontal, this);
    saturationSlider->setRange(0, 1000);
    valueSlider = new QSlider(Qt::Horizontal, this);
    valueSlider->setRange(0, 1000);

    hsvSliderLayout->addWidget(new QLabel("H:", this));
    hsvSliderLayout->addWidget(hueSlider);
    hsvSliderLayout->addWidget(new QLabel("S:", this));
    hsvSliderLayout->addWidget(saturationSlider);
    hsvSliderLayout->addWidget(new QLabel("V:", this));
    hsvSliderLayout->addWidget(valueSlider);

    hsvLayout->addRow(hsvSliderLayout);


    QGroupBox *xyzGroup = new QGroupBox("XYZ Model", this);
    QFormLayout *xyzLayout = new QFormLayout(xyzGroup);

    xSpin = new QDoubleSpinBox(this);
    xSpin->setRange(0, 100);
    xSpin->setDecimals(3);
    ySpin = new QDoubleSpinBox(this);
    ySpin->setRange(0, 100);
    ySpin->setDecimals(3);
    zSpin = new QDoubleSpinBox(this);
    zSpin->setRange(0, 100);
    zSpin->setDecimals(3);

    xyzLayout->addRow("X (0-100):", xSpin);
    xyzLayout->addRow("Y (0-100):", ySpin);
    xyzLayout->addRow("Z (0-100):", zSpin);


    QHBoxLayout *xyzSliderLayout = new QHBoxLayout();
    xSlider = new QSlider(Qt::Horizontal, this);
    xSlider->setRange(0, 100000);
    ySlider = new QSlider(Qt::Horizontal, this);
    ySlider->setRange(0, 100000);
    zSlider = new QSlider(Qt::Horizontal, this);
    zSlider->setRange(0, 100000);

    xyzSliderLayout->addWidget(new QLabel("X:", this));
    xyzSliderLayout->addWidget(xSlider);
    xyzSliderLayout->addWidget(new QLabel("Y:", this));
    xyzSliderLayout->addWidget(ySlider);
    xyzSliderLayout->addWidget(new QLabel("Z:", this));
    xyzSliderLayout->addWidget(zSlider);

    xyzLayout->addRow(xyzSliderLayout);

    QGroupBox *labGroup = new QGroupBox("LAB Model", this);
    QFormLayout *labLayout = new QFormLayout(labGroup);

    lSpin = new QDoubleSpinBox(this);
    lSpin->setRange(0, 100);
    lSpin->setDecimals(1);
    aSpin = new QDoubleSpinBox(this);
    aSpin->setRange(-128, 128);
    aSpin->setDecimals(1);
    bSpin = new QDoubleSpinBox(this);
    bSpin->setRange(-128, 128);
    bSpin->setDecimals(1);

    labLayout->addRow("L* (0-100):", lSpin);
    labLayout->addRow("a* (-128-128):", aSpin);
    labLayout->addRow("b* (-128-128):", bSpin);


    QHBoxLayout *labSliderLayout = new QHBoxLayout();
    lSlider = new QSlider(Qt::Horizontal, this);
    lSlider->setRange(0, 1000);
    aSlider = new QSlider(Qt::Horizontal, this);
    aSlider->setRange(-1280, 1280);
    bSlider = new QSlider(Qt::Horizontal, this);
    bSlider->setRange(-1280, 1280);

    labSliderLayout->addWidget(new QLabel("L:", this));
    labSliderLayout->addWidget(lSlider);
    labSliderLayout->addWidget(new QLabel("a:", this));
    labSliderLayout->addWidget(aSlider);
    labSliderLayout->addWidget(new QLabel("b:", this));
    labSliderLayout->addWidget(bSlider);

    labLayout->addRow(labSliderLayout);

    QHBoxLayout *modelsLayout = new QHBoxLayout();
    modelsLayout->addWidget(hsvGroup);
    modelsLayout->addWidget(xyzGroup);
    modelsLayout->addWidget(labGroup);

    mainLayout->addLayout(modelsLayout);
}

void MainWindow::connectSignals()
{

    connect(hueSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromHSV);
    connect(saturationSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromHSV);
    connect(valueSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromHSV);

    connect(hueSlider, &QSlider::valueChanged, this, &MainWindow::updateFromHSVSliders);
    connect(saturationSlider, &QSlider::valueChanged, this, &MainWindow::updateFromHSVSliders);
    connect(valueSlider, &QSlider::valueChanged, this, &MainWindow::updateFromHSVSliders);


    connect(xSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromXYZ);
    connect(ySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromXYZ);
    connect(zSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromXYZ);

    connect(xSlider, &QSlider::valueChanged, this, &MainWindow::updateFromXYZSliders);
    connect(ySlider, &QSlider::valueChanged, this, &MainWindow::updateFromXYZSliders);
    connect(zSlider, &QSlider::valueChanged, this, &MainWindow::updateFromXYZSliders);


    connect(lSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromLAB);
    connect(aSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromLAB);
    connect(bSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFromLAB);

    connect(lSlider, &QSlider::valueChanged, this, &MainWindow::updateFromLABSliders);
    connect(aSlider, &QSlider::valueChanged, this, &MainWindow::updateFromLABSliders);
    connect(bSlider, &QSlider::valueChanged, this, &MainWindow::updateFromLABSliders);

    connect(paletteButton, &QPushButton::clicked, this, &MainWindow::openColorPalette);
}

void MainWindow::updateFromHSVSliders()
{
    if (isUpdating) return;
    isUpdating = true;
    hueSpin->setValue(hueSlider->value() / 10.0);
    saturationSpin->setValue(saturationSlider->value() / 10.0);
    valueSpin->setValue(valueSlider->value() / 10.0);
    isUpdating = false;
    updateFromHSV();
}

void MainWindow::updateFromXYZSliders()
{
    if (isUpdating) return;
    isUpdating = true;
    xSpin->setValue(xSlider->value() / 1000.0);
    ySpin->setValue(ySlider->value() / 1000.0);
    zSpin->setValue(zSlider->value() / 1000.0);
    isUpdating = false;
    updateFromXYZ();
}

void MainWindow::updateFromLABSliders()
{
    if (isUpdating) return;
    isUpdating = true;
    lSpin->setValue(lSlider->value() / 10.0);
    aSpin->setValue(aSlider->value() / 10.0);
    bSpin->setValue(bSlider->value() / 10.0);
    isUpdating = false;
    updateFromLAB();
}

void MainWindow::updateAllFromHSV(double h, double s, double v)
{
    double x, y, z, l, a, b_val;

    ColorTransformer::hsvToXyz(h, s, v, x, y, z);
    ColorTransformer::xyzToLab(x, y, z, l, a, b_val);

    isUpdating = true;
    xSpin->setValue(x);
    ySpin->setValue(y);
    zSpin->setValue(z);
    lSpin->setValue(l);
    aSpin->setValue(a);
    bSpin->setValue(b_val);

    hueSlider->setValue(static_cast<int>(h * 10));
    saturationSlider->setValue(static_cast<int>(s * 10));
    valueSlider->setValue(static_cast<int>(v * 10));
    xSlider->setValue(static_cast<int>(x * 1000));
    ySlider->setValue(static_cast<int>(y * 1000));
    zSlider->setValue(static_cast<int>(z * 1000));
    lSlider->setValue(static_cast<int>(l * 10));
    aSlider->setValue(static_cast<int>(a * 10));
    bSlider->setValue(static_cast<int>(b_val * 10));
    isUpdating = false;

    updateColorDisplay(h, s, v);
}

void MainWindow::updateAllFromXYZ(double x, double y, double z)
{
    double h, s, v, l, a, b_val;

    ColorTransformer::xyzToHsv(x, y, z, h, s, v);
    ColorTransformer::xyzToLab(x, y, z, l, a, b_val);

    isUpdating = true;
    hueSpin->setValue(h);
    saturationSpin->setValue(s);
    valueSpin->setValue(v);
    lSpin->setValue(l);
    aSpin->setValue(a);
    bSpin->setValue(b_val);

    hueSlider->setValue(static_cast<int>(h * 10));
    saturationSlider->setValue(static_cast<int>(s * 10));
    valueSlider->setValue(static_cast<int>(v * 10));
    xSlider->setValue(static_cast<int>(x * 1000));
    ySlider->setValue(static_cast<int>(y * 1000));
    zSlider->setValue(static_cast<int>(z * 1000));
    lSlider->setValue(static_cast<int>(l * 10));
    aSlider->setValue(static_cast<int>(a * 10));
    bSlider->setValue(static_cast<int>(b_val * 10));
    isUpdating = false;

    updateColorDisplay(h, s, v);
}

void MainWindow::updateAllFromLAB(double l, double a, double b_val)
{
    double x, y, z, h, s, v;

    ColorTransformer::labToXyz(l, a, b_val, x, y, z);
    ColorTransformer::xyzToHsv(x, y, z, h, s, v);

    isUpdating = true;
    hueSpin->setValue(h);
    saturationSpin->setValue(s);
    valueSpin->setValue(v);
    xSpin->setValue(x);
    ySpin->setValue(y);
    zSpin->setValue(z);

    hueSlider->setValue(static_cast<int>(h * 10));
    saturationSlider->setValue(static_cast<int>(s * 10));
    valueSlider->setValue(static_cast<int>(v * 10));
    xSlider->setValue(static_cast<int>(x * 1000));
    ySlider->setValue(static_cast<int>(y * 1000));
    zSlider->setValue(static_cast<int>(z * 1000));
    lSlider->setValue(static_cast<int>(l * 10));
    aSlider->setValue(static_cast<int>(a * 10));
    bSlider->setValue(static_cast<int>(b_val * 10));
    isUpdating = false;

    updateColorDisplay(h, s, v);
}

void MainWindow::updateFromHSV()
{
    if (isUpdating) return;
    updateAllFromHSV(hueSpin->value(), saturationSpin->value(), valueSpin->value());
}

void MainWindow::updateFromXYZ()
{
    if (isUpdating) return;
    updateAllFromXYZ(xSpin->value(), ySpin->value(), zSpin->value());
}

void MainWindow::updateFromLAB()
{
    if (isUpdating) return;
    updateAllFromLAB(lSpin->value(), aSpin->value(), bSpin->value());
}

void MainWindow::openColorPalette()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color.isValid()) {
        isUpdating = true;
        hueSpin->setValue(color.hsvHueF() * 360);
        saturationSpin->setValue(color.hsvSaturationF() * 100);
        valueSpin->setValue(color.valueF() * 100);
        isUpdating = false;
        updateFromHSV();
    }
}

void MainWindow::updateColorDisplay(double h, double s, double v)
{
    QColor color;
    color.setHsv(static_cast<int>(h), static_cast<int>(s * 2.55), static_cast<int>(v * 2.55));

    QString style = QString("background-color: %1; border: 2px solid black;")
                        .arg(color.name());
    colorDisplay->setStyleSheet(style);
}

void MainWindow::showWarning(const QString &message)
{
    QMessageBox::warning(this, "Предупреждение", message);
}
