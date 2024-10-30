#include <unitree/robot/go2/video/video_client.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <bits/stdc++.h>
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
    string path="Photos";
    mkdir(path.c_str(),S_IRWXG | S_IRWXO | S_IRWXU);//create folder in "build" folder
    
    int StreamLen=1;
    int frame=1;
    auto starttime=chrono::steady_clock::now();
    cout<<"Give stream length:"<<endl;
    cin>>StreamLen;//can be replaced by StreamLen=constant,such as StreamLen=3 if needed.
    auto currenttime=chrono::steady_clock::now();
    auto elapsedtime=chrono::duration_cast<chrono::milliseconds>(currenttime-starttime);;

    vector<uint8_t> img;
    int ret;

    int timecnt=0,fcnt=0;

    for(int timecnt=0;timecnt<StreamLen;timecnt++)//second
    {
        starttime=chrono::steady_clock::now();
        ret=video_client.GetImageSample(img);
            if (ret == 0)
            {
                time_t rawtime;
                struct tm *timeinfo;
                char buffer[80];

                time(&rawtime);
                timeinfo=localtime(&rawtime);

                strftime(buffer, sizeof(buffer), "Photos/%Y%m%d%H%M%S.jpg", timeinfo);
                /*trying to use other funcs to replace this, in order to ensure a millisecond-level capture.*/
                string image_name(buffer);

                std::ofstream image_file(image_name, ios::binary);
                if(image_file.is_open())
                {
                    image_file.write(reinterpret_cast<const char*>(img.data()), img.size());
                    image_file.close();
                    cout<<"Image "<<image_name<<"saved successfully."<<endl;
                }
                else
                {
                    cout<<"Failed to save img."<<endl;
                }
                
            }
        //auto adjusting time to ensure time count accurant
        currenttime=chrono::steady_clock::now();
        elapsedtime=chrono::duration_cast<chrono::milliseconds>(currenttime-starttime);
        //cout<<(1000)/(1000*frame)<<endl; only for adjustment.
        sleep(1-(elapsedtime.count())/1000);
    }
    sleep(2);
    return 0;
}
/*
    Removed var "frame" since one func used did'n meet the standard of accurancy to ensure saving documents in diff names.
    Now fixing bugs and trying to make operation easier.
*/