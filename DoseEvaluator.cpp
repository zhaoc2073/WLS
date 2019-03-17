#include "DoseEvaluator.h"

DoseEvaluator::DoseEvaluator()
{
    SA_U238 = SA_Ra226 = SA_Th232 = SA_K40 = 0;
    DC_U238 = DC_Ra226 = DC_Th232 = DC_K40 = 0;
    mass = 0;
    dis = 0;
    exposurePercent = 0;
}

double DoseEvaluator::evaluate()
{
    //doseRate = SUM(SkinEquDoseRateConstants*SpecificActivity*Mass)/d^2
    //SkinEquDoseRateConstants: 皮肤剂量率常数(简写为DC_XXX)，单位：uSv m^2 / (MBq h)
    //SpecificActivity: 样品比活度(简写为SA_XXX)，单位：Bq/kg
    //Mass: 样品质量，单位：g
    //d: 样品-皮肤距离，单位：cm
    //exposurePercent: 暴露时间占比，单位：%

    double disTemp = dis;
    if ( disTemp <= 1 ) disTemp = 1;        //放射源非常近的时候，平方反比不适用。

    double temp = mass/1000.0/(disTemp/100.0*disTemp/100.0);    //Unit: kg/m^2
    doseRate = temp * (SA_U238 * DC_U238 +
                      SA_Ra226 * DC_Ra226 +
                      SA_Th232 * DC_Th232 +
                      SA_K40 * DC_K40);    //Unit: 1E-6 uSv/h

    doseRate = doseRate*1E-9*365*24*exposurePercent/100;    //Unit: mSv/a
    return doseRate;
}

QString DoseEvaluator::message()
{
    QString messageStr;
    if ( doseRate < 0.5 )
        messageStr = QString("该消费品正常使用条件下用户个人皮肤剂量当量约为%1，"
                             "远低于GB16353-1996规定限值(5mSv/a)，放射性危害较小。").arg(doseRate);
    else if ( doseRate < 5 )
        messageStr = QString("该消费品正常使用条件下用户个人皮肤剂量当量约为%1，"
                             "低于GB16353-1996规定限值(5mSv/a)，有一定放射性危害，建议继续评价其至人体有效剂量当量。").arg(doseRate);
    else
        messageStr = QString("该消费品正常使用条件下用户个人皮肤剂量当量约为%1，"
                             "高于（或等于）GB16353-1996规定限值(5mSv/a)，极可能存在较大放射性危害，请管理部门严格监控样品！").arg(doseRate);
    return messageStr;
}
