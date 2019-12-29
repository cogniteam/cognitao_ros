#include <MinimalActionServer.h>

#include <thread>

using namespace std;
enum action_code
{
  DriveForward_FORVER,
  DriveBackward_FORVER,
  DriveBackward_With_Timer,
  DriveForward_With_Timer,
  defaultNum
};

class MinimalActionServerExample : public MinimalActionServer
{

public:
  explicit MinimalActionServerExample()
      : MinimalActionServer()
  {
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
    for (auto const &param : goal.getGoal()->goal.parameters)
    {

      parameters[param.key] = param.value;
    }

    switch (g)
    {

    case DriveForward_FORVER:
      for (;;)
      {
        cout << " im driving foraward now" << endl;
        // server.publishFeedback(feedback);
        loop_rate.sleep();
        if (server.isPreemptRequested(goal))
        {

          cout << "DriveForward_FORVER --->Goal Canceled " << endl;
          result.resultvalue = false;
          if (result.resultvalue == false)
          {
            cout << "false " << endl;
          }
          else if (result.resultvalue == true)
          {
            cout << "true " << endl;
          }
          std::cout << "realtime " << &result << (int)result.resultvalue << endl;
          server.setAborted(goal, result);
          //   if (ros::ok()){
          //     std::cout<<" GOAL IS DONE!!"<<std::endl;
          //     result.resultvalue = returnValue;
          //     cout<<"result value in Server: "<<int(result.resultvalue)<<endl;

          //     server.setSucceeded(goal,result);
          //   }
          return;
        }
      }
      break;

    case DriveBackward_FORVER:

      for (;;)
      {
        cout << " im driving backward now" << endl;
        // server.publishFeedback(feedback);
        loop_rate.sleep();

        if (server.isPreemptRequested(goal))
        {

          result.resultvalue = false;
          server.setAborted(goal, result, "1");

          std::cout << "PREEMPT IS::::: " << server.isPreemptRequested(goal);

          return;
        }
      }
      break;
    case DriveForward_With_Timer:

      for (int i = 0; i < 20; i++)
      {
        cout << " im driving forward WITH_TIMER now" << i << endl;
        // server.publishFeedback(feedback);
        loop_rate.sleep();
        if (server.isPreemptRequested(goal))
        {

          cout << "DriveForward_FORVER --->Goal Canceled " << endl;
          result.resultvalue = false;
          server.setAborted(goal, result);

          return;
        }
      }
      // set return value for BehaviourRosProxy
      break;

    case DriveBackward_With_Timer:

      for (int i = 0; i < 20; i++)
      {
        cout << " im driving backward WITH_TIMER now" << i << endl;
        // server.publishFeedback(feedback);
        loop_rate.sleep();
        if (server.isPreemptRequested(goal))
        {

          cout << "DriveForward_FORVER --->Goal Canceled " << endl;
          result.resultvalue = returnValue;
          server.setAborted(goal, result);
          return;
        }
      }
      // set return value for BehaviourRosProxy
      break;

    default:
      cout << "nothing " << endl;
      break;
    }
    cout << "im hereeee" << std::endl;
    if (ros::ok())
    {
      std::cout << " GOAL IS DONE!!" << std::endl;
      result.resultvalue = true;
      cout << "result value in Server: " << int(result.resultvalue) << endl;
      // std::cout<<"2.PREEMPT IS ::::: "<<server.isPreemptRequested(goal);

      server.setSucceeded(goal, result);
    }
  }

  action_code hashit(std::string const &inString)
  {
    if (inString == "DriveForward_FORVER")
      return DriveForward_FORVER;
    if (inString == "DriveBackward_FORVER")
      return DriveBackward_FORVER;
    if (inString == "DriveBackward_With_Timer")
      return DriveBackward_With_Timer;
    if (inString == "DriveForward_With_Timer")
      return DriveForward_With_Timer;
  }

private:
  std::thread stopReqThread_;
  std::map<std::string, std::string> parameters;
};