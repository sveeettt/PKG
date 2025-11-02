/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *Size;
    QLabel *Contraction;
    QLabel *Color_depth;
    QLabel *Resolution;
    QLabel *FileName;
    QListView *listView;
    QLabel *label_fileName;
    QLabel *label_size;
    QLabel *label_contraction;
    QLabel *label_4;
    QLabel *label_resolution;
    QLineEdit *lineEdit;
    QPushButton *multiChoice;
    QPushButton *selectFolderButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(700, 500);
        Size = new QLabel(Widget);
        Size->setObjectName("Size");
        Size->setGeometry(QRect(490, 150, 200, 20));
        Contraction = new QLabel(Widget);
        Contraction->setObjectName("Contraction");
        Contraction->setGeometry(QRect(490, 190, 200, 20));
        Color_depth = new QLabel(Widget);
        Color_depth->setObjectName("Color_depth");
        Color_depth->setGeometry(QRect(490, 230, 200, 20));
        Resolution = new QLabel(Widget);
        Resolution->setObjectName("Resolution");
        Resolution->setGeometry(QRect(490, 270, 200, 20));
        FileName = new QLabel(Widget);
        FileName->setObjectName("FileName");
        FileName->setGeometry(QRect(490, 110, 200, 20));
        listView = new QListView(Widget);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(20, 20, 301, 321));
        label_fileName = new QLabel(Widget);
        label_fileName->setObjectName("label_fileName");
        label_fileName->setGeometry(QRect(370, 110, 100, 20));
        label_size = new QLabel(Widget);
        label_size->setObjectName("label_size");
        label_size->setGeometry(QRect(370, 150, 100, 20));
        label_contraction = new QLabel(Widget);
        label_contraction->setObjectName("label_contraction");
        label_contraction->setGeometry(QRect(370, 190, 100, 20));
        label_4 = new QLabel(Widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(370, 230, 100, 20));
        label_resolution = new QLabel(Widget);
        label_resolution->setObjectName("label_resolution");
        label_resolution->setGeometry(QRect(370, 270, 100, 20));
        lineEdit = new QLineEdit(Widget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(20, 350, 301, 41));
        multiChoice = new QPushButton(Widget);
        multiChoice->setObjectName("multiChoice");
        multiChoice->setGeometry(QRect(20, 400, 301, 41));
        selectFolderButton = new QPushButton(Widget);
        selectFolderButton->setObjectName("selectFolderButton");
        selectFolderButton->setGeometry(QRect(20, 450, 301, 41));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        Size->setText(QString());
        Contraction->setText(QString());
        Color_depth->setText(QString());
        Resolution->setText(QString());
        FileName->setText(QString());
        label_fileName->setText(QCoreApplication::translate("Widget", "\320\230\320\274\321\217 \321\204\320\260\320\271\320\273\320\260:", nullptr));
        label_size->setText(QCoreApplication::translate("Widget", "\320\240\320\260\320\267\320\274\320\265\321\200:", nullptr));
        label_contraction->setText(QCoreApplication::translate("Widget", "C\320\266\320\260\321\202\320\270\320\265:", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "\320\223\320\273\321\203\320\261\320\270\320\275\320\260 \321\206\320\262\320\265\321\202\320\260:", nullptr));
        label_resolution->setText(QCoreApplication::translate("Widget", "\320\240\320\260\320\267\321\200\320\265\321\210\320\265\320\275\320\270\320\265:", nullptr));
        lineEdit->setText(QCoreApplication::translate("Widget", "true", nullptr));
        multiChoice->setText(QCoreApplication::translate("Widget", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \321\204\320\260\320\271\320\273\321\213", nullptr));
        selectFolderButton->setText(QCoreApplication::translate("Widget", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \320\277\320\260\320\277\320\272\321\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
