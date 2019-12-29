#include <cognitao_ros/ActionMsgAction.h> // Note: "Action" is appended
#include <actionlib/client/simple_action_client.h>

#include <CogniTAO.h>

using namespace std;
using actionType=cognitao_ros::ActionMsgAction;
typedef actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> CL;

class BehaviourRosProxy:  public Behaviour {

public:
    BehaviourRosProxy(string name):Behaviour(name), client_("cognitao_ros", true){
        actionType_ =  name;
    }

    virtual void onStart() override {
    client_.waitForServer();
    actionGoal goal;
    goal.actiontype=actionType_;
    
    client_.sendGoal(goal,boost::bind(&BehaviourRosProxy::doneCb, this, _1, _2),
             CL::SimpleActiveCallback(),
           boost::bind(&BehaviourRosProxy::feedbackCb, this, _1));
    
    Behaviour::onStart();      
	
	}



    virtual bool action() override {
        client_.waitForResult();
        cognitao_ros::ActionMsgResultConstPtr result =client_.getResult();
        return result->resultvalue;
    }

    
private:

    void feedbackCb(const actionFeedback& feedback) {
        //cout<<" get feedback "<<endl; 
    }   
    void doneCb(const actionlib::SimpleClientGoalState& state,const cognitao_ros::ActionMsgResultConstPtr& result) {
        //std::cout<<"the result is :" <<(int)result->resultvalue<<std::endl;
    }   
    string actionType_;
    actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> client_;
};