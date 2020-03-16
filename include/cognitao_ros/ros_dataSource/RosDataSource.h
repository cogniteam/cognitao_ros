
#include <cognitao/CogniTao.h>
#include <cognitao/data_sources/DataSource.h>

#include "../../../../../devel/include/cognitao_ros/EventMsg.h"
#include "ros/ros.h"
// #include <cognitao/signals/Signal.h>

using namespace std;

class RosDataSource : public DataSource
{

public:
    RosDataSource();

    virtual ~RosDataSource();

protected:
    virtual bool publishUpdateEvent(const string &name,
                                    const string &value) = 0;
    
    // void onDataSourceEvent();

    void onDataSourceEvent(const cognitao_ros::EventMsg &msg)
    {
        // WM::setVar(, msg.value);
        //  DataSource::variableUpdated(msg.key,msg.value);
    }
    void doSpin()
    {

        ros::spin();
    }

private:
    ros::Publisher event_pub_;

    ros::Subscriber event_sub_;

    std::thread spinTHread;
    ros::ServiceServer serviceGetVar;
    ros::ServiceServer serviceSetVar;
};
