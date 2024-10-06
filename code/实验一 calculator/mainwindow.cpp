#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <string>
#include <stack>
#include <QDebug>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
        /*
        ui->setupUi(this);
        ui->lineEdit->setText("0");

        result = 0.0;
        waitForOperand = true;

        connectSlots();
        */
        ui->setupUi(this);
        ui->lineEdit->setText("0");

        result = 0.0;
//        initUi();
        waitForOperand = true;
        connectSlots();
        setShortcutKeys();
    }


MainWindow::~MainWindow()
{
    delete ui;
}

//bool MainWindow::calculate(double operand, QString pendingOperator)
//{
//    if(pendingOperator == "+")
//    {
//        result += operand;
//    }
//    else if(pendingOperator == "-")
//    {
//        result -= operand;
//    }
//    else if(pendingOperator == "*")
//    {
//        result *= operand;
//    }
//    else if(pendingOperator == "/")
//    {
//        if(operand == 0.0)
//            return false;
//        result /= operand;
//    }
//    return true;
//}


#include <iostream>
#include <cctype>
#include <cmath>
#include <sstream>

// 判断是否为运算符
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'||c=='@';
}

// 获取运算符的优先级
int precedence(char op) {
    switch (op) {
        case '+':
        case '-': return 2;
        case '*':
        case '/': return 4;
        case '^': return 6;
        case '@': return 8;
        default: return 0;
    }
}

// 判断是否为函数
bool isFunction(const std::string& token) {
    return token == "sin" || token == "cos" || token == "tan" || token == "ln" || token == "sqrt"||token == "arcsin";
}

// 将中缀表达式转换为后缀表达式
std::string infixToPostfix(const std::string& infix) {
    std::stack<std::string> stack;
    std::string postfix;
    std::string number;
    std::string func;

    for (size_t i = 0; i < infix.length(); ++i) {
        char token = infix[i];

        if (isspace(token)) {
            continue;  // 跳过空白符
        }

        if (isdigit(token) || token == '.') {
            number += token;  // 处理数字和小数点
        } else {
            if (!number.empty()) {
                postfix += number + ' ';
                number.clear();
            }

            if (isalpha(token)) {
                func += token;  // 处理函数名
                while (i + 1 < infix.length() && isalpha(infix[i + 1])) {
                    func += infix[++i];
                }
                stack.push(func);
                func.clear();
            } else if (token == '(') {
                stack.push(std::string(1, token));  // 左括号入栈
            } else if (token == ')') {
                while (!stack.empty() && stack.top() != "(") {
                    postfix += stack.top() + ' ';
                    stack.pop();
                }
                if (!stack.empty()) stack.pop();  // 弹出左括号
                if (!stack.empty() && isFunction(stack.top())) {
                    postfix += stack.top() + ' ';
                    stack.pop();
                }
            } else if (isOperator(token)) {
                if (token == '-' && (i == 0 || infix[i - 1] == '(' || isOperator(infix[i - 1]))) {
                    postfix += "0 ";  // 处理负号：前面补0
                    token='@';
//                    continue;
                }
                while (!stack.empty() && precedence(stack.top()[0]) >= precedence(token)) {
                    postfix += stack.top() + ' ';
                    stack.pop();
                }
                stack.push(std::string(1, token));
            }
        }
    }

    if (!number.empty()) {
        postfix += number + ' ';
    }

    while (!stack.empty()) {
        postfix += stack.top() + ' ';
        stack.pop();
    }

    return postfix;
}

// 计算后缀表达式
double evaluatePostfix(const std::string& postfix) {
    std::stack<double> evalStack;
    std::istringstream iss(postfix);
    std::string token;

    while (iss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
            double fl=1;
//            if(token[0]=='0') fl=-1;
            evalStack.push(std::stod(token)*fl);
        } else if (isOperator(token[0])) {
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            switch (token[0]) {
                case '+': evalStack.push(a + b); break;
                case '-': evalStack.push(a - b); break;
                case '@': evalStack.push(a - b); break;
                case '*': evalStack.push(a * b); break;
                case '/': evalStack.push(a / b); break;
                case '^': evalStack.push(std::pow(a, b)); break;  // 任意底数的指数运算
            }
        } else if (isFunction(token)) {
            double a = evalStack.top(); evalStack.pop();
            if (token == "sin") {
                evalStack.push(std::sin(a));
            } else if (token == "cos") {
                evalStack.push(std::cos(a));
            } else if (token == "tan") {
                evalStack.push(std::tan(a));
            } else if (token == "ln") {
                evalStack.push(std::log(a));
            } else if (token == "sqrt") {
                evalStack.push(std::sqrt(a));
            } else if (token == "arcsin") {
                evalStack.push(std::asin(a));
            }
        }
    }

    return evalStack.top();
}
double gt(std::string infix)
{
    std::string postfix = infixToPostfix(infix);
    QString demo = QString::fromStdString(postfix);
    qDebug() << demo << endl;
    double result = evaluatePostfix(postfix);
    return result;
}

