/********************************************************************************
** Form generated from reading UI file 'screenwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREENWIDGET_H
#define UI_SCREENWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenWidget
{
public:

    void setupUi(QWidget *ScreenWidget)
    {
        if (ScreenWidget->objectName().isEmpty())
            ScreenWidget->setObjectName(QStringLiteral("ScreenWidget"));
        ScreenWidget->resize(803, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenWidget->sizePolicy().hasHeightForWidth());
        ScreenWidget->setSizePolicy(sizePolicy);
        ScreenWidget->setMinimumSize(QSize(400, 300));
        ScreenWidget->setFocusPolicy(Qt::StrongFocus);

        retranslateUi(ScreenWidget);

        QMetaObject::connectSlotsByName(ScreenWidget);
    } // setupUi

    void retranslateUi(QWidget *ScreenWidget)
    {
        ScreenWidget->setWindowTitle(QApplication::translate("ScreenWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class ScreenWidget: public Ui_ScreenWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREENWIDGET_H
