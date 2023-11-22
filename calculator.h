#pragma once
#include <QString>
#include <QTextStream>

class Calculator
{
private:
    static const int MAX_SIZE = 1000;
    QString num;
    int size;

public:

    Calculator() : num(""), size(0){}
    Calculator(const QString& _num) : num (_num), size(_num.length()) {}
    Calculator(int value)
    {
        num = QString::number(value);
        size = num.length();
    }


    void print() const;
    QString getQString() const;
    double toDouble() const;
    Calculator operator+ (const Calculator&) const;
    Calculator operator- (const Calculator&) const;
    Calculator operator* (const Calculator&) const;
    Calculator operator/ (const Calculator&) const;
    friend QTextStream& operator>>(QTextStream& s, Calculator& v);
    bool operator>=(const Calculator&) const;
    bool isNegative() const;
    bool isaNegative() const;
    void removeSign();
};
