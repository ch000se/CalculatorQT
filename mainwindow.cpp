#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calculator.h"
#include <QRegularExpression>
#include <QPushButton>

Calculator calcVal = 0.0;
Calculator memory = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool clearTrigger = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal.toDouble()));

    for (int var = 0; var < 10; ++var) {
        QString butName = "Button" + QString::number(var);
        QPushButton *numButton = MainWindow::findChild<QPushButton *>(butName);
        connect(numButton, &QPushButton::released, this, &MainWindow::NumPressed);
    }

    connect(ui->Add, &QPushButton::released, this, &MainWindow::MathButtonPressed);
    connect(ui->Subtract, &QPushButton::released, this, &MainWindow::MathButtonPressed);
    connect(ui->Multiply, &QPushButton::released, this, &MainWindow::MathButtonPressed);
    connect(ui->Divide, &QPushButton::released, this, &MainWindow::MathButtonPressed);
    connect(ui->Equals, &QPushButton::released, this, &MainWindow::EqualButtonPressed);
    connect(ui->ChangeSign, &QPushButton::released, this, &MainWindow::ChangeNumberSign);
    connect(ui->Clear, &QPushButton::released, this, &MainWindow::ClearDisplay);
    connect(ui->MemAdd, &QPushButton::released, this, &MainWindow::MemAddPressed);
    connect(ui->MemClear, &QPushButton::released, this, &MainWindow::MemClearPressed);
    connect(ui->MemGet, &QPushButton::released, this, &MainWindow::MemGetPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::NumPressed(){
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString butVal = button->text();
    QString displayVal = ui->Display->text();

    if (clearTrigger) {
        ui->Display->setText(butVal);
        clearTrigger = false;
    } else {
        if (displayVal.toDouble() == 0.0) {
            ui->Display->setText(butVal);
        } else {
            QString newVal = displayVal + butVal;
            double dblNewVal = newVal.toDouble();
            ui->Display->setText(QString::number(dblNewVal));
        }
    }
}

void MainWindow::MathButtonPressed(){
    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    QString butVal = button->text();

    if (butVal == "/"){
        divTrigger = true;
    } else if (butVal == "*"){
        multTrigger = true;
    } else if (butVal == "+"){
        addTrigger = true;
    } else {
        subTrigger = true;
    }

    ui->Display->setText("");
}

void MainWindow::EqualButtonPressed(){
    Calculator solution = 0.0;
    QString displayVal = ui->Display->text();
    Calculator dblDisplayVal = displayVal.toDouble();
    if (addTrigger){
        solution = calcVal + dblDisplayVal;
    } else if (subTrigger){
        solution = calcVal - dblDisplayVal;
    } else if (multTrigger){
        solution = calcVal * dblDisplayVal;
    }  else if (divTrigger){
        if (dblDisplayVal.toDouble() != 0.0) {
            solution = calcVal / dblDisplayVal;
    } else{
            ui->Display->setText("Error");
            clearTrigger = true;
            return;
        }
    }
    ui->Display->setText(QString::number(solution.toDouble()));
    clearTrigger = true;
}

void MainWindow::ChangeNumberSign() {
    QString displayVal = ui->Display->text();
    QRegularExpression reg("[-+]?[0-9.]*");
    QRegularExpressionMatch match = reg.match(displayVal);
    if (match.hasMatch()) {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -dblDisplayVal;
        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}

void MainWindow::ClearDisplay() {
    ui->Display->setText("");
    clearTrigger = false;
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
}

void MainWindow::MemAddPressed() {
    QString displayVal = ui->Display->text();
    Calculator dblDisplayVal = displayVal.toDouble();
    memory = memory + dblDisplayVal;
}

void MainWindow::MemClearPressed() {
    memory = 0.0;
}

void MainWindow::MemGetPressed() {
    ui->Display->setText(QString::number(memory.toDouble()));
    clearTrigger = true;
}
