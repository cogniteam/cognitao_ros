#include <cognitao_ros/server/MinimalActionServer.h>
#include "../../../devel/include/cognitao_ros/EventMsg.h"
#include "../../../devel/include/cognitao_ros/getvar.h"
#include "../../../devel/include/cognitao_ros/setvar.h"
// #include "ros/ros.h"
#include <thread>

using namespace std;
enum action_code
{
  waitRandom,
  wait,
  generateRandom,
  generateRandom2,
  loop10,
  defaultNum
};

class MinimalActionServerExample : public RosActionServer
{

public:
  explicit MinimalActionServerExample();
  void loopStopReq(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal);

  virtual std::map<std::string, std::string> getParam(const string &name) const;
  virtual void onStart(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal);
  virtual void onStop() //check
  {
    // stopReq = 1;
  }

  virtual void execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal) override;

  action_code hashit(std::string const &inString);

private:
  std::thread stopReqThread_;
  int stopReq;
  ros::ServiceClient srv_client_get;
  ros::ServiceClient srv_client_set;
};