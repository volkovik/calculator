#include "calculator.h"
#include "./ui_calculator.h"
#include <cmath>
#include <math.h>
#include <functional>

double calcValue = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool trigTrigger = false;
bool equalTrigger = false;

void resetMathTriggers() {
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
}

Calculator::Calculator(QWidget *parent): QMainWindow(parent), ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcValue));
        QPushButton *numButtons[10];
        for (int i = 0; i < 10; ++i) {
            QString butName = "Button" + QString::number(i);
            numButtons[i] = Calculator::findChild<QPushButton *>(butName);
            connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->Pi, SIGNAL(released()), this, SLOT(PiNumPressed()));
    connect(ui->Point, SIGNAL(released()), this, SLOT(PointButtonPressed()));

    connect(ui->Sin, SIGNAL(released()), this, SLOT(SinButtonPressed()));
    connect(ui->Cos, SIGNAL(released()), this, SLOT(CosButtonPressed()));
    connect(ui->Tan, SIGNAL(released()), this, SLOT(TanButtonPressed()));
    connect(ui->ChangeView, SIGNAL(released()), this, SLOT(ChangeViewButtonPressed()));

    connect(ui->Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Subtract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));

    connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualButtonPressed()));
    connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
    connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearDisplay()));
    connect(ui->Delete, SIGNAL(released()), this, SLOT(DeleteNumDisplay()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString butValue = button->text();
    QString displayValue = ui->Display->text();
    if ((displayValue.toDouble() == 0) || (displayValue.toDouble() == 0.0)) {
        ui->Display->setText(butValue);
        equalTrigger = false;
    } else if (equalTrigger) {
        ui->Display->setText(butValue);
        equalTrigger = false;
    } else {
        QString newValue = displayValue + butValue;
        double dblNewValue = newValue.toDouble();
        ui->Display->setText(QString::number(dblNewValue, 'g', 16));
    }
}

void Calculator::PiNumPressed() {
    ui->Display->setText("3.1415");
}

void Calculator::TrigButtonAction(std::function<double(double)> trig) {
    QPushButton *changeViewButton = ui->ChangeView;
    QString changeViewMode = changeViewButton->text();

    QString displayValue = ui->Display->text();
    calcValue = displayValue.toDouble();


    if (changeViewMode == "RAD") {
        calcValue = trig(calcValue);
    } else if (changeViewMode == "DEG") {
        calcValue = fmod(calcValue, 360);

        if (calcValue >= 180) {
            calcValue = calcValue = trig((calcValue - 180) * (M_PI / 180)) * -1;
        } else {
            calcValue = trig(calcValue * (M_PI / 180));
        }
    }
    equalTrigger = true;

    ui->Display->setText(QString::number(calcValue));
}

void Calculator::SinButtonPressed() {
    TrigButtonAction([](double x) -> double { return sin(x); });
}

void Calculator::CosButtonPressed() {
    TrigButtonAction([](double x) -> double { return cos(x); });
}

void Calculator::TanButtonPressed() {
    TrigButtonAction([](double x) -> double { return tan(x); });
}

void Calculator::ChangeViewButtonPressed() {
    QPushButton *button = (QPushButton *)sender();
    QString butValue = button->text();

    if (butValue == "RAD") {
        button->setText("DEG");
    } else {
        button->setText("RAD");
    }
}

void Calculator::MathButtonPressed() {
    resetMathTriggers();

    QString displayValue = ui->Display->text();
    calcValue = displayValue.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butValue = button->text();

    if ((QString::compare(butValue, "/", Qt::CaseSensitive) == 0)) {
        divTrigger = true;
    } else if ((QString::compare(butValue, "*", Qt::CaseSensitive) == 0)) {
        multTrigger = true;
    } else if ((QString::compare(butValue, "+", Qt::CaseSensitive) == 0)) {
        addTrigger = true;
    } else {
        subTrigger = true;
    }

    ui->Display->setText("");
}

void Calculator::PointButtonPressed() {
    QString displayValue = ui->Display->text();
    if (!displayValue.contains('.') && displayValue.size() >= 1) {
        ui->Display->setText(displayValue + '.');
    }
}

void Calculator::EqualButtonPressed() {
    double solution = 0.0;
    QString displayValue = ui->Display->text();
    double dblDisplayValue = displayValue.toDouble();

    if (divTrigger || multTrigger || addTrigger || subTrigger) {
        if (addTrigger) {
            solution = calcValue + dblDisplayValue;
        } else if (subTrigger) {
            solution = calcValue - dblDisplayValue;
        } else if (multTrigger) {
            solution = calcValue * dblDisplayValue;
        } else if (divTrigger) {
            solution = calcValue / dblDisplayValue;
        }
        equalTrigger = true;
    } else {
        solution = dblDisplayValue;
    }

    resetMathTriggers();

    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign() {
    QString displayValue = ui->Display->text();
    QRegExp reg("[-]?[0-9.]*");
    if (reg.exactMatch(displayValue)) {
        double dblDisplayValue = displayValue.toDouble();
        double dblDisplayValueSign = -1 * dblDisplayValue;
        ui->Display->setText(QString::number(dblDisplayValueSign));
    }
}


void Calculator::ClearDisplay() {
    calcValue = 0.0;
    resetMathTriggers();
    equalTrigger = false;

    ui->Display->setText("0");
}

void Calculator::DeleteNumDisplay() {
    QString displayValue =  ui->Display->text();
    if (displayValue.size() == 1) {
        ui->Display->setText("0");
    } else {
        ui->Display->setText(displayValue.chopped(1));
    }
}
