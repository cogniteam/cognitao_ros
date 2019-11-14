

#include <action_manager/ActionMsgAction.h>
#include <actionlib/server/simple_action_server.h>
using namespace std;

class MinimalActionServer {
protected:

    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<action_manager::ActionMsgAction> server;
public:
    MinimalActionServer() :server(nh_, "action_manager", boost::bind(&MinimalActionServer::execute, this, _1),false) 
    {
        std::cout<<"server is running.."<<std::endl;
        server.start(); //start the server running
    } 

    ~MinimalActionServer(void) {
    }
  
    virtual void execute(const actionlib::SimpleActionServer<action_manager::ActionMsgAction>::GoalConstPtr& goal){
     for(int i=0;i<20;i++){
        //std::cout<<"doing mission.."<<std::endl;
      }
        server.setSucceeded();
       cout<<"finished "<<endl;  
    }
};
