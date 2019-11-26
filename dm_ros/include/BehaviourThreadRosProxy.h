
/////////////////////////////////////////////////////////BehaviourThreadRosProxy


#include <action_manager/ActionMsgAction.h> // Note: "Action" is appended
#include <actionlib/client/simple_action_client.h>

#include <CogniTAO.h>

using namespace std;
using actionType=action_manager::ActionMsgAction;
typedef actionlib::SimpleActionClient<action_manager::ActionMsgAction> CL;

class BehaviourThreadRosProxy:  public BehaviourThread{

public:
    BehaviourThreadRosProxy(string name):BehaviourThread(name), client_("action_manager", true){
        actionType_ =  name;

    }

    virtual void onStart() override {
  
        client_.waitForServer();

        actionGoal goal;
        goal.actiontype=actionType_;
        
        client_.sendGoal(goal,boost::bind(&BehaviourThreadRosProxy::doneCb, this, _1, _2),
                CL::SimpleActiveCallback(),
            boost::bind(&BehaviourThreadRosProxy::feedbackCb, this, _1));
        
        BehaviourThread::onStart(); 


        // stop req
        stopReqThread_ = std::thread(&BehaviourThreadRosProxy::loopStopReq, this);
	    stopReqThread_.detach();
    }

    void loopStopReq() {
     
        ros::Rate loop_rate(2);
        for (int i = 0 ; i < 10; i++ ){
            loop_rate.sleep();
            cout<<" inside thread "<<i<<stopRequested<<" "<<actionType_<<endl;
        }
        stopRequested = true;
        cout<<" outside thread "<<stopRequested<<" "<<actionType_<<endl;
         
     }

    virtual bool action() override {
        
        ros::Rate loop_rate(1);
        action_manager::ActionMsgResultConstPtr res =client_.getResult();


        cout<<"bbbbbbbbbb->"<<&res<<res->resultvalue<<"end"<<endl;

                  if (res->resultvalue == false){
            cout<<"false "<<endl;
         
           
            }
             else if (res->resultvalue == true){
                std::cout<<"true"<<endl;
               
            }
        while (true) {
            bool goalDone=client_.waitForResult(ros::Duration(1));
            cout<<" stopRequested lin "<<actionType_<<"--"<<stopRequested<<endl;

            if (stopRequested == true){
                cout<<"finished ----> send cancelllll "<<endl;
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
        
        while( !serverRes) {

        } 
        action_manager::ActionMsgResultConstPtr result =client_.getResult();

        cout<<"rrrrrrr->"<<&result<<result->resultvalue<<"end"<<endl;
        if (result->resultvalue == false){
          cout<<"false "<<endl;
          return false;
         
          }
           else if (result->resultvalue == true){
              std::cout<<"true"<<endl;
              return true;
          }
          
        return result->resultvalue;
    }

    private:

    void feedbackCb(const actionFeedback& feedback) {
        cout<<" get feedback "<<endl; 
    }   
    void doneCb(const actionlib::SimpleClientGoalState& state,const action_manager::ActionMsgResultConstPtr& result) {
        std::cout<<"the result is :" <<(int)result->resultvalue<<std::endl;
        serverRes = true;
    }   
    string actionType_;
    actionlib::SimpleActionClient<action_manager::ActionMsgAction> client_;
    std::thread stopReqThread_;
     bool serverRes= false;

};