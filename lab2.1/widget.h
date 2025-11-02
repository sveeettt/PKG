#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileSystemModel>
#include <QImageWriter>
#include <QTableWidget>
#include <QFileDialog>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_multiChoice_clicked();
    void on_selectFolderButton_clicked();
    void on_lineEdit_returnPressed();

private:
    Ui::Widget *ui;
    QFileSystemModel *model;

    QString getAdditionalInfo(const QString &format, const QString &filePath);

    void displayImageInfo(const QModelIndex &index, const QFileInfo &fileInfo);
    void setTableHeaders(QTableWidget *table);
    bool populateTableRow(const QString &filePath, int rowIndex, QTableWidget *table); // <<< добавьте это
    bool isValidImageExtension(const QString &extension);
    void showImageDialog(const QString &title, const QStringList &files);
};

#endif // WIDGET_H
