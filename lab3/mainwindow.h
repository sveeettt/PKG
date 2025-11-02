#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <vector>
#include <cmath>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void saveImage();
    void processImage();
    void resetImage();
    void updateThreshold(int value);
    void updateKernelSize(int value);
    void updateSigma(double value);
    void onTestImageSelected(int index);

private:
    void setupUI();
    void createTestImages();
    QImage applyGlobalThresholdOtsu(const QImage& input);
    QImage applyGlobalThresholdIterative(const QImage& input);
    QImage applyAdaptiveThreshold(const QImage& input, int blockSize);
    QImage detectEdgesSobel(const QImage& input);
    QImage detectLinesHough(const QImage& input);
    QImage detectPoints(const QImage& input);
    QImage convertToGrayscale(const QImage& input);
    QImage addNoise(const QImage& input);
    QImage addBlur(const QImage& input);
    QImage adjustContrast(const QImage& input, double factor);
    QImage adjustBrightness(const QImage& input, int value);
    std::vector<std::vector<double>> createGaussianKernel(int size, double sigma);
    double calculateLocalMean(const QImage& image, int x, int y, int blockSize);

    // UI элементы
    QLabel *originalImageLabel;
    QLabel *processedImageLabel;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QPushButton *processButton;
    QPushButton *resetButton;
    QComboBox *methodComboBox;
    QComboBox *testImagesComboBox;
    QSpinBox *thresholdSpinBox;
    QSlider *thresholdSlider;
    QSpinBox *kernelSizeSpinBox;
    QDoubleSpinBox *sigmaSpinBox;
    QCheckBox *grayscaleCheckBox;

    QImage originalImage;
    QImage processedImage;
};

#endif // MAINWINDOW_H
