#include <bits/stdc++.h>//
#include <unitree/robot/client/client.hpp>//
#include <robot/go2/sport/sport_client.hpp>//
//import para.

using namespace unitree::common;
using namespace unitree::robot;
using namespace std;
//allowing

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " networkInterface" << endl;
        exit(-1); 
    }

    go2::SportClient sport_client;
    unitree::robot::ChannelFactory::Instance()->Init(0,argv[1]);
    sport_client.SetTimeout(10.0f);
    sport_client.Init();
    sport_client.WaitLeaseApplied();

    int movetime=3;
    auto starttime=chrono::steady_clock::now();

    double t=0;
    double dt=0.01;
    while(1)
    {
        //functions.
        sport_client.Move(0.1,0.0,0.0);
        sport_client.BodyHeight(0.01*sin(1145*t));
        //time module 2 ctrl workin' time
        auto currenttime=chrono::steady_clock::now();
        auto elapsedtime=chrono::duration_cast<chrono::seconds>(currenttime-starttime);
        if(elapsedtime.count()>=movetime)//or et>=3
        {
            break;
        }
        t+=dt;
    }
    sleep(2);
    sport_client.BodyHeight(0);
    return 0;
}