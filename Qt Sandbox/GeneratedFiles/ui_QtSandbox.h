/********************************************************************************
** Form generated from reading UI file 'QtSandbox.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSANDBOX_H
#define UI_QTSANDBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtSandboxClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtSandboxClass)
    {
        if (QtSandboxClass->objectName().isEmpty())
            QtSandboxClass->setObjectName(QStringLiteral("QtSandboxClass"));
        QtSandboxClass->resize(600, 400);
        menuBar = new QMenuBar(QtSandboxClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QtSandboxClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtSandboxClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtSandboxClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtSandboxClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtSandboxClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtSandboxClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtSandboxClass->setStatusBar(statusBar);

        retranslateUi(QtSandboxClass);

        QMetaObject::connectSlotsByName(QtSandboxClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtSandboxClass)
    {
        QtSandboxClass->setWindowTitle(QApplication::translate("QtSandboxClass", "QtSandbox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtSandboxClass: public Ui_QtSandboxClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSANDBOX_H
