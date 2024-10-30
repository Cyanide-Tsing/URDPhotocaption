/*
    This is an example based on Snapcatch v3, showing the possibility of using moving command as well as obtaining picture stream.
    Moving command will work in a low frame rate to prevent possible glitches.
    To run the example, plz 
*/

#include <unitree/robot/go2/video/video_client.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <bits/stdc++.h>
#include <unitree/robot/client/client.hpp>
#include <unitree/robot/go2/sport/sport_client.hpp>

using namespace unitree::common;
using namespace unitree::robot;
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
      std::cout << "Usage: " << argv[0] << " networkInterface" << std::endl;//for all sdks -w-
      exit(-1);
    }
    unitree::robot::ChannelFactory::Instance()->Init(0, argv[1]);
    unitree::robot::go2::VideoClient video_client;

    video_client.SetTimeout(1.0f);
    video_client.Init();
    time_t rawtime;
    struct tm *timeinfo;
    char folder[80];

    go2::SportClient sport_client;
    sport_client.SetTimeout(10.0f);
    sport_client.Init();
    sport_client.WaitLeaseApplied();

    time(&rawtime);
    timeinfo=localtime(&rawtime);
    strftime(folder, sizeof(folder), "Photos-%Y%m%d%H%M%S", timeinfo);//folder timestamp
    string path;
    path=folder;
    mkdir(path.c_str(),S_IRWXG | S_IRWXO | S_IRWXU);//create folder in "build" folder
    
    int StreamLen=1;
    int frame=1;
    auto starttime=chrono::steady_clock::now();

    struct LinearDat{
        double dir;
        double x;
        double y;
    };
    struct CircularDat{
        double rad;
        double r;
    };

    LinearDat linear={0.0,0.0,0.0};
    CircularDat c={0.0,0.0};

    cout<<"Give stream length:";
    cin>>StreamLen;//can be replaced by StreamLen=constant,such as StreamLen=3 if needed.
    cout<<"Give frame per sec:";
    cin>>frame;
    if (frame>3)
    {
        cout<<"Frame rate should be less than 4."<<endl;
        frame=3;
    }
    cout<<"Give moving command:"<<endl<<"move in a trail of(l=linear):";
    std::string trail;
    cin>>trail;
    if (trail!="l")
    {
        if (trail=="c")
        {
            cout<<"move in an angle of(use deg instead of rad):";
            cin>>c.rad;
            cout<<"moving in radius of:";
            cin>>c.r;
        }
        else
        {
            cout<<"Undef. trail."<<endl;
            trail="l";
        }
    }
    else
    {
        cout<<"move in a direction of(use deg instead of rad):";
        cin>>linear.dir;
        if (linear.dir>=360)
        {
            cout<<"Out of range."<<endl;
            linear.dir=0;
        }
        cout<<"moving in x-axis:";
        cin>>linear.x;
        cout<<"moving in y-axis";
        cin>>linear.y;
    }
    
    auto currenttime=chrono::steady_clock::now();
    auto elapsedtime=chrono::duration_cast<chrono::microseconds>(currenttime-starttime);;

    vector<uint8_t> img;
    int ret;

    int timecnt=0,fcnt=0;

    if (trail=="l")
    {
        sport_client.Move(0.0,0.0,linear.dir);
    }

    for(int timecnt=0;timecnt<StreamLen;timecnt++)//second
    {
        for(int fcnt=0;fcnt<frame;fcnt++)//frame
        {
            starttime=chrono::steady_clock::now();
            ret=video_client.GetImageSample(img);
            if (ret == 0)
            {

                /*To ensure a millisecond-level capture without replacing pictures.*/
                std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
                long int accutime=time.time_since_epoch().count();
                std::string buffer=to_string(accutime);
                buffer=path+"/"+buffer+".jpg";
                string image_name(buffer);

                std::ofstream image_file(image_name, ios::binary);
                if(image_file.is_open())
                {
                    image_file.write(reinterpret_cast<const char*>(img.data()), img.size());
                    image_file.close();
                    cout<<"Image "<<image_name<<" saved successfully."<<endl;
                }
                else
                {
                    cout<<"Failed to save img."<<endl;
                }
                
            }
            //functions.
            if (trail=="l")
            {
                sport_client.Move(linear.x/frame,linear.y/frame,0.0);
            }
            else
            {
                if (trail=="c")
                {
                    sport_client.Move((c.r*c.rad)/(180/frame),0,c.rad/(frame*StreamLen));
                }
            }
            //auto adjusting time to ensure time count accurant
            currenttime=chrono::steady_clock::now();
            elapsedtime=chrono::duration_cast<chrono::microseconds>(currenttime-starttime);
            //double frmt=1000/frame;
            //cout<<(1000000-elapsedtime.count())/frame<<endl; 
            //only for adjustment.
            usleep((1000000-elapsedtime.count())/frame);
            /*cout<<(1000000/frame)<<endl;
            usleep(1000000/frame);*/
        }
        
    }
    sleep(2);
    return 0;
}
/*
    Now fixin' bugs n tryin' 2 make operation easier.
*/