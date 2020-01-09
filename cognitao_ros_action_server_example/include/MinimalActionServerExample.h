#include <MinimalActionServer.h>
#include "../../../../devel/include/cognitao_ros/EventMsg.h"
#include "../../../../devel/include/cognitao_ros/getvar.h"
#include "../../../../devel/include/cognitao_ros/setvar.h"
// #include "ros/ros.h"
#include <thread>

using namespace std;
enum action_code
{
  waitRandom,
  wait,
  generateRandom,
  loop10,
  defaultNum
};

class MinimalActionServerExample : public MinimalActionServer
{

public:
  explicit MinimalActionServerExample()
      : MinimalActionServer()
  {
    ros::NodeHandle n_;
    srv_client_get = n_.serviceClient<cognitao_ros::getvar>("server_get_var");
    srv_client_set = n_.serviceClient<cognitao_ros::setvar>("server_set_var");
  }

  virtual void execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal) override
  {
    cout << " start execute " << goal.getGoal()->goal.actiontype << endl;
    const string goal_ = goal.getGoal()->goal.actiontype;
    bool returnValue = true;
    ros::Rate loop_rate(1);
    action_code g = hashit(goal_);
    cognitao_ros::ActionMsgResult result;
    cognitao_ros::ActionMsgFeedback feedback;
    cognitao_ros::getvar srv;
    cognitao_ros::setvar srvSet;
    result.resultvalue = false;
    for (auto const &param : goal.getGoal()->goal.parameters)
    {

      parameters[param.key] = param.value;
    }

    switch (g)
    {

    case waitRandom:
      cout << "enter to wait random" << endl;
      srv.request.key = "random";
      if (srv_client_get.call(srv))
      {
        std::this_thread::sleep_for(std::chrono::seconds(stoi(srv.response.value)));
        cout << "sleep random for " << srv.response.value << " seconds!!";
      }
      else
      {
        ROS_ERROR("ERROR");
      }
      // std::string rnd = WM::getVar("random");
      break;

    case wait:
      cout << "server wait for a seconddddd" << endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      break;

    case generateRandom:
    {
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

      srand(seed);
      int rnd = (rand() % 9) + 1;
      std::stringstream ss;
      ss <<rnd;
      srvSet.request.key = "random";
      srvSet.request.value = ss.str();
      if (srv_client_set.call(srvSet))
      {

        cout << "generate random to " << srvSet.request.value << " seconds!!";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // for(;;){
        //   cout<<"lin"<<endl;
        // }
      }
      else
      {
        ROS_ERROR("Failed to call service add_two_ints");
      }
    }
    break;

    case loop10:
    {
      std::stringstream ss;
      for (int i = 0; i < 10; i++)
      {
        ss << i;
        srvSet.request.key = "random";
        srvSet.request.value = ss.str();
        if (srv_client_set.call(srvSet))
        {

          cout << "loop10 to " << srvSet.request.value << " seconds!!";
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        else
        {
          ROS_ERROR("Failed to call service add_two_ints");
        }
      }
    }
    break;

    default:
      cout << "GOAL: " << goal_ << "---- UNKNOWN ACTION ----" << endl;
      break;
    }

    if (ros::ok())
    {
      std::cout << " GOAL IS DONE!!" << std::endl;
      result.resultvalue = true;
      cout << "result value in Server: " << int(result.resultvalue) << endl;
      // std::cout<<"2.PREEMPT IS ::::: "<<server.isPreemptRequested(goal);

      server.setSucceeded(goal, result);
    }

    cout<<"ANSWER "<<(int)result.resultvalue<<endl;
  }

  action_code hashit(std::string const &inString)
  {
    if (inString == "waitRandom")
      return waitRandom;
    if (inString == "wait")
      return wait;
    if (inString == "generateRandom")
      return generateRandom;
    if (inString == "loop10")
      return loop10;

    return defaultNum;
  }

private:
  std::thread stopReqThread_;
  std::map<std::string, std::string> parameters;

  ros::ServiceClient srv_client_get;
  ros::ServiceClient srv_client_set;
};