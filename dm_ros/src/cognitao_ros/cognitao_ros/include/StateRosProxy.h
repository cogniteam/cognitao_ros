#include <action_manager/ActionMsgAction.h> // Note: "Action" is appended
#include <actionlib/client/simple_action_client.h>

#include <CogniTAO.h>

using actionType=action_manager::ActionMsgAction;
using actionGoal=action_manager::ActionMsgGoal;
using actionFeedback=action_manager::ActionMsgFeedbackConstPtr;
typedef actionlib::SimpleActionClient<action_manager::ActionMsgAction> CL;

class StateRosProxy:  public State {

public:

    StateRosProxy(string name):State(name), client_("action_manager", true){	
        actionType_ =  name;          
    }
    void feedbackCb(const actionFeedback& feedback) {
    cout<<" get feedback "<<endl; 
    }

    void doneCb(const actionlib::SimpleClientGoalState& state,const action_manager::ActionMsgResultConstPtr& result) {
    std::cout<<"the result is :" <<result->resultvalue<<std::endl;
    }

    virtual void onStart() override {
        client_.waitForServer();
        actionGoal goal;
        goal.actiontype=actionType_;
        
        client_.sendGoal(goal,boost::bind(&StateRosProxy::doneCb, this, _1, _2),
                 CL::SimpleActiveCallback(),
               boost::bind(&StateRosProxy::feedbackCb, this, _1));
         //    client_.sendGoal(goal);
        run();            
	
	}

    virtual void run() override{

        client_.waitForResult();
            
        if (client_.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        printf("action complete!");

        printf("Current State: %s\n", client_.getState().toString().c_str());
            
	}

private:
    string actionType_;
    actionlib::SimpleActionClient<action_manager::ActionMsgAction> client_;
};

