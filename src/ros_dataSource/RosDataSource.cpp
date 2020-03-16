#include <cognitao_ros/ros_dataSource/RosDataSource.h>

RosDataSource::RosDataSource()
{
    if (!ros::isInitialized())
    {
        int n = 0;
        ros::init(n, nullptr, "cognitao_ros1");
    }
    ros::NodeHandle n_;

    event_sub_ = n_.subscribe("/wme/in", 1000,
                              &RosDataSource::onDataSourceEvent, this);

    event_pub_ = n_.advertise<cognitao_ros::EventMsg>("/wme/out", 1000);
    // serviceGetVar = n_.advertiseService("server_get_var", &RosDataSource::serverGetVar, this);
    // serviceSetVar = n_.advertiseService("server_set_var", &RosDataSource::serverSetVar, this);
    spinTHread = std::thread(&RosDataSource::doSpin, this);
    spinTHread.detach();
}

RosDataSource::~RosDataSource()
{
    spinTHread.join();
}

bool RosDataSource::publishUpdateEvent(const string &name, const string &value)
{
    cognitao_ros::EventMsg msg;

    msg.key = name;
    msg.value = value;
    cout << "publish msg " << name << ", " << value << endl;
    event_pub_.publish(msg);
}
// void RosDataSource::onDataSourceEvent(){

// }