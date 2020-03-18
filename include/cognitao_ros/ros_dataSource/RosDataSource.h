
#include <cognitao/CogniTao.h>
#include <cognitao/data_sources/DataSource.h>

#include "../../../../../devel/include/cognitao_ros/EventMsg.h"
#include "ros/ros.h"
// #include <cognitao/signals/Signal.h>

using namespace std;

class RosDataSource : public DataSource
{

public:
    static DataSource::Ptr create()
    {
        return DataSource::Ptr(new RosDataSource());
    }
    RosDataSource();

    virtual ~RosDataSource();

protected:
    virtual bool publishUpdateEvent(const string &name,
                                    const string &value);

    // void onDataSourceEvent();

    void onDataSourceEvent(const cognitao_ros::EventMsg &msg);

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
