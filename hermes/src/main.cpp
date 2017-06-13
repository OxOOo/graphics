#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "time_log.h"

using namespace std;

int main()
{
    setTimePoint("PROCESS");
    cout << "Hello World" << endl;
    logTimePoint("PROCESS");
    return 0;
}