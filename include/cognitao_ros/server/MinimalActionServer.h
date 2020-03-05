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
  std::map<std::string, std::string> parameters;

public:
  MinimalActionServer();

  ~MinimalActionServer(void);
  virtual void onStart(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal) = 0;
  virtual void onStop() = 0;
  virtual std::map<std::string, std::string> getParam(const string &name) const = 0;

  virtual void execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal);
 
};
