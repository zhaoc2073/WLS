#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "DoseEvaluator.h"
#include "SettingDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setParameterDialog();
    void setParameter();
    void evaluate();

private:
    DoseEvaluator evaluator;
    SettingDialog * settingDialog;
    QLineEdit * U238Edit;
    QLineEdit * Ra226Edit;
    QLineEdit * Th232Edit;
    QLineEdit * K40Edit;
    QLineEdit * massEdit;
    QLineEdit * disEdit;
    QLineEdit * percentEdit;
};

#endif // MAINWINDOW_H
