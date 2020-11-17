/*
 * @Description: 
 * @FilePath: /Area-cpp/src/calculator.h
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2019-09-20 19:47:33
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-02-28 20:13:58
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#pragma once

#include <stack> //使用标准库里面的栈

using namespace std;

/******************************************************************************
* @describe 类：计算器
******************************************************************************/
class Calculator
{
public:
    Calculator();
    void printStacks(void);
    bool getFormula(void);
    void getFormula(string &str);
    double count(void);

private:
    bool FlagLastIsNumber;
    bool FlagCouldAdd0;
    double LastValue;

    string StrFormula;
    string UnknowNum;

    stack<char> Buffer;
    stack<char> Suffix;
    stack<char> TempStack;
    stack<double> Value;

    int operateLevel(int c);
    inline void pushToBuffer(char c);
    inline char popFromBuffer(void);
    inline void pushToSuffix(char c);
    inline char popFromSuffix(void);
    inline void pushToStack(char c);
    inline char popFromStack(void);
    void pushToValue(double f);
    double popFromValue(void);
    size_t digits(double num);
    char getop(string &str, char c);
    char binSearch(string &str, struct operation operate[], int ops);
    char mathOperate(char c);
    char getOperate(string &operate);
    int whenCountGetOperate(string &operate);

    void printStack(stack<char> sta);
    void printStack(stack<double> sta);
    void reverseStack(stack<char> &sta);

    void reverseFormulaToBuffer(void);
    void changeBufferToSuffix(void);
    void reverseSuffix(void);
};
