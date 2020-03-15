/**
 * @brief ROS 1 cognitao runner 
 * 
 * @file Ros1Runner.h
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


#ifndef COGNITAO_ROS_ROS1RUNNER_H_
#define COGNITAO_ROS_ROS1RUNNER_H_


#include <actionlib/client/simple_action_client.h>

#include <cognitao/CogniTao.h>
#include <cognitao_ros/ActionMsgAction.h> // Note: "Action" is appended


using namespace std;

using actionType = cognitao_ros::ActionMsgAction;
using actionFeedback = cognitao_ros::ActionMsgFeedbackConstPtr;
using actionType = cognitao_ros::ActionMsgAction;
using actionGoal = cognitao_ros::ActionMsgGoal;

typedef actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> CL;


namespace cognitao_ros {
namespace runner {


/**
 * @brief 
 */
class Ros1Runner : public Runner {

public:

    Ros1Runner();

    Ros1Runner(const string &action, 
            const std::map<std::string, std::string>& parameters);

public:

    /**
     * @brief 
     * @return true 
     * @return false 
     */
    virtual bool run() override;

    /**
     * @brief Gets the Type
     * @return std::string 
     */
    virtual std::string getType();

    /**
     * @brief 
     */
    virtual void stop();

private:

    void feedbackCallback(const actionFeedback &feedback);

    void doneCallback(const actionlib::SimpleClientGoalState &state, 
            const cognitao_ros::ActionMsgResultConstPtr &result);

private:

    actionlib::SimpleActionClient<cognitao_ros::ActionMsgAction> client_;

    bool existRes_ = false;

    atomic<bool> stopRequested_;

    bool success_;

};


} /* namespace runners */
} /* namespace cognitao_ros */
                             

#endif /* COGNITAO_ROS_ROS1RUNNER_H_ */