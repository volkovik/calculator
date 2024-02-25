#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <functional>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;

private slots:
    void NumPressed();
    void PiNumPressed();
    void TrigButtonAction(std::function<double(double)> trig);
    void SinButtonPressed();
    void CosButtonPressed();
    void TanButtonPressed();
    void ChangeViewButtonPressed();
    void MathButtonPressed();
    void PointButtonPressed();
    void EqualButtonPressed();
    void ChangeNumberSign();
    void ClearDisplay();
    void DeleteNumDisplay();
};
#endif // CALCULATOR_H
