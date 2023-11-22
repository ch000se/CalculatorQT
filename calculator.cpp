#include "Calculator.h"
#include <QTextStream>
#include <QStringList>
#include <cstdlib>
#include <QRegularExpression>

void Calculator::print() const
{
    QTextStream(stdout) << num;
}

Calculator Calculator::operator+(const Calculator& v) const
{
    if (isNegative() && v.isNegative()) {
        return -(std::abs(num.toInt()) + std::abs(v.num.toInt()));
    }
    else if (isNegative()) {
        return v - std::abs(num.toInt());
    }
    else if (v.isNegative()) {
        return *this - std::abs(v.num.toInt());
    }
    else {
        QString answer;
        int maxlen = qMax(size, v.size);
        int carry = 0;

        for (int i = 0; i < maxlen; i++) {
            int a1 = (i >= size) ? 0 : num[size - i - 1].digitValue();
            int a2 = (i >= v.size) ? 0 : v.num[v.size - i - 1].digitValue();

            int sum = a1 + a2 + carry;

            if (sum >= 10) {
                sum -= 10;
                carry = 1;
            }
            else {
                carry = 0;
            }

            answer = QString::number(sum) + answer;
        }

        if (carry > 0) {
            answer = QString::number(carry) + answer;
        }

        return Calculator(answer);
    }
}

Calculator Calculator::operator-(const Calculator& v) const
{
    if (isNegative() && v.isNegative()) {
        return std::abs(v.num.toInt()) - std::abs(num.toInt());
    }
    else if (isNegative()) {
        return -(std::abs(v.num.toInt()) + std::abs(num.toInt()));
    }
    else if (v.isNegative()) {
        return *this + std::abs(v.num.toInt());
    }
    else {
        QString answer;
        int maxlen = qMax(size, v.size);
        int borrow = 0;

        bool swapNeeded = false;

        if (size < v.size || (size == v.size && num < v.num)) {
            swapNeeded = true;
        }

        for (int i = 0; i < maxlen; i++) {
            int a1 = (i >= size) ? 0 : num[size - i - 1].digitValue();
            int a2 = (i >= v.size) ? 0 : v.num[v.size - i - 1].digitValue();

            if (swapNeeded) {
                qSwap(a1, a2);
            }

            int diff = a1 - a2 - borrow;

            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else {
                borrow = 0;
            }

            answer = QString::number(diff) + answer;
        }

        while (!answer.isEmpty() && answer.at(0) == '0') {
            answer.remove(0, 1);
        }

        if (answer.isEmpty()) {
            answer = "0";
        }

        if (swapNeeded) {
            answer = "-" + answer;
        }

        return Calculator(answer);
    }
}

Calculator Calculator::operator*(const Calculator& v) const
{
    QString answer;
    int maxlen = size + v.size;
    QVector<int> result(maxlen, 0);

    for (int i = size - 1; i >= 0; i--)
    {
        int carry = 0;
        int a1 = num[i].digitValue();

        for (int j = v.size - 1; j >= 0; j--)
        {
            int a2 = v.num[j].digitValue();
            int product = a1 * a2 + result[i + j + 1] + carry;

            carry = product / 10;
            result[i + j + 1] = product % 10;
        }

        result[i] += carry;
    }

    for (int digit : result)
    {
        answer += QString::number(digit);
    }

    while (!answer.isEmpty() && answer.at(0) == '0')
    {
        answer.remove(0, 1);
    }

    if (answer.isEmpty())
    {
        answer = "0";
    }

    return Calculator(answer);
}

Calculator Calculator::operator/(const Calculator& v) const
{
    if (v.num == "0")
    {
        QTextStream(stderr) << "Ділення на 0";
        return Calculator("0");
    }

    QString quotient;
    QString currentDividend = "";
    int currentIndex = 0;

    while (currentIndex < size)
    {
        currentDividend += num[currentIndex];
        int count = 0;
        Calculator partialDividend(currentDividend);

        while (partialDividend >= v)
        {
            partialDividend = partialDividend - v;
            count++;
        }

        quotient += QString::number(count);
        currentDividend = partialDividend.num;
        currentIndex++;
    }

    while (!quotient.isEmpty() && quotient.at(0) == '0')
    {
        quotient.remove(0, 1);
    }

    if (quotient.isEmpty())
    {
        quotient = "0";
    }

    return Calculator(quotient);
}


bool Calculator::operator>=(const Calculator& v) const {
    int numValue = num.toInt();
    int vNumValue = v.num.toInt();
    return numValue >= vNumValue;
}


QTextStream& operator>>(QTextStream& s, Calculator& v)
{
    QString input;
    s >> input;

    v.num = input;
    v.size = input.length();

    return s;
}

bool Calculator::isNegative() const
{
    return !num.isEmpty() && num[0] == '-';
}

QString Calculator::getQString() const
{
    return num;
}

double Calculator::toDouble() const {
    return num.toDouble();
}
