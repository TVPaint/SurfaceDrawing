/********************************************************************************
** Form generated from reading UI file 'ConnectionDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTIONDIALOG_H
#define UI_CONNECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConnectionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QFormLayout *formLayout;
    QLabel *iPLabel;
    QLineEdit *iPLineEdit;
    QLabel *portLabel;
    QSpinBox *portSpinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConnectionDialog)
    {
        if (ConnectionDialog->objectName().isEmpty())
            ConnectionDialog->setObjectName(QStringLiteral("ConnectionDialog"));
        ConnectionDialog->resize(253, 115);
        verticalLayout = new QVBoxLayout(ConnectionDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ConnectionDialog);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        iPLabel = new QLabel(ConnectionDialog);
        iPLabel->setObjectName(QStringLiteral("iPLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, iPLabel);

        iPLineEdit = new QLineEdit(ConnectionDialog);
        iPLineEdit->setObjectName(QStringLiteral("iPLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, iPLineEdit);

        portLabel = new QLabel(ConnectionDialog);
        portLabel->setObjectName(QStringLiteral("portLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, portLabel);

        portSpinBox = new QSpinBox(ConnectionDialog);
        portSpinBox->setObjectName(QStringLiteral("portSpinBox"));
        portSpinBox->setMaximum(99999);

        formLayout->setWidget(1, QFormLayout::FieldRole, portSpinBox);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(ConnectionDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ConnectionDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectionDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectionDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConnectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectionDialog)
    {
        ConnectionDialog->setWindowTitle(QApplication::translate("ConnectionDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("ConnectionDialog", "Connect to server", nullptr));
        iPLabel->setText(QApplication::translate("ConnectionDialog", "IP", nullptr));
        portLabel->setText(QApplication::translate("ConnectionDialog", "Port", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectionDialog: public Ui_ConnectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTIONDIALOG_H
