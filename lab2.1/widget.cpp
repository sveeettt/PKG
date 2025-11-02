#include "widget.h"
#include "ui_widget.h"
#include <QElapsedTimer>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QLinearGradient>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollBar>
#include <QDir>

// Минималистичный делегат для списка файлов
class MinimalListDelegate : public QStyledItemDelegate {
public:
    explicit MinimalListDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        painter->setRenderHint(QPainter::Antialiasing);

        // Фон
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor(59, 130, 246, 50));
            painter->setPen(QPen(QColor(59, 130, 246), 1));
            painter->drawRect(option.rect.adjusted(0, 0, -1, -1));
        } else if (option.state & QStyle::State_MouseOver) {
            painter->fillRect(option.rect, QColor(243, 244, 246));
        }

        // Текст
        painter->setPen(QColor(31, 41, 55));
        QRect textRect = option.rect.adjusted(12, 8, -12, -8);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        size.setHeight(36);
        return size;
    }
};

// Минималистичный делегат для таблицы
class MinimalTableDelegate : public QStyledItemDelegate {
public:
    explicit MinimalTableDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        painter->setRenderHint(QPainter::Antialiasing);

        // Чередующийся фон строк
        if (index.row() % 2 == 0) {
            painter->fillRect(option.rect, QColor(249, 250, 251));
        }

        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, QColor(59, 130, 246, 30));
        }

        // Текст
        painter->setPen(QColor(31, 41, 55));
        QRect textRect = option.rect.adjusted(8, 4, -8, -4);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    }
};

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->gridLayout_2->setColumnMinimumWidth(0, 130);
    ui->gridLayout_2->setColumnStretch(1, 1);




    // Устанавливаем минималистичные стили
    this->setStyleSheet(R"(
        QWidget {
            background-color: #ffffff;
            font-family: 'Segoe UI', system-ui, sans-serif;
            color: #1f2937;
        }

        QLineEdit {
            border: 1px solid #d1d5db;
            border-radius: 8px;
            padding: 12px 16px;
            font-size: 14px;
            background: #f9fafb;
            selection-background-color: #3b82f6;
        }

        QLineEdit:focus {
            border-color: #3b82f6;
            background: #ffffff;
        }

        QPushButton {
            border: none;
            border-radius: 8px;
            padding: 12px 20px;
            font-size: 14px;
            font-weight: 500;
            background: #3b82f6;
            color: white;
            min-height: 44px;
        }

        QPushButton:hover {
            background: #2563eb;
        }

        QPushButton:pressed {
            background: #1d4ed8;
        }

        QLabel {
            color: #1f2937;
            font-size: 14px;
        }

        QGroupBox {
            border: 1px solid #e5e7eb;
            border-radius: 12px;
            margin-top: 20px;
            padding-top: 10px;
            background: #fafbfc;
            font-weight: 600;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 4px 12px;
            background: #3b82f6;
            color: white;
            border-radius: 6px;
        }
    )");

    // Настройка модели файловой системы
    model = new QFileSystemModel(this);
    model->setFilter(QDir::AllEntries | QDir::NoDot);
    model->setRootPath(QDir::rootPath());

    // Применяем минималистичный делегат
    ui->listView->setItemDelegate(new MinimalListDelegate(this));
    ui->listView->setModel(model);
    ui->listView->setRootIndex(model->index(QDir::homePath()));

    // Настраиваем отображение
    ui->listView->setIconSize(QSize(24, 24));

    // Устанавливаем начальный путь
    ui->lineEdit->setText(QDir::homePath());
    ui->lineEdit->setPlaceholderText("Введите путь или нажмите Enter...");

    // Настраиваем информацию о файле
    ui->FileName->setText("-");
    ui->Size->setText("-");
    ui->Resolution->setText("-");
    ui->Color_depth->setText("-");
    ui->Contraction->setText("-");
}

Widget::~Widget() {
    delete ui;
}

