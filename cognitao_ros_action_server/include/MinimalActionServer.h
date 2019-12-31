#include <cognitao_ros/ActionMsgAction.h>
#include "MultiGoalActionServer.h"

#include <string>
#include <iostream>
#include <thread>

using namespace std;

class MinimalActionServer
{
protected:
  ros::NodeHandle nh_;
  //actionlib::SimpleActionServer<cognitao_ros::ActionMsgAction> server;
  actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction> server;

public:
  MinimalActionServer() : server(nh_, "cognitao_ros",
                                 boost::bind(&MinimalActionServer::execute, this, _1), false)
  {

    std::cout << "server is running.." << std::endl;
    server.start(); //start the server running
  }

  ~MinimalActionServer(void){}

  virtual void execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal)
  {
    for (int i = 0; i < 20; i++)
    {
      //std::cout<<"doing mission.."<<std::endl;
    }
    server.setSucceeded(goal);
    cout << "finished " << endl;
  }
};
