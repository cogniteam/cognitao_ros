/**
 * @brief ROS 1 cognitao runner
 * 
 * @file Ros1Runner.cpp
 * 
 * @author Igor Makhtes (igor@cogniteam.com)
 * @date 2020-03-15
 * @copyright Cogniteam (c) 2020
 *    
 * MIT License
 *   
 * Permission is hereby granted, free of charge, to any person obtaining a  copy
 * of this software and associated documentation files (the 'Software'), to deal
 * in the Software without restriction, including without  limitation the rights
 * to use, copy, modify, merge,  publish,  distribute,  sublicense,  and/or sell
 * copies of the Software, and  to  permit  persons  to  whom  the  Software  is 
 * furnished to do so, subject to the following conditions:
 *   
 * The   above   copyright   notice   and  this   permission   notice  shall  be
 * included in all copies or substantial portions of the Software.
 *   
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
 * IMPLIED, INCLUDING BUT NOT LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN  NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE  LIABLE  FOR  ANY  CLAIM,  DAMAGES  OR  OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */


#include <cognitao_ros/client/Ros1Runner.h>


namespace cognitao_ros {
namespace runner {


Ros1Runner::Ros1Runner() 
    : client_("cognitao_ros", true) {

	if (!ros::isInitialized()) {
        int n = 0;
		ros::init(n, nullptr, "cognitao_ros", ros::init_options::AnonymousName);
	}

    stopRequested_ = false;
    success_ = false;
}

Ros1Runner::Ros1Runner(const string &action, 
        const std::map<std::string, std::string>& parameters) 
    : Runner(action, parameters), client_("cognitao_ros", true) {

    stopRequested_ = false;
    success_ = false;
}

bool Ros1Runner::run() {

    cout<<"lin azan"<<endl;
    stopRequested_ = false;
    success_ = false;

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

    client_.sendGoal(goalMsg, boost::bind(
            &Ros1Runner::doneCallback, this, _1, _2),
            CL::SimpleActiveCallback(),
            boost::bind(&Ros1Runner::feedbackCallback, this, _1));

    // stopReqThread_ = std::thread(&Ros1Runner::loopStopReq, this);
    // stopReqThread_.detach();

    ros::Rate loop_rate(1);
    cognitao_ros::ActionMsgResultConstPtr res = client_.getResult();

    while (true) // check if someone ask to stop the task while the server do the action
    {
        bool goalDone = client_.waitForResult(ros::Duration(1)); 

        if (stopRequested_ == true) { // task end or someone stop the task
        
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

    while (!existRes) {
        // sleep 1
    }

    cognitao_ros::ActionMsgResultConstPtr result = client_.getResult();

    if (result->resultvalue == false) {

        cout << "false " << endl;
        return false;

    } else if (result->resultvalue == true) {

        std::cout << "true" << endl;
        success_ = true;
        return true;
        
    }

    return result->resultvalue;
}

std::string Ros1Runner::getType() { 
    return "ros"; 
}

void Ros1Runner::stop() {
    stopRequested_ = true;
}

void Ros1Runner::feedbackCallback(const actionFeedback &feedback) {
    cout << " get feedback " << endl;
}

void Ros1Runner::doneCallback(const actionlib::SimpleClientGoalState &state, 
        const cognitao_ros::ActionMsgResultConstPtr &result) {

    std::cout << "the result is :" << (int)result->resultvalue << std::endl;
    existRes = true;
    stopRequested_ = true; // task completed- stop request to end run() function and return the result

}


} /* namespace runners */
} /* namespace cognitao_ros */