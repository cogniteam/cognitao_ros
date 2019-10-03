
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CogniTAO.h>
#include "ros/ros.h"

int main(int argc, char **argv) {

    ros::init(argc, argv, "some_node");

    std::cout<<"node running"<<std::endl;

    // // cerr << " getenv(home) " << getenv("HOME") << endl;
    // // cerr << " path " << getenv("HOME") << "/dm_ros/src/" << endl;

    // stringstream ss;

    // ss << getenv("HOME") << "/dm_ros/src/" << "cognitao.git/config_lin.json";

    // cerr << ss.str() << endl;


   
   
    // config(ss.str()); 
   std::cout<<"linazan"<<std::endl;
    
WM::init(new MapThreadSafeDataSource());
   	std::ostringstream strs;
	WM::setVar("EVENT","");
	std::cout<<"linazan 1"<<std::endl;

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
		   std::cout<<"linazan 2"<<std::endl;

	m.start();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	bool lockCoinEvent_,unLockCoinEvent_,unLockPushEvent_,lockPushEvent_;

	WM::setVar("COIN","event");
	(m.getState()->getName().compare("unlocked")==0)?
			lockCoinEvent_=true:lockCoinEvent_=false;
	std::this_thread::sleep_for(std::chrono::seconds(2));

	WM::setVar("COIN","event");
	(m.getState()->getName().compare("unlocked")==0)?
			unLockCoinEvent_=true:unLockCoinEvent_=false;
	std::this_thread::sleep_for(std::chrono::seconds(2));

	WM::setVar("PUSH","event");
	(m.getState()->getName().compare("locked")==0)?
			unLockPushEvent_=true:unLockPushEvent_=false;
	std::this_thread::sleep_for(std::chrono::seconds(2));

	WM::setVar("PUSH","event");
	(m.getState()->getName().compare("locked")==0)?
			lockPushEvent_=true:lockPushEvent_=false;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	   std::cout<<"linazan 3"<<std::endl;

	WM::setVar("COIN","event");
	(m.getState()->getName().compare("unlocked")==0)?
			lockCoinEvent_=true:lockCoinEvent_=false;

	(lockCoinEvent_&&lockPushEvent_&&unLockCoinEvent_&&unLockPushEvent_)?
			strs << __func__ << " PASSED ": strs << __func__ << " FAILED ";

	std::cout<<strs.str()<<std::endl;

    return 0;
}
