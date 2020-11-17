/*
 * @Description: 
 * @FilePath: /Area-cpp/src/area.cpp
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2019-09-20 19:47:33
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-02-29 22:32:03
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#include <iostream>
#include <string>
#include "calculator.h"

using namespace std;

int main(int argc, char const *argv[])
{
	Calculator calculator;
	
	cout << "Please end with 'EOF'" << endl;
	cout << "---------------------------" << endl;
	cout << "(Windows     : ctrl + z)" << endl;
	cout << "(Linux/MacOS : control + d)" << endl;
	cout << "---------------------------" << endl; 
	cout << "Please input a formula" << endl;
    while (calculator.getFormula())
    {
    	calculator.count();
    	cout << "Please input a formula" << endl;
    }
    return 0;
}
