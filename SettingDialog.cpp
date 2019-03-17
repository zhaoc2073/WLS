#include "SettingDialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QDebug>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent), bAccepted(true)
{
    this->setWindowTitle("参数设置");

    QLabel * TipLabel  = new QLabel("皮肤剂量率常数：uSv m^2 / (MBq h)");
    QLabel * U238Label  = new QLabel("U-238");
    QLabel * Ra226Label = new QLabel("Ra-226");
    QLabel * Th232Label = new QLabel("Th-232");
    QLabel * K40Label   = new QLabel("K-40");

    U238Edit    = new QLineEdit;
    Ra226Edit   = new QLineEdit;
    Th232Edit   = new QLineEdit;
    K40Edit     = new QLineEdit;

    QDoubleValidator * validator = new QDoubleValidator(this);
    validator->setBottom(0);
    U238Edit->setValidator(validator);
    Ra226Edit->setValidator(validator);
    Th232Edit->setValidator(validator);
    K40Edit->setValidator(validator);

    QGridLayout * parameterLayout = new QGridLayout;
    parameterLayout->addWidget(TipLabel,0,1,1,2);
    parameterLayout->addWidget(U238Label,1,1,1,1);
    parameterLayout->addWidget(Ra226Label,2,1,1,1);
    parameterLayout->addWidget(Th232Label,3,1,1,1);
    parameterLayout->addWidget(K40Label,4,1,1,1);
    parameterLayout->addWidget(U238Edit,1,2,1,1);
    parameterLayout->addWidget(Ra226Edit,2,2,1,1);
    parameterLayout->addWidget(Th232Edit,3,2,1,1);
    parameterLayout->addWidget(K40Edit,4,2,1,1);

    QPushButton * acceptButton = new QPushButton("确定");
    QPushButton * rejectButton = new QPushButton("取消");
    QHBoxLayout * buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(rejectButton);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(parameterLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptSetting()));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(rejectSetting()));
}

void SettingDialog::acceptSetting()
{
    bAccepted = true;
    hide();
    emit hided();
}

void SettingDialog::rejectSetting()
{
    bAccepted = false;
    hide();
    emit hided();
}

void SettingDialog::pullParameter(const DoseEvaluator & evaluator)
{
    U238Edit->setText(QString::number(evaluator.DC_U238));
    Ra226Edit->setText(QString::number(evaluator.DC_Ra226));
    Th232Edit->setText(QString::number(evaluator.DC_Th232));
    K40Edit->setText(QString::number(evaluator.DC_K40));
}

void SettingDialog::pushParameter(DoseEvaluator & evaluator) const
{
    evaluator.DC_U238 = U238Edit->text().toDouble();
    evaluator.DC_Ra226 = Ra226Edit->text().toDouble();
    evaluator.DC_Th232 = Th232Edit->text().toDouble();
    evaluator.DC_K40 = K40Edit->text().toDouble();

    QSettings settings(QString("config.ini"), QSettings::IniFormat);
    settings.setValue("SkinEquDoseRateConstants/U238",evaluator.DC_U238);
    settings.setValue("SkinEquDoseRateConstants/Ra226",evaluator.DC_Ra226);
    settings.setValue("SkinEquDoseRateConstants/Th232",evaluator.DC_Th232);
    settings.setValue("SkinEquDoseRateConstants/K40",evaluator.DC_K40);
}
