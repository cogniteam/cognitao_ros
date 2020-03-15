#include <cognitao_ros/server/RosActionServer.h>

RosActionServer::RosActionServer() : server(nh_, "cognitao_ros",
                                                    boost::bind(&RosActionServer::execute, this, _1), false)
{
    std::cout << "server is running.." << std::endl;
    server.start(); //start the server running
}

RosActionServer::~RosActionServer(void) {}

void RosActionServer::execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal)
{
    for (int i = 0; i < 20; i++)
    {
        //std::cout<<"doing mission.."<<std::endl;
    }
    server.setSucceeded(goal);
    cout << "finished " << endl;
}