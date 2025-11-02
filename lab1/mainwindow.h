#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QGroupBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFromHSV();
    void updateFromXYZ();
    void updateFromLAB();
    void openColorPalette();
    void updateFromHSVSliders();
    void updateFromXYZSliders();
    void updateFromLABSliders();

private:

    QLabel *colorDisplay;//виджет для отображения текущего цвета

    // HSV
    QDoubleSpinBox *hueSpin; //ввод оттенка(0-360)
    QDoubleSpinBox *saturationSpin;//ввод насыщенности(0-100)
    QDoubleSpinBox *valueSpin;//ввод яркости
    QSlider *hueSlider;
    QSlider *saturationSlider;
    QSlider *valueSlider;

    // XYZ
    QDoubleSpinBox *xSpin;
    QDoubleSpinBox *ySpin;
    QDoubleSpinBox *zSpin;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    // LAB
    QDoubleSpinBox *lSpin;//ввод яркости (0-100)
    QDoubleSpinBox *aSpin;//зелено-красная ось
    QDoubleSpinBox *bSpin;//сине-желтая
    QSlider *lSlider;
    QSlider *aSlider;
    QSlider *bSlider;

    QPushButton *paletteButton;

    bool isUpdating;

    void setupUI();//инициализация пользов интерфейса
    void connectSignals();
    void updateAllFromHSV(double h, double s, double v);
    void updateAllFromXYZ(double x, double y, double z);
    void updateAllFromLAB(double l, double a, double b);
    void updateColorDisplay(double h, double s, double v);
    void showWarning(const QString &message);//предупреждение
};

#endif // MAINWINDOW_H