//обработка двойного клика
void Widget::on_listView_doubleClicked(const QModelIndex &index) {
    QFileInfo fileInfo = model->fileInfo(index);
    ui->lineEdit->setText(model->filePath(index));

    if (fileInfo.fileName() == "..") {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        ui->listView->setRootIndex(model->index(dir.absolutePath()));
        ui->lineEdit->setText(dir.absolutePath());
    } else if (fileInfo.fileName() == ".") {
        ui->listView->setRootIndex(model->index(QDir::rootPath()));
        ui->lineEdit->setText(QDir::rootPath());
    } else if (fileInfo.isDir()) {
        ui->listView->setRootIndex(index);
        ui->lineEdit->setText(model->filePath(index));
    } else {
        displayImageInfo(index, fileInfo);
    }
}

void Widget::on_lineEdit_returnPressed() {
    QString path = ui->lineEdit->text();
    QDir dir(path);

    if (dir.exists()) {
        ui->listView->setRootIndex(model->index(path));
    } else {
        ui->lineEdit->setText(model->filePath(ui->listView->rootIndex()));
    }
}

//информация об изображении
void Widget::displayImageInfo(const QModelIndex &index, const QFileInfo &fileInfo) {
    QString extension = fileInfo.suffix().toLower();

    if (isValidImageExtension(extension)) {
        QImage img(model->filePath(index));
        QImageWriter writer(model->filePath(index));

        if (!img.isNull()) {
            ui->FileName->setText(fileInfo.fileName());
            ui->Size->setText(QString("%1 × %2 px").arg(img.width()).arg(img.height()));
            ui->Resolution->setText(QString("%1 DPI").arg(img.dotsPerMeterX() * 0.0254));
            ui->Color_depth->setText(QString("%1 bit").arg(img.bitPlaneCount()));

            // Добавляем дополнительную информацию
            QString additionalInfo = getAdditionalInfo(extension.toUpper(), model->filePath(index));
            ui->Contraction->setText(additionalInfo.isEmpty() ?
                                         QString::number(writer.compression()) : additionalInfo);
        }
    }
}

void Widget::on_multiChoice_clicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          "Выберите изображения",
                                                          QDir::homePath(),
                                                          "Images (*.jpg *.jpeg *.gif *.tif *.bmp *.png *.pcx);;All Files (*)");

    if (!fileNames.isEmpty()) {
        showImageDialog("Выбранные изображения", fileNames);
    }
}

void Widget::on_selectFolderButton_clicked() {
    QString directory = QFileDialog::getExistingDirectory(this,
                                                          "Выберите папку с изображениями",
                                                          QDir::homePath());

    if (!directory.isEmpty()) {
        QDir dir(directory);
        QStringList filters = {"*.jpg", "*.jpeg", "*.gif", "*.tif", "*.bmp", "*.png", "*.pcx"};
        QStringList files = dir.entryList(filters, QDir::Files);

        QStringList fullPaths;
        for (const QString &file : files) {
            fullPaths.append(dir.filePath(file));
        }

        showImageDialog(QString("Папка: %1").arg(directory), fullPaths);
    }
}

