#include <cognitao_ros/client/Ros1Runner.h>

// void h_sig_sigint(int signum)
// {
//     std::cout << "Receive signum: " << signum << std::endl;
//     Ros1Runner::stop();
// }
Ros1Runner::Ros1Runner() : client_("cognitao_ros", true)
{
    // ros::NodeHandle n;
    // ros::Subscriber sub = n.subscribe("stopMe", 1000, stopCallback);
    stopRequested = false;
    success = false;
}
Ros1Runner::Ros1Runner(const string &action, std::map<std::string, std::string> parameters) : Runner(action, parameters), client_("cognitao_ros", true)
{
    stopRequested = false;
    success = false;
}

bool Ros1Runner::run()
{
    
    stopRequested = false;
    success = false;

    // signal(SIGINT, h_sig_sigint);
    cout << " DO ACTION _____ " << action_ << endl;
    client_.waitForServer();

    actionGoal goalMsg;
    goalMsg.goal.actiontype = action_;
    for (auto const &x : parameters_)
    {
        cognitao_ros::KeyValue param;
        param.key = x.first;
        param.value = x.second;
        goalMsg.goal.parameters.push_back(param);
    }

    client_.sendGoal(goalMsg, boost::bind(&Ros1Runner::doneCb, this, _1, _2),
                     CL::SimpleActiveCallback(),
                     boost::bind(&Ros1Runner::feedbackCb, this, _1));

    // stopReqThread_ = std::thread(&Ros1Runner::loopStopReq, this);
    // stopReqThread_.detach();

    ros::Rate loop_rate(1);
    cognitao_ros::ActionMsgResultConstPtr res = client_.getResult();

    while (true) // check if someone ask to stop the task while the server do the action
    {
        bool goalDone = client_.waitForResult(ros::Duration(1));

        if (stopRequested == true) // task end or someone stop the task
        {
            if (!existRes)
            {
                client_.cancelGoal();
                ROS_INFO("goal is being canceled");
                cout << "goal is being canceled " << endl;
            }
            else
            {
                cout << "goal DONE!!" << endl;
            }
            break;
        }

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
        success = true;
        return true;
    }

    return result->resultvalue;
}
std::string Ros1Runner::getType() { return "ros1"; };
void Ros1Runner::stop()
{
    stopRequested = true;
}

void Ros1Runner::feedbackCb(const actionFeedback &feedback)
{
    cout << " get feedback " << endl;
}
void Ros1Runner::doneCb(const actionlib::SimpleClientGoalState &state, const cognitao_ros::ActionMsgResultConstPtr &result)
{
    std::cout << "the result is :" << (int)result->resultvalue << std::endl;
    existRes = true;
    stopRequested = true; // task completed- stop request to end run() function and return the result
}