#include "MainWindow.h"
#include <QToolBar>
#include <QAction>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QPushButton>
#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), settingDialog(0)
{
    ///////////////////////////////////////////////////////////////////////////
    //UI
    setWindowTitle("含放射性物质消费品风险评估软件");
    QToolBar *toolBar = addToolBar(tr("设置(&s)"));
    QAction * set = toolBar->addAction("设置(&s)");

    QGroupBox * nuclideGroup = new QGroupBox("放射性核素比活度(Bq/kg)");
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

    QGridLayout * nuclideLayout = new QGridLayout;
    //nuclideLayout->setHorizontalSpacing(20);
    //nuclideLayout->setVerticalSpacing(20);
    nuclideLayout->addWidget(U238Label,0,1,1,1);
    nuclideLayout->addWidget(Ra226Label,1,1,1,1);
    nuclideLayout->addWidget(Th232Label,2,1,1,1);
    nuclideLayout->addWidget(K40Label,3,1,1,1);
    nuclideLayout->addWidget(U238Edit,0,2,1,1);
    nuclideLayout->addWidget(Ra226Edit,1,2,1,1);
    nuclideLayout->addWidget(Th232Edit,2,2,1,1);
    nuclideLayout->addWidget(K40Edit,3,2,1,1);
    nuclideGroup->setLayout(nuclideLayout);

    QGroupBox * conditionGroup = new QGroupBox("暴露条件");
    QLabel * massLabel      = new QLabel("消费品重量(g)");
    QLabel * disLabel       = new QLabel("消费品-体表距离(cm)");
    QLabel * percentLabel  = new QLabel("暴露时间占比(%)");

    massEdit    = new QLineEdit;
    disEdit     = new QLineEdit;
    percentEdit = new QLineEdit;

    massEdit->setValidator(validator);
    disEdit->setValidator(validator);

    QDoubleValidator * validator2 = new QDoubleValidator(this);
    validator2->setRange(0, 100, 0);
    percentEdit->setValidator(validator2);

    QGridLayout * conditionLayout = new QGridLayout;
    //conditionLayout->setHorizontalSpacing(20);
    //conditionLayout->setVerticalSpacing(20);
    conditionLayout->addWidget(massLabel,0,1,1,1);
    conditionLayout->addWidget(disLabel,1,1,1,1);
    conditionLayout->addWidget(percentLabel,2,1,1,1);
    conditionLayout->addWidget(massEdit,0,2,1,1);
    conditionLayout->addWidget(disEdit,1,2,1,1);
    conditionLayout->addWidget(percentEdit,2,2,1,1);
    conditionGroup->setLayout(conditionLayout);

    QHBoxLayout * inputLayout = new QHBoxLayout;
    inputLayout->addWidget(nuclideGroup);
    inputLayout->addWidget(conditionGroup);

    QPushButton * evaluateButton = new QPushButton("辐射风险评估");
    evaluateButton->setFixedSize(120,40);

    QHBoxLayout * butttonLayout = new QHBoxLayout;
    butttonLayout->addStretch();
    butttonLayout->addWidget(evaluateButton);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(butttonLayout);

    QWidget * centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);

    this->setCentralWidget(centralWidget);
    ///////////////////////////////////////////////////////////////////////////

    QSettings settings(QString("config.ini"), QSettings::IniFormat);
    evaluator.DC_U238 = settings.value("SkinEquDoseRateConstants/U238", "0").toDouble();
    evaluator.DC_Ra226 = settings.value("SkinEquDoseRateConstants/Ra226", "0").toDouble();
    evaluator.DC_Th232 = settings.value("SkinEquDoseRateConstants/Th232", "0").toDouble();
    evaluator.DC_K40 = settings.value("SkinEquDoseRateConstants/K40", "0").toDouble();

    connect(set, SIGNAL(triggered()), this, SLOT(setParameterDialog()));
    connect(evaluateButton, SIGNAL(clicked()), this, SLOT(evaluate()));
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::setParameterDialog()
{
    if ( settingDialog == 0 ) {
        settingDialog = new SettingDialog(this);
        connect(settingDialog, SIGNAL(hided()), this, SLOT(setParameter()));
        settingDialog->pullParameter(evaluator);
    }
    settingDialog->show();
}

void MainWindow::setParameter()
{
    if ( settingDialog->accepted() ) {
        settingDialog->pushParameter(evaluator);
    } else {
        settingDialog->pullParameter(evaluator);
    }
}

void MainWindow::evaluate()
{
    evaluator.SA_U238 = U238Edit->text().toDouble();
    evaluator.SA_Ra226 = Ra226Edit->text().toDouble();
    evaluator.SA_Th232 = Th232Edit->text().toDouble();
    evaluator.SA_K40 = K40Edit->text().toDouble();
    evaluator.mass = massEdit->text().toDouble();
    evaluator.dis = disEdit->text().toDouble();
    evaluator.exposurePercent = percentEdit->text().toDouble();

    if ( evaluator.DC_U238 <= 0 ||
         evaluator.DC_Ra226 <= 0 ||
         evaluator.DC_Th232 <= 0 ||
         evaluator.DC_K40 <= 0
         )
    {
        QMessageBox::information(NULL, tr("错误！"),
                                 QString("皮肤剂量率常数应大于零！\n请重新设置！"));
        setParameterDialog();
        return;
    }

    if ( evaluator.SA_U238 < 0 ||
         evaluator.SA_Ra226 < 0 ||
         evaluator.SA_Th232 < 0 ||
         evaluator.SA_K40 < 0
         )
    {
        QMessageBox::information(NULL, tr("错误！"),
                                 QString("放射性核素比活度不应小于零！\n请重新设置！"));
        return;
    }

    if ( evaluator.mass <= 0 ) {
        QMessageBox::information(NULL, tr("错误！"),
                                 QString("消费品重量不应小于零！\n请重新设置！"));
        return;
    }

    if ( evaluator.dis < 0 ) {
        QMessageBox::information(NULL, tr("错误！"),
                                 QString("消费品-体表距离不应小于零！\n请重新设置！"));
        return;
    }

    if ( evaluator.exposurePercent < 0 ||
         evaluator.exposurePercent > 100 ) {
        QMessageBox::information(NULL, tr("错误！"),
                                 QString("暴露时间占比应在0~100范围内！\n请重新设置！"));
        return;
    }

    evaluator.evaluate();
    QString message = evaluator.message();
    QMessageBox::information(NULL, tr("辐射风险评估"),
                             message);

}

