/**
 * @brief 
 * 
 * @file MinimalActionServerExample.h
 * 
 * @author Lin Azar (lin@cogniteam.com)
 * @author Igor Makhtes (igor@cogniteam.com)
 * @date 2020-03-12
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


#include <cognitao_ros/server/MinimalActionServer.h>

#include "../../../devel/include/cognitao_ros/EventMsg.h"
#include "../../../devel/include/cognitao_ros/getvar.h"
#include "../../../devel/include/cognitao_ros/setvar.h"
// #include "ros/ros.h"
#include <thread>

using namespace std;
enum action_code
{
  waitRandom,
  wait,
  generateRandom,
  generateRandom2,
  loop10,
  defaultNum
};

class MinimalActionServerExample : public MinimalActionServer
{

public:
  explicit MinimalActionServerExample();
  void loopStopReq(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal);

  virtual std::map<std::string, std::string> getParam(const string &name) const;
  virtual void onStart(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal);
  virtual void onStop() //check
  {
    // stopReq = 1;
  }

  virtual void execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal) override;

  action_code hashit(std::string const &inString);

private:
  std::thread stopReqThread_;
  int stopReq;
  ros::ServiceClient srv_client_get;
  ros::ServiceClient srv_client_set;
};