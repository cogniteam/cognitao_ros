#include <MinimalActionServer.h>
using namespace std;
enum action_code {
    DriveForward_FORVER,
    DriveBackward_FORVER,
    DriveBackward_With_Timer,
    DriveForward_With_Timer,
    defaultNum
};

class MinimalActionServerExample: public MinimalActionServer {

public:

  explicit MinimalActionServerExample()
    :MinimalActionServer(){
    }

virtual void execute(const actionlib::SimpleActionServer<action_manager::ActionMsgAction>::GoalConstPtr& goal) override{
const string goal_ = goal->actiontype;
bool returnValue= true;
ros::Rate loop_rate(1);
action_code g=hashit(goal_);
action_manager::ActionMsgResult result;
action_manager::ActionMsgFeedback feedback;

switch (g)
{
case DriveForward_FORVER :
    for(;;){
          cout<<" im driving foraward now"<<endl;
          server.publishFeedback(feedback);
          loop_rate.sleep();
          if (server.isPreemptRequested()) {
      
            cout<<"DriveForward_FORVER --->Goal Canceled "<<endl;
            server.setAborted();
            return;
          }
        }
        break;

case DriveBackward_FORVER:
        
        for(;;){
          cout<<" im driving backward now"<<endl;
          server.publishFeedback(feedback);
          loop_rate.sleep();
          
          if (server.isPreemptRequested()) {
      
            cout<<"DriveForward_FORVER --->Goal Canceled "<<endl;
            server.setAborted();
            return;
          }
        }
        break;
      case DriveBackward_With_Timer:
        
        for(int i = 0; i < 20; i++){
          cout<<" im driving backward WITH_TIMER now"<<i<<endl;
          server.publishFeedback(feedback);
          loop_rate.sleep();
          if (server.isPreemptRequested()) {
      
            cout<<"DriveForward_FORVER --->Goal Canceled "<<endl;
            server.setAborted();
            return;
          }
        }
        // set return value for BehaviourRosProxy
        break; 
         

       case DriveForward_With_Timer:
        
        for(int i = 0; i < 20; i++){
          cout<<" im driving forward WITH_TIMER now"<<i<<endl;
          server.publishFeedback(feedback);
          loop_rate.sleep();
          if (server.isPreemptRequested()) {
      
            cout<<"DriveForward_FORVER --->Goal Canceled "<<endl;
            server.setAborted();
            return;
          }
        }
        // set return value for BehaviourRosProxy
        break; 
         

default:
cout<<"nothing "<<endl;
    break;
}
if (ros::ok()){
    std::cout<<" GOAL IS DONE!!"<<std::endl;
    result.resultvalue = returnValue;
    cout<<"result value in Server: "<<result.resultvalue<<endl;

    server.setSucceeded(result);
}
}

action_code hashit (std::string const& inString) {
    if (inString == "DriveForward_FORVER") return DriveForward_FORVER;
    if (inString == "DriveBackward_FORVER") return DriveBackward_FORVER;
    if (inString == "DriveBackward_With_Timer") return DriveBackward_With_Timer;
    if (inString == "DriveForward_With_Timer") return DriveForward_With_Timer;
  }
  private: 
  
};