cmake_minimum_required(VERSION 2.8.3)
project(cognitao_ros_action_server)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  actionlib_msgs
  dynamic_reconfigure
  message_generation
  actionlib
)

find_package(catkin REQUIRED genmsg actionlib_msgs actionlib)

find_package(Threads)

include_directories(
	include
	${catkin_INCLUDE_DIRS} 
	../../cognitao.git/include/
)

add_action_files(DIRECTORY ../action_manager/action FILES ActionMsg.action)


generate_messages(DEPENDENCIES actionlib_msgs)
add_executable(action_server_not_composable src/main.cpp)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES cognitao_ros_action_server
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)
add_dependencies(action_server_not_composable
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)
target_link_libraries(action_server_not_composable
${catkin_LIBRARIES}
	
)