void MainWindow::setShortcutKeys()
{

    Qt::Key key[21] = {
        Qt::Key_0,          Qt::Key_1,          Qt::Key_2,          Qt::Key_3,
        Qt::Key_4,          Qt::Key_5,          Qt::Key_6,          Qt::Key_7,
        Qt::Key_8,          Qt::Key_9,
        Qt::Key_Plus,       Qt::Key_Minus,      Qt::Key_Asterisk,   Qt::Key_Slash,
        Qt::Key_Enter,      Qt::Key_Period,     Qt::Key_Backspace,  Qt::Key_M,
        Qt::Key_ParenLeft,  Qt::Key_ParenRight, Qt::Key_AsciiCircum
    };
    QPushButton *btn[21] = {
            ui->pushButton0,      ui->pushButton1,          ui->pushButton2,          ui->pushButton3,
            ui->pushButton4,      ui->pushButton5,          ui->pushButton6,          ui->pushButton7,
            ui->pushButton8,      ui->pushButton9,
            ui->addBtn,         ui->subtractionBtn,     ui->mulBtn,  ui->divisionBtn,
            ui->equalBtn,       ui->pointBtn,           ui->clearBtn,           ui->signBtn,
            ui->leftBracketBtn, ui->rightBracketBtn,    ui->powBtn
        };
        for (int i = 0; i < 21; i++)
            btn[i]->setShortcut(QKeySequence(key[i]));
        ui->clearAllBtn->setShortcut(QKeySequence("Ctrl+Backspace"));
};


void MainWindow::abortOperation()
{
//    result = 0.0;
//    pendingOperator.clear();
//    ui->lineEdit->setText("0");
//    waitForOperand = true;
//    QMessageBox::warning(this, "运算错误", "除数不能为零");
    ui->lineEdit->setText("0");
    waitForOperand = true;
    QMessageBox::warning(this, "运算错误", error);
}

void MainWindow::connectSlots()
{
//   QPushButton *digitBtns[10] = {
//        ui->pushButton0,      ui->pushButton1,          ui->pushButton2,          ui->pushButton3,
//        ui->pushButton4,      ui->pushButton5,          ui->pushButton6,          ui->pushButton7,
//        ui->pushButton8,      ui->pushButton9
//    };
//    for (auto btn : digitBtns)
//        connect(btn, &QPushButton::clicked, this, &MainWindow::digitClicked);
//    QPushButton *operatorBtns[4] = {
//        ui->addBtn,         ui->subtractionBtn,     ui->mulBtn,  ui->divisionBtn,
//    };
//    for (auto btn : operatorBtns)
//        connect(btn, &QPushButton::clicked, this, &MainWindow::operatorClicked);
    QPushButton *digitBtns[10] = {
            ui->pushButton0,      ui->pushButton1,          ui->pushButton2,          ui->pushButton3,
            ui->pushButton4,      ui->pushButton5,          ui->pushButton6,          ui->pushButton7,
            ui->pushButton8,      ui->pushButton9
        };
        for (auto btn : digitBtns)
            connect(btn, &QPushButton::clicked, this, &MainWindow::digitClicked);
        QPushButton *operatorBtns[7] = {
            ui->addBtn,         ui->subtractionBtn,     ui->mulBtn,  ui->divisionBtn,
            ui->leftBracketBtn, ui->rightBracketBtn,    ui->powBtn
        };
        for (auto btn : operatorBtns)
            connect(btn, &QPushButton::clicked, this, &MainWindow::operatorClicked);
}

void MainWindow::digitClicked()
{
//    QPushButton *digitBtn = static_cast<QPushButton*>(sender());
//    QString value = digitBtn->text();
//    if(ui->lineEdit->text() == "0" && value == "0")
//        return;
//    if(waitForOperand)
//    {
//        ui->lineEdit->setText(value);
//        waitForOperand = false;
//    }
//    else
//    {
//        ui->lineEdit->setText(ui->lineEdit->text() + value);
//    }
    QPushButton *digitBtn = static_cast<QPushButton*>(sender());
    QString value = digitBtn->text();
    if(ui->lineEdit->text() == "0" && value == "0")
        return;
    if(waitForOperand)
    {
        ui->lineEdit->setText(value);
        waitForOperand = false;
    }
    else
    {
        ui->lineEdit->setText(ui->lineEdit->text() + value);
    }
}

void MainWindow::on_clearBtn_clicked()
{
//    //将当前显示的数归零
//    ui->lineEdit->setText("0");
//    waitForOperand = true;
    //去掉末尾字符
    QString str = ui->lineEdit->text();
    if (str != "0")
    {
        ui->lineEdit->setText(str.left(str.count() - 1));
    }
}

void MainWindow::on_clearAllBtn_clicked()
{
    //将当前显示的数据归零，并将之前保存的数据运算清除
    ui->lineEdit->setText("0");
    waitForOperand = true;
    result = 0.0;
    pendingOperator.clear();
}

