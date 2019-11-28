#include <action_manager/ActionMsgAction.h>
#include <actionlib/client/simple_action_client.h>

#include <CogniTAO.h>

using actionType=action_manager::ActionMsgAction;
using actionGoal=action_manager::ActionMsgGoal;
using actionFeedback=action_manager::ActionMsgFeedbackConstPtr;
typedef actionlib::SimpleActionClient<action_manager::ActionMsgAction> CL;

class StateThreadRosProxy: public StateThread {
public:

    StateThreadRosProxy(string name):StateThread(name), client_("action_manager", true){	
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
        
        client_.sendGoal(goal,boost::bind(&StateThreadRosProxy::doneCb, this, _1, _2),
                 CL::SimpleActiveCallback(),
               boost::bind(&StateThreadRosProxy::feedbackCb, this, _1));
         //    client_.sendGoal(goal);

        StateThread::onStart();          
	
	}

    virtual void run() override{
        ros::Rate loop_rate(1);
        int count = 0;
        while(!stopRequested) {
            count++;
			std::cout<<count<<" stopRequested is "<<stopRequested<<endl;;
            loop_rate.sleep();
		}
        cout<<" stopRequested "<<endl;

    }
        virtual void onStop() override{

        StateThread::onStop();
        client_.cancelGoal();
        ROS_INFO("goal is being canceled");
        
    }

    private:
    string actionType_;
    actionlib::SimpleActionClient<action_manager::ActionMsgAction> client_;
};