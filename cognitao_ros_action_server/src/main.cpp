#include "../include/MinimalActionServer.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "action_server_not_composable");
  
  MinimalActionServer server;
  ros::spin();
  return 0;
}