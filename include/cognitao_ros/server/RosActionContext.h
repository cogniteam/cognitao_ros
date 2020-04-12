/**
 * @brief 
 * 
 * @file RosActionContext.h
 * 
 * @author ari yakir (ari@cogniteam.com)
 * @date 2020-04-16
 * @copyright Cogniteam (c) 2020
 * 
 * Cogniteam LTD CONFIDENTIAL
 *   
 * Unpublished Copyright (c) 2016-2017 Cogniteam,        All Rights Reserved.
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
#ifndef COGNITAO_ROS_ROSACTIONCONTEXT_H_
#define COGNITAO_ROS_ROSACTIONCONTEXT_H_

#include <cognitao_ros/RunnerAction.h>
#include <cognitao_ros/server/MultiGoalActionServer.h>


class RosActionContext {
public:    
    /**
     * @brief Construct a new Action Context object
     * @param goal 
     */
    RosActionContext  (
        const actionlib::MultiGoalActionServer<cognitao_ros::RunnerAction>::GoalHandle & goal,
        actionlib::MultiGoalActionServer<cognitao_ros::RunnerAction>* server)
    {
        server_ = server;
        goal_ = goal;
        for (auto const &param : goal.getGoal()->action.parameters){

            parameters_[param.key] = param.value;
            cout << "key" << param.key << endl;
            cout << "val" << param.value << endl;
        }
    }

    std::map<std::string, std::string> getParameters() const{
        return parameters_;
    }

    bool isPreemptRequested()
    {
        return server_->isPreemptRequested(goal_);
    }

    void setResult(bool value)
    {
        cognitao_ros::RunnerResult result;
        result.resultvalue  = value;
        server_->setSucceeded(goal_,result);
        cout << "[ goal " << value << " ]" << endl;
    }

private:
    /**
     * @brief a goal 
     */
    actionlib::MultiGoalActionServer<cognitao_ros::RunnerAction>::GoalHandle goal_;
    
    /**
     * @brief parameters from the goal
     */
    std::map<std::string, std::string> parameters_;
    
    actionlib::MultiGoalActionServer<cognitao_ros::RunnerAction> * server_;

};
#endif /* COGNITAO_ROS_ROSACTIONCONTEXT_H_ */