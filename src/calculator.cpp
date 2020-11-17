/*
 * @Description: 
 * @FilePath: /Area-cpp/src/calculator.cpp
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2018-09-20 19:47:33
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-02-28 20:13:48
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#include <iostream>
#include <stack>    //使用标准库里面的栈
#include <cstring>
#include <cctype>
#include <climits>
#include <algorithm>
#include <string>
#include <cmath>
#include <cstdlib>
#include "calculator.h"

using namespace std;

//#define TEXT

#define MAXOP     (100)
#define BUFSIZE   (200)
#define OPLEN_MAX (8)

#define PO_FLOAT  (1e6)
#define NE_FLOAT  (1e-6)

#define MYERROR   (-1)
#define SCTERROR  (-32767)

#define PI        (3.14159265358979)
#define E         (2.718281828459)
#define DPI       (PI/128)

#define NUMBER    '0'

#define SQRT      '['
#define FABS      '|'

#define SIN       's'
#define COS       'c'
#define TAN       't'
#define SINH      'S'
#define COSH      'C'
#define TANH      'T'

#define LN        'l'
#define LOG       'L'

#define ARCSIN    'i'   //  取第5个字符i，注意操作符不能用数字
#define ARCCOS    'o'
#define ARCTAN    'a'

#define NEWFORMULA (0)
#define QUIT       (-1)
#define RETURN     (-2)

Calculator :: Calculator(void)
{
    FlagLastIsNumber = false;
    FlagCouldAdd0 = false;
    LastValue = 0;
}

void Calculator :: printStack(stack <char> sta)
{
    stack <char> temp;
    char c;

    while(!sta.empty()){
        c = sta.top();
        sta.pop();
        temp.push(c);
    }
    while(!temp.empty()){
        c = temp.top();
        temp.pop();
        cout << c;
    }
}

void Calculator :: printStack(stack <double> sta)
{
    stack <double> temp;
    double c;

    while(!sta.empty()){
        c = sta.top();
        sta.pop();
        temp.push(c);
    }
    while(!temp.empty()){
        c = temp.top();
        temp.pop();
        cout << c;
    }
    cout << endl;
}

void Calculator :: printStacks(void) 
{
    cout << "Buffer    : ";
    printStack(Buffer);
    cout << endl;
    cout << "Suffix    : ";
    printStack(Suffix);
    cout << endl;
    cout << "TempStack : ";
    printStack(TempStack);
    cout << endl;
    cout << "Value     : ";
    printStack(Value);
    cout << endl;
}

// 返回一个运算符的运算等级
int Calculator :: operateLevel(int c) 
{
    switch(c) 
    {
        case '(' : return 0;
        case ')' : return 0;
        case '+' : return 1;
        case '-' : return 1;
        case '*' : return 2;
        case '/' : return 2;
        case '%' : return 2;
        case '^' : return 3;
        case SQRT :return 3;    //  '[' 开根号
        case FABS :return 3;    //  '|' 取绝对值
        case SINH :return 3;
        case COSH :return 3;
        case TANH :return 3;
        case SIN : return 3;
        case COS : return 3;
        case TAN : return 3;
        case LOG : return 3;
        case LN  : return 3;
        case ARCSIN : return 3;
        case ARCCOS : return 3;
        case ARCTAN : return 3;
        default  : return MYERROR;
    }
}

// 将一个字符返回的输入流中
inline void Calculator :: pushToBuffer(char c)
{
    Buffer.push(c);
}

// 从输入流返回一个字符
inline char Calculator :: popFromBuffer(void)
{
    char top;
    
    if(!Buffer.empty()){
        top = Buffer.top();
        Buffer.pop();
        return top;
    }
    return -1;
}

// 把c压入到后缀式的栈中
inline void Calculator :: pushToSuffix(char c)
{
    Suffix.push(c);
}

// 从g_cSuffix[]后缀式返回一个字符
inline char Calculator :: popFromSuffix(void)
{
    if(!Suffix.empty()){
        char top = Suffix.top();
        Suffix.pop();
        return top;
    }
    else{
        cout << Suffix.size() << " stack 'Suffix' is empty!" << endl;
        return -1;
    }
}

// 把c压入到后缀式的栈中
inline void Calculator :: pushToStack(char c)
{
    TempStack.push(c);
}

// 从g_cSuffix[]后缀式返回一个字符
inline char Calculator :: popFromStack(void)
{
    if(!TempStack.empty()){
        char top = TempStack.top();
        TempStack.pop();
        return top;
    }
    else{
        cout << TempStack.size() << " stack 'TempStack' is empty!" << endl;
        return -1;
    }
}

// 把f压入到值栈中
void Calculator :: pushToValue(double f)
{
    Value.push(f);
}

// 弹出并返回栈顶的值
double Calculator :: popFromValue(void) 
{
    if(!Value.empty()){
        double top = Value.top();
        Value.pop();
        return top;
    }
    else{
        cout << Value.size() << " stack 'Value' is empty!" << endl;
        return 0.0;
    }
}

// 获取一个算式
bool Calculator :: getFormula(void)
{
    return cin >> StrFormula;
}

// 获取一个算式
void Calculator :: getFormula(string &str)
{
    StrFormula = str;
}

// 倒转算式
void Calculator :: reverseFormulaToBuffer(void)
{
    reverse(StrFormula.begin(), StrFormula.end());

    int len = StrFormula.size();
    const char *str = StrFormula.c_str();

    pushToBuffer('=');
    for(int i = 0; i < len; i++){
        pushToBuffer(str[i]);
    }
}

// 计算小数位数
size_t Calculator :: digits(double num)
{
    int a, n = 0;
    const double b = 1e-6;
    
    a = num;
    num = num - a;
    while((b < num && num < 1 - b) || (b - 1 < num && num < -b)){
        num = num * 10;
        a = num;
        num = num - a;
        n++;
    }
    return n;
}

// 匹配运算符
struct operation  
{
    string  word;               // 运算符字符串
    int     time;               // 出现次数
    char    op;                 // 运算符值（单字符）
};

char Calculator :: getop(string &str, char c)
{   
    int i;
    char type;

    str[0] = c;                 // 持续检测下一个字符，直到下一个字符是回车或者不是字母
    for (i = 1; (type = popFromBuffer()) != '\n' && isalpha(type); i++) 
    {
        str[i] = type;
        if (type == '(') {      // 检测到左括号(，停止检测
            i++;
            break;
        }
    }                           // i始终指向srting的下一个地址
    str[i] = '\0';
    if (i < OPLEN_MAX) {        // 操作得到的string运算符一般小于8个字节
        if (type != '\n') {     // 如果下一个字符不是回车结束符，那么把这个字符重新返回到输入流中
            pushToBuffer(type);
        }
        return 1;
    }
    return MYERROR;             // 如果i == OPLEN_MAX，就返回错误值-1
}

// 在字符串集中中搜索匹配word字符串的字符串，返回找到的字符串集的下标，ops为字符串的长度
char Calculator :: binSearch(string &str, struct operation operate[], int ops)  
{
    int cond;
    int low, high, mid;

    low = 0;
    high = ops - 1;

    while (low <= high) {
        mid = (low + high) / 2;
        if ((cond = strcmp(str.c_str(), operate[mid].word.c_str())) < 0) {
            high = mid - 1;
        }
        else if (cond > 0) {
            low = mid + 1;
        }
        else {
            return mid;
        }
    }
    return MYERROR;
}

// 匹配数学运算符
char Calculator :: mathOperate(char c)
{
    struct operation operate[] = { //按ascll码顺序从小到大排放
        "ARCCOS", 0, ARCCOS,
        "ARCSIN", 0, ARCSIN,
        "ARCTAN", 0, ARCTAN,
        "COSH", 0, COSH,
        "COS" , 0, COS,
        "FABS", 0, FABS,
        "LN"  , 0, LN,
        "LOG" , 0, LOG,
        "SINH", 0, SINH,
        "SIN" , 0, SIN,
        "SQRT", 0, SQRT,
        "TANH", 0, TANH,
        "TAN" , 0, TAN,

        "arccos", 0, ARCCOS,
        "arcsin", 0, ARCSIN,
        "arctan", 0, ARCTAN,
        "cosh", 0, COSH,
        "cos" , 0, COS,
        "fabs", 0, FABS,
        "ln"  , 0, LN,
        "log" , 0, LOG,
        "sinh", 0, SINH,
        "sin" , 0, SIN,
        "sqrt", 0, SQRT,
        "tanh", 0, TANH,
        "tan" , 0, TAN,
    };
    string str;
    int find;
    int ops = (int)(sizeof(operate) / sizeof(struct operation)); // 字符串个个数

    getop(str, c);
    if ((find = binSearch(str, operate, ops)) != MYERROR)
    {
        operate[find].time++;           // 运算符出现的次数++
        return operate[find].op;        // 返回运算符（单字符）
    }
    return MYERROR;
}

// 获得第一个运算符
char Calculator :: getOperate(string &operate)
{
    int pStr = 0;
    char c;

   while ((c = popFromBuffer()) == ' ' || c == '\t') // 跳过所有的空格
       ;
    operate[pStr++] = c;
    operate[pStr++] = '\0';

    if (!isdigit(c) && c != '.') 
    {
        if (FlagLastIsNumber == true) {       // 上一个值是数字，但是下一个是运算符，两者之间没有乘，自动添加一个
            if (c == '(' || c == '[' || c == '|' 
                || c == 'f' || c == 's' || c == 'c' || c == 't' || c == 'l'
                || c == 'p' || c == 'e' || c == 'a'
                || c == 'F' || c == 'S' || c == 'C' || c == 'T' || c == 'L'
                || c == 'P' || c == 'E' || c == 'A') {  //  数字 (,[ 之间没有乘*
                pushToBuffer(c);
                FlagLastIsNumber = false;
                FlagCouldAdd0 = false;
                return '*';
            }
        }
        else {
            if (c == 'f' ||c == 's' ||c == 'c'||c == 't'||c == 'l'||c == 'a'
                ||c == 'F'||c == 'S'||c == 'C'||c == 'T'||c == 'L'||c == 'A') {
                FlagLastIsNumber = false;
                FlagCouldAdd0 = true;
                return mathOperate(c);
            }
            else if (c == 'p' || c == 'P') {//  是pi = PI = 3.1415926, e = 2.7     
                FlagLastIsNumber = true;
                FlagCouldAdd0 = false;
                operate = "3.14159265358979";
                return NUMBER;
            }
            else if (c == 'e' || c == 'E') {
                FlagLastIsNumber = true;
                FlagCouldAdd0 = false;
                operate = "2.718281828459";
                return NUMBER;
            }                               //  运算符+-前面没有数字，自动添加一个0
            else if (c == 'x' || c == 'X') {
                FlagLastIsNumber = true;
                FlagCouldAdd0 = false;
                operate = UnknowNum;
                return NUMBER;
            } 
            else if (FlagCouldAdd0 == true 
                && (c == '-' || c == '+' || c == '*' || c == '/' || c == '%')){
                pushToBuffer(c);
                FlagLastIsNumber = true;
                FlagCouldAdd0 = false;
                operate = '0';
                return NUMBER;
            }
        }
        if (c == '(' || c == '[' || c == '|') {
            FlagCouldAdd0 = true;
        }
        else {
            FlagCouldAdd0 = false;
        }
        FlagLastIsNumber = false;
        return c;                   //  不是数
    }
    else{
        pStr--;                     //  覆盖'\0'
        if (isdigit(c)) {           //  收集整数部分
            while (isdigit(operate[pStr++] = c = popFromBuffer()))
                ;
        }
        if (c == '.') {             //  收集小数部分
            while (isdigit(operate[pStr++] = c = popFromBuffer()))
                ;
        }
        operate[--pStr] = '\0';
        if (c != EOF) {
            pushToBuffer(c);
        }
        FlagLastIsNumber = true;
        return NUMBER;
    }
}

// 将输入流中的内容转化为一个后缀式
void Calculator :: changeBufferToSuffix(void)
{
    string operate;
    int pOperate = 0;
    char type, op;
    int typeLevel;
    int lastLevel = 0;              //运算等级
    
    while ((type = getOperate(operate)) != '=') 
    {
        switch (type) {
            case NUMBER:
                for (pOperate = 0; operate[pOperate] != '\0'; pOperate++) {
                    pushToSuffix(operate[pOperate]);
                }
                pushToSuffix(' ');
                break;
            case '+' : case '-' : 
            case '*' : case '/' : 
            case '%' : case '^' : 
            case SQRT :
            case FABS :
            case SINH :case COSH :case TANH :
            case SIN : case COS : case TAN :
            case LOG : case LN  : 
            case ARCSIN :
            case ARCCOS :
            case ARCTAN :
                typeLevel = operateLevel(type);
                if (typeLevel > lastLevel) {
                    pushToStack(type);
                }
                else {
                    if(!TempStack.empty()){
                        op = popFromStack();
                        while (typeLevel <= operateLevel(op)) {
                            pushToSuffix(op);
                            pushToSuffix(' ');
                            if(!TempStack.empty()){
                                op = popFromStack();
                            }
                            else{
                                break;
                            }
                        }
                    }
                    pushToStack(type);
                }
                lastLevel = typeLevel;
                break;
            case '(':
                lastLevel = operateLevel(type);
                pushToStack(type);
                break;
            case ')':
                if(!TempStack.empty()){
                    op = popFromStack();
                    while (operateLevel(op) > 0) {
                        pushToSuffix(op);
                        pushToSuffix(' ');
                        if(!TempStack.empty()){
                            op = popFromStack();
                        }
                        else{
                            break;
                        }
                    }
                }
                if(!TempStack.empty()){
                    op = popFromStack();                // 得到'('前一个运算符的运算等级
                    lastLevel = operateLevel(op);       // 记录为上一次运算等级
                    pushToStack(op);                    // 放回后缀式中
                }
                else{
                    lastLevel = operateLevel(op);
                }
                break;
            default:
                cout << "[Error]: Unknown command [ " << operate << " ]" << endl;
                break;
        }
    }

    while (!TempStack.empty()) {
        pushToSuffix(popFromStack());
        pushToSuffix(' ');
    }
    pushToSuffix('=');
}

// 倒转缓存栈
void Calculator :: reverseStack(stack <char> &sta)
{
    stack <char> temp1;
    stack <char> temp2;
    char c;

    while(!sta.empty()){
        c = sta.top();
        sta.pop();
        temp1.push(c);
    }
    while(!temp1.empty()){
        c = temp1.top();
        temp1.pop();
        temp2.push(c);
    }
    while(!temp2.empty()){
        c = temp2.top();
        temp2.pop();
        sta.push(c);
    }
}

void Calculator :: reverseSuffix(void)
{
    reverseStack(Suffix);
}

// 计算时获取下一个运算符或者数值操作数
int Calculator :: whenCountGetOperate(string &operate)
{
    int  pStr = 0;
    char c;
    
    while ((operate[0] = c = popFromSuffix()) == ' ' || c == '\t')
        ;
    operate[1] = '\0';
    if (!isdigit(c) && c != '.') {
        FlagLastIsNumber = false;
        return c;               //  不是数
    }
    pStr = 0;
    if (isdigit(c)) {           //  收集整数部分
        while (isdigit(operate[++pStr] = c = popFromSuffix()))
            ;
    }
    if (c == '.') {             //  收集小数部分
        while (isdigit(operate[++pStr] = c = popFromSuffix()))
            ;
    }
    operate[pStr] = '\0';
    if (c != EOF) {
        pushToSuffix(c);
    }
    FlagLastIsNumber = true;
    return NUMBER;
}

// 计算结果函数
double Calculator :: count(void)
{
    int type;
    double op1, op2;
    double lfResult;
    long ldResult;
    string str;

	#ifdef TEXT
		printStacks();
		cout << "reverseFormulaToBuffer:" << endl;
	#endif
	reverseFormulaToBuffer();
	#ifdef TEXT
		printStacks();
		cout << "changeBufferToSuffix:" << endl;
	#endif
	changeBufferToSuffix();
	#ifdef TEXT
		printStacks();
		cout << "reverseSuffix:" << endl;
	#endif
	reverseSuffix();
	#ifdef TEXT
		printStacks();
	#endif

    while ((type = whenCountGetOperate(str)) != EOF) 
    {
        switch (type) 
        {
            case NUMBER :
                pushToValue(atof(str.c_str()));
                break;
            case '+' :
                pushToValue(popFromValue() + popFromValue());
                break;
            case '-' :
                op2 = popFromValue();
                pushToValue(popFromValue() - op2);
                break;
            case '*' :
                op2 = popFromValue();
                pushToValue(popFromValue() * op2);
                break;
            case '/' :
                op2 = popFromValue();
                if (op2 != 0.0) {
                    pushToValue(popFromValue() / op2);
                }
                else {
                    cout << "[Error]: Zero divisor." << endl;
                }
                break;
            case '%' :
                op2 = popFromValue();          //不可以互换顺序
                if (digits(op2) == 0) {
                    op1 = popFromValue();
                    if (digits(op1) == 0) {
                        pushToValue((double)((int)op1 % (int)op2));
                    }
                }
                else {
                    cout << "[Error]: Can't operate '%%'." << endl;
                }
                break;
            case '^' :
                op2 = popFromValue();
                op1 = popFromValue();          //不可以互换顺序
                if (op1 == 0.0 && op2 >= 0.0) {
                    pushToValue(0.0);
                }
                else if (op1 == 0.0 && op2 < 0.0) {
                    cout << "[Error]: Can't operate 'pow()'." << endl;
                }
                else if (op2 == 0.0) {
                    pushToValue(1.0);
                }
                else {
                    pushToValue(pow(op1, op2));
                }
                break;
            case SQRT :
                op2 = popFromValue();
                if (op2 >= 0) {
                    pushToValue(sqrt(op2)); 
                }
                else {
                    cout << "[Error]: Can't operate 'fabs()'." << endl;
                }
                break;
            case FABS :pushToValue(fabs(popFromValue())); break;
            case SINH :pushToValue(sinh(popFromValue())); break;
            case COSH :pushToValue(cosh(popFromValue())); break;
            case TANH :pushToValue(tanh(popFromValue())); break;
            case SIN : 
                op2 = popFromValue();
                lfResult = ((double)((long)(fabs(op2 - PI) * PO_FLOAT))) 
                                  / ((long)(PI * PO_FLOAT));
                ldResult = lfResult * PO_FLOAT;
                if (ldResult % ((long)(PO_FLOAT/1)) < 10) {     //  可以容忍小数点后第6位上的误差
                    pushToValue(0.0);
                }
                else {
                    pushToValue(sin(op2));
                }
                break;
            case COS : 
                pushToValue(cos(popFromValue()));
                break;
            case TAN : 
                op2 = popFromValue();
                lfResult = ((double)((long)(fabs(op2 - PI) * PO_FLOAT))) 
                                  / ((long)(PI * PO_FLOAT));
                ldResult = lfResult * PO_FLOAT;
                if (ldResult % ((long)(PO_FLOAT/2)) < 10) {
                    pushToValue(LONG_MAX);
                    cout << "[Error]: 'tan((+/-)p/2)' is worse!" << endl;
                }
                else {
                    pushToValue(tan(op2));
                }
                break;
            case LOG : pushToValue(log10(popFromValue())); break;
            case LN  : pushToValue(log(popFromValue()));   break;
            case ARCSIN : pushToValue(asin(popFromValue())); break;
            case ARCCOS : pushToValue(acos(popFromValue())); break;
            case ARCTAN : pushToValue(atan(popFromValue())); break;
            case '=' :
                cout << "-------------------------------------------------" << endl;
                LastValue = popFromValue();
                cout << "Value = " << LastValue << endl << endl;
                return LastValue;
            default:
                cout << "[Error]: Unknown command [ " << str << "]." << endl;
                break;
        }
    }
    cout << "Count error !!" << endl;
    return 0.0;
}