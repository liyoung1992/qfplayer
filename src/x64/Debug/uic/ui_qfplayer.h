/********************************************************************************
** Form generated from reading UI file 'qfplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QFPLAYER_H
#define UI_QFPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "video/QFVideoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_qfplayerClass
{
public:
    QFVideoWidget *video;
    QPushButton *openFile;

    void setupUi(QWidget *qfplayerClass)
    {
        if (qfplayerClass->objectName().isEmpty())
            qfplayerClass->setObjectName(QString::fromUtf8("qfplayerClass"));
        qfplayerClass->resize(936, 647);
        video = new QFVideoWidget(qfplayerClass);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(-1, -1, 941, 651));
        openFile = new QPushButton(qfplayerClass);
        openFile->setObjectName(QString::fromUtf8("openFile"));
        openFile->setGeometry(QRect(10, 610, 75, 23));

        retranslateUi(qfplayerClass);

        QMetaObject::connectSlotsByName(qfplayerClass);
    } // setupUi

    void retranslateUi(QWidget *qfplayerClass)
    {
        qfplayerClass->setWindowTitle(QApplication::translate("qfplayerClass", "qfplayer", nullptr));
        openFile->setText(QApplication::translate("qfplayerClass", "\346\211\223\345\274\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qfplayerClass: public Ui_qfplayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QFPLAYER_H
