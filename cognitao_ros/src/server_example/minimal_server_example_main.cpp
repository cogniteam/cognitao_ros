#include "../include/cognitao_ros/server_example/MinimalActionServerExample.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "action_server_not_composable");
  
  MinimalActionServerExample server;
  ros::spin();
  return 0;
}
