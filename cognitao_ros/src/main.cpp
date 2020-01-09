#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CogniTAO.h>
#include "ros/ros.h"
#include "../include/RosDataSource.h"
#include <actionlib/client/simple_action_client.h>
#include <cognitao_ros/ActionMsgAction.h>
#include "../include/Ros1Runner.h"
#include "../include/Ros1RunnerFactoryMethod.h"


int main(int argc, char **argv)
{
	std::cout << "node running" << std::endl;


	WM::init(new RosDataSource(argc, argv));


	RunnerFactory::registerFactoryMethod("ros1", new Ros1RunnerFactoryMethod());

	Config config("src/cognitao_ros/cognitao_ros/ros_config_debug.json");

	//update WM variables from config
	for (auto elem : config.getWmElements())
	{
		WM::setVar(elem.first, elem.second);
	}

	// read machine from XML
	Machine *m = MachineXMLReader::read(config.getMachineXmlPath());


	//create WebServer instance with a machine
	MachineWebServer w(
		config.getWebServerBasePath(),
		config.getWebServerIp(),
		config.getWebServerPort(), m);

	m->start();

	while (true) //!m->getCurrentState()->hasReturn())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	exit(0);
	return 0;
}