void Widget::showImageDialog(const QString &title, const QStringList &files) {
    QElapsedTimer timer;
    timer.start();

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Информация об изображениях");
    dialog->setMinimumSize(1000, 600);
    dialog->setStyleSheet(R"(
        QDialog {
            background: #ffffff;
            font-family: 'Segoe UI', system-ui, sans-serif;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    // Заголовок
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: 600;
            color: #1f2937;
            padding: 16px;
            background: #f8fafc;
            border-radius: 8px;
            margin: 8px;
        }
    )");
    titleLabel->setWordWrap(true);
    layout->addWidget(titleLabel);

    // Таблица
    QTableWidget *table = new QTableWidget();
    table->setColumnCount(6);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setItemDelegate(new MinimalTableDelegate(this));

    // Стилизация таблицы
    table->setStyleSheet(R"(
        QTableWidget {
            border: 1px solid #e5e7eb;
            border-radius: 8px;
            background: #ffffff;
            gridline-color: #f3f4f6;
        }

        QHeaderView::section {
            background: #3b82f6;
            color: white;
            padding: 12px 8px;
            border: none;
            font-weight: 600;
        }

        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #f3f4f6;
        }
    )");

    setTableHeaders(table);
    table->setRowCount(files.size());

    int validCount = 0;
    for (int i = 0; i < files.size(); i++) {
        if (populateTableRow(files[i], validCount, table)) {
            validCount++;
        }
    }

    table->setRowCount(validCount);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (validCount > 0) {
        layout->addWidget(table);

        // Статистика
        qint64 elapsed = timer.elapsed();
        QLabel *statsLabel = new QLabel(QString("Обработано %1 изображений за %2 мс").arg(validCount).arg(elapsed));
        statsLabel->setStyleSheet("padding: 12px; color: #6b7280; font-size: 13px;");
        statsLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(statsLabel);
    } else {
        QLabel *noFilesLabel = new QLabel("Не найдено поддерживаемых изображений");
        noFilesLabel->setAlignment(Qt::AlignCenter);
        noFilesLabel->setStyleSheet("font-size: 14px; color: #9ca3af; padding: 40px;");
        layout->addWidget(noFilesLabel);
    }

    // Кнопка закрытия
    QPushButton *closeButton = new QPushButton("Закрыть");
    closeButton->setStyleSheet(R"(
        QPushButton {
            background: #6b7280;
            margin: 8px;
        }
        QPushButton:hover {
            background: #4b5563;
        }
    )");

    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog->exec();
    delete dialog;
}

void Widget::setTableHeaders(QTableWidget *table) {
    QStringList headers = {"Имя файла", "Размер", "Разрешение", "Глубина цвета", "Сжатие", "Формат"};
    table->setHorizontalHeaderLabels(headers);
}

bool Widget::populateTableRow(const QString &filePath, int rowIndex, QTableWidget *table) {
    QFileInfo fileInfo(filePath);
    QString extension = fileInfo.suffix().toLower();

    if (!isValidImageExtension(extension)) {
        return false;
    }

    QImage img(filePath);
    if (img.isNull()) {
        return false;
    }

    QImageWriter writer(filePath);
    QString additionalInfo = getAdditionalInfo(extension.toUpper(), filePath);

    table->setItem(rowIndex, 0, new QTableWidgetItem(fileInfo.fileName()));
    table->setItem(rowIndex, 1, new QTableWidgetItem(QString("%1 × %2").arg(img.width()).arg(img.height())));
    table->setItem(rowIndex, 2, new QTableWidgetItem(QString("%1 DPI").arg(qRound(img.dotsPerMeterX() * 0.0254))));
    table->setItem(rowIndex, 3, new QTableWidgetItem(QString("%1 bit").arg(img.bitPlaneCount())));

    // Используем дополнительную информацию или стандартное сжатие
    table->setItem(rowIndex, 4, new QTableWidgetItem(
                                    additionalInfo.isEmpty() ?
                                        (img.hasAlphaChannel() ? "Alpha" : QString::number(writer.compression()))
                                                             : additionalInfo
                                    ));

    table->setItem(rowIndex, 5, new QTableWidgetItem(extension.toUpper()));

    return true;
}

bool Widget::isValidImageExtension(const QString &extension) {
    return extension == "jpg" || extension == "jpeg" || extension == "gif" ||
           extension == "tif" || extension == "bmp" || extension == "png" ||
           extension == "pcx";
}

QString Widget::getAdditionalInfo(const QString &format, const QString &filePath)
{
    QString additional;

    if (format == "JPG" || format == "JPEG") {
        additional = "Цветовое пространство: YCbCr";
    } else if (format == "PNG") {
        QImage image(filePath);
        if (!image.isNull()) {
            additional = QString("Каналов: %1").arg(image.isGrayscale() ? "1" : "3-4");
        }
    } else if (format == "GIF") {
        additional = "Палитровое изображение (256 цветов)";
    } else if (format == "BMP") {
        additional = "Прямое кодирование цветов";
    } else if (format == "TIF" || format == "TIFF") {
        additional = "Поддержка слоев и тегов";
    } else if (format == "PCX") {
        additional = "RLE-сжатие";
    }

    return additional;
}
