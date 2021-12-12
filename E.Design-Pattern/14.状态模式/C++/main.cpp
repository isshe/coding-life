
#include "TCPConnection.h"
#include <iostream>
using namespace std;

int main()
{
    TCPConnection tc;
    tc.ActiveOpen();
    tc.Close();
    return 0;
}