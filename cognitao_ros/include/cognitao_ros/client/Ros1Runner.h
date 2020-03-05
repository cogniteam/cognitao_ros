#include <cognitao_ros/ActionMsgAction.h> // Note: "Action" is appended
#include <actionlib/client/simple_action_client.h>

#include <CogniTao.h>

using namespace std;
using actionType = cognitao_ros::ActionMsgAction;
using actionFeedback = cognitao_ros::ActionMsgFeedbackConstPtr;
using actionType = cognitao_ros::ActionMsgAction;
using actionGoal = cognitao_ros::ActionMsgGoal;

typedef actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> CL;
class Ros1Runner : public Runner
{
public:
    Ros1Runner();
    Ros1Runner(const string &action, std::map<std::string, std::string> parameters);
    virtual bool run() override;
    virtual std::string getType();
    virtual void stop();

private:
    void feedbackCb(const actionFeedback &feedback);
    void doneCb(const actionlib::SimpleClientGoalState &state, const cognitao_ros::ActionMsgResultConstPtr &result);
    actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> client_;
    bool existRes = false;

    atomic<bool> stopRequested;
    bool success;
};