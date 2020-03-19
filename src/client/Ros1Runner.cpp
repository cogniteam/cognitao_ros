/*
 * Ros1Runner.cpp
 * 
 * @author Lin Azan (lin@cogniteam.com)
 * @date 2020-03-15
 * @copyright Cogniteam (c) 2020
 *
 *
 * Cogniteam LTD CONFIDENTIAL
 *
 * Unpublished Copyright (c) 2010-2020 Cogniteam,        All Rights Reserved.
 *
 * NOTICE:  All information contained  herein  is,  and  remains the property
 * of Cogniteam.   The   intellectual   and   technical   concepts  contained
 * herein are proprietary to Cogniteam and may  be  covered  by  Israeli  and
 * Foreign Patents, patents in process,  and  are  protected  by trade secret
 * or copyright law. Dissemination of  this  information  or  reproduction of
 * this material is strictly forbidden unless  prior  written  permission  is
 * obtained  from  Cogniteam.  Access  to  the  source  code contained herein
 * is hereby   forbidden   to   anyone  except  current  Cogniteam employees,
 * managers   or   contractors   who   have   executed   Confidentiality  and
 * Non-disclosure    agreements    explicitly    covering     such     access
 *
 * The copyright notice  above  does  not  evidence  any  actual  or intended
 * publication  or  disclosure    of    this  source  code,   which  includes
 * information that is confidential  and/or  proprietary,  and  is  a   trade
 * secret, of   Cogniteam.    ANY REPRODUCTION,  MODIFICATION,  DISTRIBUTION,
 * PUBLIC   PERFORMANCE,  OR  PUBLIC  DISPLAY  OF  OR  THROUGH USE   OF  THIS
 * SOURCE  CODE   WITHOUT   THE  EXPRESS  WRITTEN  CONSENT  OF  Cogniteam  IS
 * STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL
 * TREATIES.  THE RECEIPT OR POSSESSION OF  THIS SOURCE  CODE  AND/OR RELATED
 * INFORMATION DOES  NOT CONVEY OR IMPLY ANY RIGHTS  TO  REPRODUCE,  DISCLOSE
 * OR  DISTRIBUTE ITS CONTENTS, OR TO  MANUFACTURE,  USE,  OR  SELL  ANYTHING
 * THAT      IT     MAY     DESCRIBE,     IN     WHOLE     OR     IN     PART
 *
 */


#include <cognitao_ros/client/Ros1Runner.h>

// namespace cognitao_ros {
// namespace runner {


Ros1Runner::Ros1Runner() : client_("cognitao_ros", true){

    // ros::NodeHandle n;
    // ros::Subscriber sub = n.subscribe("stopMe", 1000, stopCallback);
    if (!ros::isInitialized()) {
        int n = 0;
		ros::init(n, nullptr, "cognitao_ros", ros::init_options::AnonymousName);
	}

    stopRequested = false;
    success = false;
}

Ros1Runner::Ros1Runner(const string &action,
                    std::map<std::string, std::string> parameters):
                    Runner(action, parameters), client_("cognitao_ros", true){

    if (!ros::isInitialized()) {
        int n = 0;
		ros::init(n, nullptr, "cognitao_ros", ros::init_options::AnonymousName);
	}                    
    stopRequested = false;
    success = false;
}

bool Ros1Runner::run(){
    
    stopRequested = false;
    success = false;

    
    cout << " DO ACTION _____ " << action_ << endl;
    client_.waitForServer();

    actionGoal goalMsg;
    goalMsg.goal.actiontype = action_;

    for (auto const &x : parameters_){

        cognitao_ros::KeyValue param;
        param.key = x.first;
        param.value = x.second;
        
        goalMsg.goal.parameters.push_back(param);
    }

    client_.sendGoal(goalMsg, boost::bind(&Ros1Runner::doneCallback, this, _1, _2),
                     CL::SimpleActiveCallback(),
                     boost::bind(&Ros1Runner::feedbackCb, this, _1));

    // stopReqThread_ = std::thread(&Ros1Runner::loopStopReq, this);
    // stopReqThread_.detach();

    ros::Rate loop_rate(1);
    cognitao_ros::ActionMsgResultConstPtr res = client_.getResult();

    while (true){ // check if someone ask to stop the task while the server do the action
    
        bool goalDone = client_.waitForResult(ros::Duration(1));

        if (stopRequested == true) // task end or someone stop the task
        {
            if (!resultExist)
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

    while (!resultExist)
    {
    }
    cognitao_ros::ActionMsgResultConstPtr result = client_.getResult();

    if (result->resultvalue == false){
        cout << "false " << endl;
        return false;
    }

    else if (result->resultvalue == true){
        std::cout << "true" << endl;
        success = true;
        return true;
    }

    return result->resultvalue;
}

std::string Ros1Runner::getType() { 
    return "ros1";
     };

void Ros1Runner::stop(){
   
    stopRequested = true;
}

void Ros1Runner::feedbackCb(const actionFeedback &feedback){
    
    cout << " get feedback " << endl;
}

void Ros1Runner::doneCallback(const actionlib::SimpleClientGoalState &state, const cognitao_ros::ActionMsgResultConstPtr &result){
    
    std::cout << "the result is :" << (int)result->resultvalue << std::endl;
    resultExist = true;
    stopRequested = true; // task completed- stop request to end run() function and return the result
}

// }
// }