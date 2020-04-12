#include <cognitao_ros/server_example/RosActionWaitServer.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "ros_action_server", ros::init_options::AnonymousName);
    ros::NodeHandle nh_;	
    RosActionWaitServer server(nh_,"wait");
    ros::spin();
    return 0;
}


