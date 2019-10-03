
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CogniTAO.h>
#include "ros/ros.h"
#include "/home/lin/dm_ros/src/dm_ros/include/RosDataSource.h"

int main(int argc, char **argv) {

    ros::init(argc, argv, "some_node");

    std::cout<<"node running"<<std::endl;
    
	WM::init(new rosns::RosDataSource());



	auto s1 = new State("locked");
	auto s2 = new State("unlocked");

	Machine m;
	auto E1 = new ProtocolTransition ("PUSH");
	auto E2 = new ProtocolTransition ("COIN");
	m.setInitialTask(s1);
	m.addLink(s1,s2,E2);
	m.addLink(s2,s1,E1);
	m.addLink(s1,s1,E1);
	m.addLink(s2,s2,E2);


	m.start();

	// std::this_thread::sleep_for(std::chrono::seconds(10));
	// //bool lockCoinEvent_,unLockCoinEvent_,unLockPushEvent_,lockPushEvent_;
	// cout<<" setVar "<<endl;
	// WM::setVar("COIN","event");
	// //(m.getState()->getName().compare("unlocked")==0)?




    return 0;
}
