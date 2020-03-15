#include <cognitao_ros/server_example/MinimalActionServerExample.h>

MinimalActionServerExample::MinimalActionServerExample()
    : RosActionServer()
{
    ros::NodeHandle n_;
    srv_client_get = n_.serviceClient<cognitao_ros::getvar>("server_get_var");
    srv_client_set = n_.serviceClient<cognitao_ros::setvar>("server_set_var");
}

void MinimalActionServerExample::loopStopReq(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal)
{
    while (!server.isPreemptRequested(goal))
    {
        cout << "no preemption" << endl;
    }

    cognitao_ros::ActionMsgResult result;
    result.resultvalue = false;
    server.setPreempted(goal, result, "lin request");
    // stopReq = 1;
}

std::map<std::string, std::string> MinimalActionServerExample::getParam(const string &name) const
{
    return parameters;
}

void MinimalActionServerExample::onStart(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal)
{
    // stopReq=0;
    execute(goal);
}

void RosActionServer::onStop(){//stopReq=1;
}
void MinimalActionServerExample::execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal)
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
    bool cancelReq = false;
    result.resultvalue = false;
    for (auto const &param : goal.getGoal()->goal.parameters)
    {

        parameters[param.key] = param.value;
        cout << "loop" << endl;
        cout << "key" << param.key << endl;
        cout << "val" << param.value << endl;
    }

    switch (g)
    {

    case waitRandom:
        cout << "enter to wait random" << endl;
        srv.request.key = "random";
        if (srv_client_get.call(srv))
        {
            if (!server.isPreemptRequested(goal))
            {
                std::this_thread::sleep_for(std::chrono::seconds(stoi(srv.response.value)));
                cout << "sleep random for " << srv.response.value << " seconds!!";
            }
            else
            {
                cout << "STOP TASK_ CANCELATION" << endl;
            }
        }
        else
        {
            ROS_ERROR("ERROR");
        }
        // std::string rnd = WM::getVar("random");
        break;

    case wait:
        // if (!stopReq)
        {

            // cout << "server wait for a seconddddd" << endl;
            // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            cout << "--------------CASE WAIT---------------" << endl;
            cout << "time=======>" << parameters["time"].c_str() << endl;
            int t = atoi(parameters["time"].c_str());
            cout << "t is " << t << endl;
            cout << "WAITING TASK" << endl;
            for (int i = 0; i < t; i++)
            {
                cout << " executing wait " << i << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        break;

    case generateRandom:
    {
        // stopReqThread_ = std::thread(&MinimalActionServerExample::loopStopReq, this, goal);
        // stopReqThread_.detach();
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        srand(seed);
        int rnd = (rand() % 9) + 1;
        std::stringstream ss;
        ss << rnd;
        srvSet.request.key = "random";
        srvSet.request.value = ss.str();
        if (srv_client_set.call(srvSet))
        {
            if (!server.isPreemptRequested(goal))
            {

                // cout << "generate random to " << srvSet.request.value << " seconds!!";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            else
            {
                cout << "STOP TASK_ CANCELATION" << endl;
                cancelReq = true;
                break;
            }
        }
        else
        {
            // server.setPreempted(goal,result,"lin request");
            ROS_ERROR("Failed to call service add_two_ints");
        }
    }

    break;
    case generateRandom2:
    {
        // stopReqThread_ = std::thread(&MinimalActionServerExample::loopStopReq, this, goal);
        // stopReqThread_.detach();
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        srand(seed);
        int rnd = (rand() % 9) + 1;
        std::stringstream ss;
        ss << rnd;
        srvSet.request.key = "random";
        srvSet.request.value = ss.str();
        if (srv_client_set.call(srvSet))
        {

            for (int i = 0; i < 10; i++)
            {
                if (!server.isPreemptRequested(goal))
                {
                    cout << "check task action" << endl;
                    // cout << "generate random to " << srvSet.request.value << " seconds!!";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }

                else
                {
                    cout << "STOP TASK_ CANCELATION" << endl;
                    cancelReq = true;
                    break;
                }
            }
        }
        else
        {
            // server.setPreempted(goal,result,"lin request");
            ROS_ERROR("Failed to call service add_two_ints");
        }
    }
    break;
    case loop10:
    {
        std::stringstream ss;
        for (int i = 0; i < 10; i++)
        {
            // if (!stopReq)

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
        if (cancelReq)
        {
            server.setPreempted(goal, result, "1");

            cout << "DUE TO CANCEL REQ: " << endl;
            cout << "result value in Server: " << int(result.resultvalue) << endl;
        }
        else
        {
            std::cout << goal_ << " GOAL IS DONE!!" << std::endl;
            result.resultvalue = true;
            cout << "result value in Server: " << int(result.resultvalue) << endl;
            // std::cout<<"2.PREEMPT IS ::::: "<<server.isPreemptRequested(goal);

            server.setSucceeded(goal, result);
        }
    }

    cout << "ANSWER " << (int)result.resultvalue << endl;
}

action_code MinimalActionServerExample::hashit(std::string const &inString)
  {
    if (inString == "waitRandom")
      return waitRandom;
    if (inString == "wait")
      return wait;
    if (inString == "generateRandom")
      return generateRandom;
    if (inString == "generateRandom2")
      return generateRandom2;
    if (inString == "loop10")
      return loop10;

    return defaultNum;
  }
