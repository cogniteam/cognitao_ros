/*
 * RosActionServer.h
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


#ifndef COGNITAO_ROS_ROSACTIONSERVER_H_
#define COGNITAO_ROS_ROSACTIONSERVER_H_


#include <string>
#include <iostream>
#include <thread>

#include <cognitao_ros/ActionMsgAction.h>
#include <cognitao_ros/server/MultiGoalActionServer.h>

using namespace std;

/**
 * manage client's requests
 */
class RosActionServer {

public:

  RosActionServer();

  ~RosActionServer(void);

public:

  virtual void onStart(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal) = 0;

  virtual void onStop() = 0;

  virtual std::map<std::string, std::string> getParam(const string &name) const = 0;

  virtual void execute(const actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction>::GoalHandle &goal);
 
 protected:

  ros::NodeHandle nh_;

  actionlib::MultiGoalActionServer<cognitao_ros::ActionMsgAction> server;

  std::map<std::string, std::string> parameters;
};


#endif 
