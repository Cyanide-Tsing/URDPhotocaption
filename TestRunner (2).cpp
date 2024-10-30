#include <iostream>
#include <fstream>
#include <ctime>
#include <bits/stdc++.h>
using namespace std;
int main()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo=localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "Photos/%Y%m%d%H%M%S", timeinfo);
    /*trying to use other funcs to replace this, in order to ensure a millisecond-level capture.*/
    buffer=buffer+"114514";
    string image_name(buffer);
    cout<<buffer<<endl;
    sleep(3);
    return 0;
}