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

find_package(Threads 
   
)


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


add_executable(cognitao_ros_node
	  src/client/Ros1RunnerPlugin.cpp
    src/ros_dataSource/RosDataSourcePlugin.cpp
    src/ros_dataSource/RosDataSource.cpp
    src/ros_dataSource/src_executables_cognitao_wmm.cpp
    src/client/Ros1Runner.cpp		
)  
add_dependencies(cognitao_ros_node
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)

# shared
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

## shared data_source
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

generate_messages(DEPENDENCIES std_msgs actionlib_msgs )
catkin_package()
	 
	  
target_link_libraries(cognitao_ros_node
cognitao_core
${catkin_LIBRARIES}
	
)

add_executable(action_server_not_composable
 src/server/action_server_main.cpp
 src/server/MinimalActionServer.cpp
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

add_executable(action_server 
src/server_example/minimal_server_example_main.cpp
src/server/MinimalActionServer.cpp
src/server_example/MinimalActionServerExample.cpp
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

	





