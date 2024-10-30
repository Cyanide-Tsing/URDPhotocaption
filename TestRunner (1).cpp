#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <bits/stdc++.h>
using namespace std;
int main()
{
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
    long int accutime=time.time_since_epoch().count();
    std::string buffer=to_string(accutime);
    buffer="Photos/"+buffer+".jpg";
    string image_name(buffer);
    cout<<buffer<<endl;
    sleep(3);
    return 0;
}