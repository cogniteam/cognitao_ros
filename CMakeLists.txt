cmake_minimum_required(VERSION 2.8.3)
project(cognitao_ros)

add_compile_options(-std=c++14)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  actionlib_msgs
  dynamic_reconfigure
  message_generation
  actionlib 
  genmsg
  actionlib_msgs 
  
)

find_package(Threads)

include_directories(
	include
	${catkin_INCLUDE_DIRS} 
	/usr/include/cognitao/
)

add_message_files(
  FILES
  EventMsg.msg
  KeyValue.msg
  GoalMsg.msg

)
add_service_files(
  FILES
  getvar.srv
  setvar.srv
)



add_action_files(DIRECTORY action FILES ActionMsg.action)


generate_messages(DEPENDENCIES std_msgs actionlib_msgs )

catkin_package()

#
# Build
#

#
# shared Runner plugin
#

add_library(cognitao_ros_shared SHARED
 src/client/Ros1RunnerPlugin.cpp
 src/client/Ros1Runner.cpp 
)


add_dependencies(cognitao_ros_shared
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)

	  
target_link_libraries(cognitao_ros_shared
${catkin_LIBRARIES}	
)

#
## shared Data_source
#

add_library(ros_datasource_shared SHARED
 src/ros_dataSource/RosDataSourcePlugin.cpp
 src/ros_dataSource/RosDataSource.cpp
)


add_dependencies(ros_datasource_shared
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)

	  
target_link_libraries(ros_datasource_shared
${catkin_LIBRARIES}	
)

#install(TARGETS
#   ros_datasource_shared
#  DESTINATION /usr/lib/cognitao/plugins/
#)



#
# Ros action server
#

add_executable(action_server_not_composable
 src/server/action_server_main.cpp
 src/server/RosActionServer.cpp
 )

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
${catkin_LIBRARIES})

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)


#
# Action server executable example
#

add_executable(action_server 
src/server_example/Ros_Server_Example_Main.cpp
src/server/RosActionServer.cpp
src/server_example/RosActionServerExample.cpp
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES cognitao_ros_action_server
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

add_dependencies(action_server
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)

target_link_libraries(action_server
${catkin_LIBRARIES}
	
)



##generate_messages(DEPENDENCIES actionlib_msgs cognitao_ros)

	





