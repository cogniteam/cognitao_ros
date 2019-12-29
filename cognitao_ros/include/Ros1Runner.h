#include <cognitao_ros/ActionMsgAction.h> // Note: "Action" is appended
#include <actionlib/client/simple_action_client.h>

#include <CogniTAO.h>

using namespace std;
using actionType = cognitao_ros::ActionMsgAction;
using actionFeedback = cognitao_ros::ActionMsgFeedbackConstPtr;
using actionType = cognitao_ros::ActionMsgAction;
using actionGoal = cognitao_ros::ActionMsgGoal;

typedef actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> CL;
class Ros1Runner : public Runner
{
public:
    Ros1Runner(string action, std::map<std::string, std::string> parameters) : Runner(action, parameters), client_("cognitao_ros", true)
    {
        action_ = action;
        paramMap = parameters;
    }

    void loopStopReq()
    {

        ros::Rate loop_rate(2);
        for (int i = 0; i < 10; i++)
        {
            loop_rate.sleep();
            cout << "in loop " << i << endl;
        }
        cout << "outttttttt of loop" << endl;
        stopRequested = true;
    }
    virtual bool run()
    {
        client_.waitForServer();

        actionGoal goalMsg;
        goalMsg.goal.actiontype = action_;
        for (auto const &x : paramMap)
        {
            cognitao_ros::KeyValue param;
            param.key = x.first;
            param.value = x.second;
            goalMsg.goal.parameters.push_back(param);
        }

        client_.sendGoal(goalMsg, boost::bind(&Ros1Runner::doneCb, this, _1, _2),
                         CL::SimpleActiveCallback(),
                         boost::bind(&Ros1Runner::feedbackCb, this, _1));

        stopReqThread_ = std::thread(&Ros1Runner::loopStopReq, this);
        stopReqThread_.detach();

        ros::Rate loop_rate(1);
        cognitao_ros::ActionMsgResultConstPtr res = client_.getResult();

        while (true)
        {
            bool goalDone = client_.waitForResult(ros::Duration(1));

            if (stopRequested == true)
            {
                cout << "finished ----> send cancelllll " << endl;
                client_.cancelGoal();
                ROS_INFO("goal is being canceled");
                break;
            }

            // if (goalDone){
            //     cout<<"finished ----> server SUCCESS "<<endl;
            //     break;
            // }

            loop_rate.sleep();
        }

        while (!existRes)
        {
        }
        cognitao_ros::ActionMsgResultConstPtr result = client_.getResult();

        if (result->resultvalue == false)
        {
            cout << "false " << endl;
            return false;
        }
        else if (result->resultvalue == true)
        {
            std::cout << "true" << endl;
            return true;
        }

        return result->resultvalue;
    }
    virtual std::string getType() { return "ROS1RUNNER"; };
    virtual void stop()
    {
        stopRequested = true;
    }

private:
    void feedbackCb(const actionFeedback &feedback)
    {
        cout << " get feedback " << endl;
    }
    void doneCb(const actionlib::SimpleClientGoalState &state, const cognitao_ros::ActionMsgResultConstPtr &result)
    {
        std::cout << "the result is :" << (int)result->resultvalue << std::endl;
        existRes = true;
    }
    actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> client_;
    std::thread stopReqThread_;
    bool existRes = false;
    atomic<bool> stopRequested;
    std::map<std::string, std::string> paramMap;
};