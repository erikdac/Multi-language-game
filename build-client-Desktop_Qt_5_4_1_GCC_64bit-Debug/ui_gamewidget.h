/********************************************************************************
** Form generated from reading UI file 'gamewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWIDGET_H
#define UI_GAMEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ui/playerwidget.h"
#include "ui/screenwidget.h"
#include "ui/targetwidget.h"

QT_BEGIN_NAMESPACE

class Ui_GameWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *Bar;
    QHBoxLayout *horizontalLayout;
    PlayerWidget *playerwidget;
    TargetWidget *targetwidget;
    QGridLayout *screenlayout;
    ScreenWidget *screenwidget;

    void setupUi(QWidget *GameWidget)
    {
        if (GameWidget->objectName().isEmpty())
            GameWidget->setObjectName(QStringLiteral("GameWidget"));
        GameWidget->resize(600, 400);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GameWidget->sizePolicy().hasHeightForWidth());
        GameWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(GameWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Bar = new QWidget(GameWidget);
        Bar->setObjectName(QStringLiteral("Bar"));
        Bar->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Bar->sizePolicy().hasHeightForWidth());
        Bar->setSizePolicy(sizePolicy1);
        Bar->setMinimumSize(QSize(400, 75));
        Bar->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout = new QHBoxLayout(Bar);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(8, 8, 8, 8);
        playerwidget = new PlayerWidget(Bar);
        playerwidget->setObjectName(QStringLiteral("playerwidget"));

        horizontalLayout->addWidget(playerwidget);

        targetwidget = new TargetWidget(Bar);
        targetwidget->setObjectName(QStringLiteral("targetwidget"));

        horizontalLayout->addWidget(targetwidget);


        verticalLayout->addWidget(Bar);

        screenlayout = new QGridLayout();
        screenlayout->setSpacing(0);
        screenlayout->setObjectName(QStringLiteral("screenlayout"));
        screenwidget = new ScreenWidget(GameWidget);
        screenwidget->setObjectName(QStringLiteral("screenwidget"));

        screenlayout->addWidget(screenwidget, 0, 0, 1, 1);


        verticalLayout->addLayout(screenlayout);


        retranslateUi(GameWidget);

        QMetaObject::connectSlotsByName(GameWidget);
    } // setupUi

    void retranslateUi(QWidget *GameWidget)
    {
        GameWidget->setWindowTitle(QApplication::translate("GameWidget", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class GameWidget: public Ui_GameWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWIDGET_H
