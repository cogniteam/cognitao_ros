
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CogniTAO.h>
#include "ros/ros.h"
#include "../include/RosDataSource.h"
#include "../include/StateRosProxy.h"
#include <actionlib/client/simple_action_client.h> 
#include <action_manager/ActionMsgAction.h> 

int main(int argc, char **argv) {

    

    std::cout<<"node running"<<std::endl;
    
	WM::init(new RosDataSource(argc, argv));
	

	UILink link_("src/cognitao.git/www/","127.0.0.1",1234);
	link_.start();
	
	auto s1 = new StateRosProxy("DriveForward_With_Timer");
	
	auto s2 = new StateRosProxy("DriveBackward_With_Timer");

	Machine m;
	auto E1 = new ProtocolTransition ({"TO_DriveForward"});
	auto E2 = new ProtocolTransition ({"TO_DriveBackward"});
	m.setInitialTask(s1);
	
	m.addLink(s2,s1,E1);
	m.addLink(s1,s2,E2);


	Task * stateS1 =TaskFactory::createTask("state","root");
	stateS1->setMachine(&m);
	WM::setVar("GRAPH", StateJSONWriter::toString(stateS1)  );




	// cout<<StateJSONWriter::toString(stateS1)<<endl;



	m.start();

	std::this_thread::sleep_for(std::chrono::seconds(1000));
	cout<<" stop everything "<<endl;
	m.stop();
	link_.stop();
	exit(0);
    return 0;
}
