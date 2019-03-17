#ifndef DOSEEVALUATOR_H
#define DOSEEVALUATOR_H

#include <QString>

class DoseEvaluator
{
public:
    DoseEvaluator();
    double evaluate();
    QString message();
    double SA_U238, SA_Ra226, SA_Th232, SA_K40;
    double DC_U238, DC_Ra226, DC_Th232, DC_K40;
    double mass, dis, exposurePercent;
    double doseRate;

private:

};

#endif // DOSEEVALUATOR_H
