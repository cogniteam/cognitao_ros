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

class UserFunctions
{
public:
	bool wait(BehaviourTask *b)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return true;
	}

	bool generateRandom(BehaviourTask *b)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		srand(seed);
		int rnd = (rand() % 9) + 1;
		WM::setVar("random", StringGenerator::itos(rnd));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		return true;
	}

	bool loop10(BehaviourTask *b)
	{
		for (int i = 0; i < 10; i++)
		{
			WM::setVar("loop", StringGenerator::itos(i));
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		return true;
	}

	bool waitRandom(BehaviourTask *b)
	{
		std::string rnd = WM::getVar("random");
		// std::this_thread::sleep_for(std::chrono::seconds(stoi(rnd)));
		std::this_thread::sleep_for(std::chrono::seconds(stoi("hello")));
		return true;
	}
};
int main(int argc, char **argv)
{
	std::cout << "node running" << std::endl;

	auto userFunctions = new UserFunctions();

	WM::init(new RosDataSource(argc, argv));

	Action::registerAction("wait", &UserFunctions::wait, userFunctions);
	Action::registerAction("generateRandom", &UserFunctions::generateRandom, userFunctions);
	Action::registerAction("loop10", &UserFunctions::loop10, userFunctions);
	Action::registerAction("waitRandom", &UserFunctions::waitRandom, userFunctions);

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
