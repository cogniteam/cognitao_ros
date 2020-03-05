#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CogniTao.h>
#include "../include/cognitao_ros/client/Ros1Runner.h"

#include <atomic>
// #include "ros/ros.h"
// #include "../include/RosDataSource.h"
// #include <actionlib/client/simple_action_client.h>
// #include <cognitao_ros/ActionMsgAction.h>
// #include "../include/Ros1Runner.h"
// #include "../include/Ros1RunnerFactoryMethod.h"

// static std::atomic<int> counter(0);
extern "C" Runner *create_runner()
{
	if (!ros::isInitialized())
	{
		cout<<"NOT INITIALIZE!!!!!"<<endl;
		int n = 0;
		ros::init(n, nullptr, "cognitao_ros1",ros::init_options::NoSigintHandler);
	}
	// counter++;
	return new Ros1Runner();
}

extern "C" void destroy_runner(Runner *object)
{
	// counter--;
	delete object;
}

extern "C" const char *get_runner_type()
{
	return "ros1_runner";
}

int main(int argc, char **argv)
{

	std::cout << "node running" << std::endl;

	// map<string, string> map_;
	// map_["time"] = "60.0";
	// Runner *r1 = create_runner();
	// Runner *r2 = create_runner();
	// r1->setAction("wait");
	// r1->setParameters(map_);
	// r1->run();
	// r2->setAction("wait");
	// r2->setParameters(map_);
	// r2->run();


	// WM::init(new RosDataSource(argc, argv));

	// RunnerFactory::registerFactoryMethod("ros1", new Ros1RunnerFactoryMethod());

	 //Config config("src/cognitao_ros/cognitao_ros/ros_config_debug.json");

	// //update WM variables from config
	// for (auto elem : config.getWmElements())
	// {
	// 	WM::setVar(elem.first, elem.second);
	// }

	// // read machine from XML
	// Machine *m = MachineXMLReader::read(config.getMachineXmlPath());

	// //create WebServer instance with a machine
	// MachineWebServer w(
	// 	config.getWebServerBasePath(),
	// 	config.getWebServerIp(),
	// 	config.getWebServerPort(), m);

	// m->start();

	// while (true) //!m->getCurrentState()->hasReturn())
	// {
	// 	std::this_thread::sleep_for(std::chrono::seconds(1));
	// }

	// exit(0);
	return 0;
}