#include <cognitao_ros/ros_dataSource/RosDataSource.h>

RosDataSource::RosDataSource()
{
    if (!ros::isInitialized())
    { cout<<"data source ctor initialization"<<endl;
        int n = 0;
        ros::init(n, nullptr, "cognitao_ros1");
    }
    ros::NodeHandle n_;
    cout<<"create SUB"<<endl;
    event_sub_ = n_.subscribe("/cognitao_ros/world_model/updates", 1000,
                              &RosDataSource::onDataSourceEvent, this);
    cout<<"create PUB"<<endl;

    event_pub_ = n_.advertise<cognitao_ros::EventMsg>("/cognitao_ros/world_model/updates", 1000);
    // serviceGetVar = n_.advertiseService("server_get_var", &RosDataSource::serverGetVar, this);
    // serviceSetVar = n_.advertiseService("server_set_var", &RosDataSource::serverSetVar, this);
    spinTHread = std::thread(&RosDataSource::doSpin, this);
    spinTHread.detach();
    cout<<"finish CTOR"<<endl;
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
void RosDataSource::onDataSourceEvent(const cognitao_ros::EventMsg &msg)
{
    // WM::setVar(, msg.value);
    DataSource::variableUpdated(msg.key, msg.value);
}
