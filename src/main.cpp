
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CogniTAO.h>
#include "ros/ros.h"

int main(int argc, char **argv) {

    ros::init(argc, argv, "some_node");

    std::cout<<"node running"<<std::endl;

    // cerr << " getenv(home) " << getenv("HOME") << endl;
    // cerr << " path " << getenv("HOME") << "/dm_ros/src/" << endl;

    stringstream ss;

    ss << getenv("HOME") << "/dm_ros/src/" << "cognitao.git/config_lin.json";

    cerr << ss.str() << endl;


    config(ss.str()); 
   
    return 0;
}
