/*
 * RosDataSource.h
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


#ifndef COGNITAO_ROS_ROSDATASOURCE_H_
#define COGNITAO_ROS_ROSDATASOURCE_H_


#include "ros/ros.h"

#include <cognitao/CogniTao.h>
#include <cognitao/data_sources/DataSource.h>
#include "../../../../../devel/include/cognitao_ros/EventMsg.h"


using namespace std;

/**
 * manage the WM
 */
class RosDataSource : public DataSource{

public:

    RosDataSource();

    virtual ~RosDataSource();

public:

    static DataSource::Ptr create(){
        return DataSource::Ptr(new RosDataSource());
    }

protected:

    /**
     * @brief publish events
     * @return bool
     */
    virtual bool publishUpdateEvent(const string &name,
                                    const string &value);

    /**
     * @brief updates the world model
     * @return bool
     */
    void onDataSourceEvent(const cognitao_ros::EventMsg &msg);

    void doSpin();

private:

    ros::Publisher event_pub_;

    ros::Subscriber event_sub_;

    std::thread spinTHread;

    ros::ServiceServer serviceGetVar;

    ros::ServiceServer serviceSetVar;
};


#endif /* COGNITAO_ROS_ROSDATASOURCE_H_ */
