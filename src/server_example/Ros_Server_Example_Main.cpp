#include <cognitao_ros/server_example/RosActionServerExample.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "action_server_not_composable");
  
  RosActionServerExample server;
  ros::spin();
  return 0;
}


