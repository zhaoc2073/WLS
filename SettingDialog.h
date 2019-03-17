#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "DoseEvaluator.h"
class QLineEdit;

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);
    bool accepted() const {return bAccepted;}
    void pullParameter(const DoseEvaluator & evaluator);
    void pushParameter(DoseEvaluator & evaluator) const;

    
signals:
    void hided();
    
public slots:
    void acceptSetting();
    void rejectSetting();

private:
    bool bAccepted;
    QLineEdit * U238Edit, * Ra226Edit, * Th232Edit, * K40Edit;
};

#endif // SETTINGDIALOG_H
