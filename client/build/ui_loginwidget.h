/********************************************************************************
** Form generated from reading UI file 'loginwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIDGET_H
#define UI_LOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer_2;
    QLabel *usernameLabel;
    QLineEdit *password;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *username;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_3;
    QLabel *passwordLabel;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName(QStringLiteral("LoginWidget"));
        LoginWidget->resize(800, 600);
        LoginWidget->setLayoutDirection(Qt::LeftToRight);
        LoginWidget->setAutoFillBackground(false);
        gridLayout = new QGridLayout(LoginWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton = new QPushButton(LoginWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        pushButton->setFont(font);
        pushButton->setFocusPolicy(Qt::NoFocus);
        pushButton->setAutoDefault(false);
        pushButton->setDefault(false);
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 8, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 235, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 9, 2, 1, 1);

        usernameLabel = new QLabel(LoginWidget);
        usernameLabel->setObjectName(QStringLiteral("usernameLabel"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        QBrush brush1(QColor(159, 158, 158, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        usernameLabel->setPalette(palette);
        QFont font1;
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        usernameLabel->setFont(font1);

        gridLayout->addWidget(usernameLabel, 1, 2, 1, 1);

        password = new QLineEdit(LoginWidget);
        password->setObjectName(QStringLiteral("password"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(password->sizePolicy().hasHeightForWidth());
        password->setSizePolicy(sizePolicy);
        password->setMinimumSize(QSize(200, 0));
        password->setFocusPolicy(Qt::StrongFocus);

        gridLayout->addWidget(password, 5, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(300, 24, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 8, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 4, 4, 5, 1);

        username = new QLineEdit(LoginWidget);
        username->setObjectName(QStringLiteral("username"));
        sizePolicy.setHeightForWidth(username->sizePolicy().hasHeightForWidth());
        username->setSizePolicy(sizePolicy);
        username->setMinimumSize(QSize(200, 0));
        username->setFocusPolicy(Qt::StrongFocus);

        gridLayout->addWidget(username, 2, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 182, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 7, 2, 1, 1);

        passwordLabel = new QLabel(LoginWidget);
        passwordLabel->setObjectName(QStringLiteral("passwordLabel"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        passwordLabel->setPalette(palette1);
        passwordLabel->setFont(font1);

        gridLayout->addWidget(passwordLabel, 4, 2, 1, 1);


        retranslateUi(LoginWidget);
        QObject::connect(password, SIGNAL(returnPressed()), pushButton, SLOT(click()));
        QObject::connect(username, SIGNAL(returnPressed()), pushButton, SLOT(click()));

        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        LoginWidget->setWindowTitle(QApplication::translate("LoginWidget", "To be Omnanmnad", 0));
        pushButton->setText(QApplication::translate("LoginWidget", "Login", 0));
        usernameLabel->setText(QApplication::translate("LoginWidget", "Username:", 0));
#ifndef QT_NO_TOOLTIP
        password->setToolTip(QApplication::translate("LoginWidget", "<html><head/><body><p><br/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        username->setToolTip(QApplication::translate("LoginWidget", "<html><head/><body><p><br/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        passwordLabel->setText(QApplication::translate("LoginWidget", "Password:", 0));
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
