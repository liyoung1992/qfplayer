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
#include <QtWidgets/QWidget>
#include "video/QFVideoWidget.h"

QT_BEGIN_NAMESPACE

class Ui_qfplayerClass
{
public:
    QFVideoWidget *video;

    void setupUi(QWidget *qfplayerClass)
    {
        if (qfplayerClass->objectName().isEmpty())
            qfplayerClass->setObjectName(QString::fromUtf8("qfplayerClass"));
        qfplayerClass->resize(965, 621);
        video = new QFVideoWidget(qfplayerClass);
        video->setObjectName(QString::fromUtf8("video"));
        video->setGeometry(QRect(10, 20, 921, 561));

        retranslateUi(qfplayerClass);

        QMetaObject::connectSlotsByName(qfplayerClass);
    } // setupUi

    void retranslateUi(QWidget *qfplayerClass)
    {
        qfplayerClass->setWindowTitle(QApplication::translate("qfplayerClass", "qfplayer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qfplayerClass: public Ui_qfplayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QFPLAYER_H