double gt(char x)
{
    if(x=='+'||x=='-') return 1;
    if(x=='*'||x=='/') return 2;
    if(x=='(') return 0;
    if(x==')') return 10;
    if(x=='^');
}

string inToPost(string s)
{
    stack<char> num;
    stack<char> op;

    for(int i=0;i<(int)s.size();++i)
    {
        char x = s[i];
        if(x>='0'&&x<='9')
        {

            continue;
        }


    }
}

void MainWindow::on_equalBtn_clicked()
{
//    double operand = ui->lineEdit->text().toDouble();
//    if(pendingOperator.isEmpty())
//        return;
//    if(!calculate(operand, pendingOperator))
//    {
//        abortOperation();
//        return;
//    }
//    ui->lineEdit->setText(QString::number(result));
//    pendingOperator.clear();
//    result = 0.0;
//    waitForOperand = true;
    double result = 0.0;
    try
    {
        result = gt(ui->lineEdit->text().toStdString());
//        result = compute(inToPost(ui->lineEdit->text()));
//        result = 0;
    }
    catch(const char *er)
    {
        error = er;
        abortOperation();
        return;
    }
    ui->lineEdit->setText(ui->lineEdit->text() + '=' + QString::number(result));
    waitForOperand = true;
}
void MainWindow::on_signBtn_clicked()
{
//    QString text = ui->lineEdit->text();
//    double value = text.toDouble();
//    if(value > 0)
//    {
//        text.prepend('-');
//    }
//    else if(value < 0)
//    {
//        text.remove(0, 1);
//    }
//    ui->lineEdit->setText(text);
    QString text = ui->lineEdit->text();
    QChar sign = text[text.size() - 1];
    if(sign == '-')
    {
        text.remove(text.size() - 1, 1);
    }
    else
    {
        text.append('-');
    }
    ui->lineEdit->setText(text);
}

void MainWindow::on_pushButtonSin_clicked()
{
    if(waitForOperand)
    {
        ui->lineEdit->setText("");
        waitForOperand = false;
    }
    QString text = ui->lineEdit->text();
    ui->lineEdit->setText(ui->lineEdit->text() + "sin(");
}

void MainWindow::on_pushButtonCos_clicked()
{
    if(waitForOperand)
    {
        ui->lineEdit->setText("");
        waitForOperand = false;
    }
    QString text = ui->lineEdit->text();
    ui->lineEdit->setText(ui->lineEdit->text() + "cos(");
}

void MainWindow::on_pushButtonTan_clicked()
{
    if(waitForOperand)
    {
        ui->lineEdit->setText("");
        waitForOperand = false;
    }
    QString text = ui->lineEdit->text();
    ui->lineEdit->setText(ui->lineEdit->text() + "tan(");
}

void MainWindow::on_pushButtonLn_clicked()
{
    if(waitForOperand)
    {
        ui->lineEdit->setText("");
        waitForOperand = false;
    }
    QString text = ui->lineEdit->text();
    ui->lineEdit->setText(ui->lineEdit->text() + "ln(");
}

void MainWindow::on_pushButtonSqrt_clicked()
{
    if(waitForOperand)
    {
        ui->lineEdit->setText("");
        waitForOperand = false;
    }
    QString text = ui->lineEdit->text();
    ui->lineEdit->setText(ui->lineEdit->text() + "sqrt(");
}

void MainWindow::on_pushButtonArcsin_clicked()
{
    if(waitForOperand)
    {
        ui->lineEdit->setText("");
        waitForOperand = false;
    }
    QString text = ui->lineEdit->text();
    ui->lineEdit->setText(ui->lineEdit->text() + "arcsin(");
}

void MainWindow::operatorClicked()
{
//    QPushButton *clickedBtn = qobject_cast<QPushButton *>(sender());
//    QString clickedOperator = clickedBtn->text();
//    double operand = ui->lineEdit->text().toDouble();
//    if(!pendingOperator.isEmpty())
//    {
//        if(!calculate(operand, pendingOperator))
//        {
//            abortOperation();
//            return;
//        }
//        ui->lineEdit->setText(QString::number(result));
//    }
//    else
//    {
//        result = operand;
//    }
//    pendingOperator = clickedOperator;
//    waitForOperand = true;
    QPushButton *clickedBtn = qobject_cast<QPushButton *>(sender());
    QString clickedOperator = clickedBtn->text();
    ui->lineEdit->setText(ui->lineEdit->text() + clickedOperator);
}
void MainWindow::on_pointBtn_clicked()
{
//    if (waitForOperand)
//        ui->lineEdit->setText("0");
//    if (!ui->lineEdit->text().contains('.'))
//        ui->lineEdit->setText(ui->lineEdit->text() + ".");
//    waitForOperand = false;
    if (waitForOperand)
    ui->lineEdit->setText("0");
    ui->lineEdit->setText(ui->lineEdit->text() + ".");
    waitForOperand = false;
}




