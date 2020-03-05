#include <cognitao_ros/server/MinimalActionServer.h>

MinimalActionServer::MinimalActionServer() : server(nh_, "cognitao_ros",
                                                    boost::bind(&MinimalActionServer::execute, this, _1), false)
{
    std::cout << "server is running.." << std::endl;
    server.start(); //start the server running
}

MinimalActionServer::~MinimalActionServer(void) {}

void MinimalActionServer::execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal)
{
    for (int i = 0; i < 20; i++)
    {
        //std::cout<<"doing mission.."<<std::endl;
    }
    server.setSucceeded(goal);
    cout << "finished " << endl;
}