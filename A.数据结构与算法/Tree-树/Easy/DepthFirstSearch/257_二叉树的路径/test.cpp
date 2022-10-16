/*=========================================================================\
* Copyright(C)2016 Chudai.
*
* File name    : test.cpp
* Version      : v1.0.0
* Author       : 初代
* Date         : 2016/08/06
* Description  :
* Function list: 1.
*                2.
*                3.
* History      :
\*=========================================================================*/

/*-----------------------------------------------------------*
 * 头文件                                                    *
 *-----------------------------------------------------------*/
#include <iostream>
#include <sstream>
using namespace std;

int main(void)
{
     string str ;
     int a = 5;
     stringstream ss;

     ss << a;

     str = "->" + ss.str();

//     str = "->" + ;
     cout << str;

     return 0;
}
