/**
 * @brief ROS 1 cognitao runner 
 * 
 * @file RosRunner.h
 * 
 * @author Lin Azan (lin@cogniteam.com)
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


#ifndef COGNITAO_ROS_ROSRUNNER_H_
#define COGNITAO_ROS_ROSRUNNER_H_

#include <ros/xmlrpc_manager.h>
#include <actionlib/client/simple_action_client.h>

#include <cognitao_ros/RunnerAction.h> 
#include <cognitao/CogniTao.h>


using namespace std;

using actionFeedback = cognitao_ros::RunnerFeedbackConstPtr;
using actionType = cognitao_ros::RunnerAction;
using actionGoal = cognitao_ros::RunnerGoal;

typedef actionlib::SimpleActionClient<cognitao_ros::RunnerAction> CL;


/**
 * Allows to receive data, manage callback and publish da
 */
class RosRunner : public Runner{

public:

    RosRunner();
    ~RosRunner();
public:  

    virtual void setAction(const std::string &action) override;
    /**
     * @brief execute task
     * @return bool 
     */
    virtual bool run() override;

    /**
     * @brief Gets the Type
     * @return std::string 
     */
    virtual std::string getType() override;

    /**
     * @brief stop executing task 
     */
    virtual void stop() override;

private:

    void feedbackCallback(const actionFeedback &feedback);

    /**
     * @brief update result when task done  
     */
    void doneCallback(const actionlib::SimpleClientGoalState &state,
             const cognitao_ros::RunnerResultConstPtr &result);

    actionlib::SimpleActionClient<cognitao_ros::RunnerAction> *  client_;

    atomic<bool> stopRequested;
};


#endif /* COGNITAO_ROS_ROSRUNNER_H_ */