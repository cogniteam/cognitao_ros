/*
 * RosRunner.cpp
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


#include <cognitao/Exception.h>
#include <cognitao_ros/runner/RosRunner.h>


RosRunner::RosRunner(){

    if (!ros::isInitialized()) {
        int n = 0;
		ros::init(n, nullptr, "cognitao_ros", ros::init_options::AnonymousName | ros::init_options::NoSigintHandler);
	}

    stopRequested = false;
}

RosRunner::~RosRunner()
{
    if (client_!= nullptr)
    {
        delete client_;
    }
}

void RosRunner::setAction(const std::string &action){
  client_ = new actionlib::SimpleActionClient<cognitao_ros::RunnerAction>(action, true);
}

bool RosRunner::run(){
    
    stopRequested = false;

    if(!client_->waitForServer(ros::Duration(1, 0)))
    {
        throw CogniTAOException("Remote actionlib task cannot be executed, Server timeout");
    }

    actionGoal goalMsg;
    
    for (auto const &x : parameters_){

        cognitao_ros::KeyValue param;
        param.key = x.first;
        param.value = x.second;
        goalMsg.action.parameters.push_back(param);
    }

    client_->sendGoal(goalMsg, boost::bind(&RosRunner::doneCallback, this, _1, _2),
                     CL::SimpleActiveCallback(),
                     boost::bind(&RosRunner::feedbackCallback, this, _1));

    ros::Rate loop_rate(1);
    while (ros::ok() && // CTRL^C
           !stopRequested && // By the runner manager
           !client_->waitForResult(ros::Duration(0.1)) // no result available, waiting
          )
    { 
        //SERVER CRASHED!
        if(!client_->isServerConnected())
        { 
            throw CogniTAOException("Remote actionlib task cannot be executed " + this->getAction() + " server is not connected");
        }
    }
    //cout<< "done waiting" <<endl;
    
    
    // was asked to stop but did not finish
    // remote task execution, need to cancel it
    if(stopRequested) 
    {
        if(client_->isServerConnected())
        { 
            client_->cancelGoal();
            ROS_INFO("goal is being canceled");
            cout << "goal is being canceled " << endl;
        }
        return false;
    }
    cognitao_ros::RunnerResultConstPtr result = client_->getResult();
    //cout<< "returing result " << result->resultvalue <<endl;
    return result->resultvalue;
}

std::string RosRunner::getType() { 
    return "ros";
};

void RosRunner::stop(){
   
    stopRequested = true;
}

void RosRunner::feedbackCallback(const actionFeedback &feedback){
    
    //cout << " get feedback " << endl;
}

void RosRunner::doneCallback(const actionlib::SimpleClientGoalState &state, const cognitao_ros::RunnerResultConstPtr &result){
    
    //std::cout << "the result is :" << (int)result->resultvalue << std::endl;
}
