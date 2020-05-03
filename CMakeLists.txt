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

include_directories(
  include
  ${catkin_INCLUDE_DIRS} 
)

add_message_files(
  FILES
  Event.msg
  KeyValue.msg
  Action.msg
)

add_service_files(
  FILES
  getvar.srv
  setvar.srv
)

add_action_files(DIRECTORY action FILES Runner.action)

generate_messages(DEPENDENCIES std_msgs actionlib_msgs)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES cognitao_ros_action_server
)

#
# Build
#

#
# Runner plugin
#

add_library(cognitao_ros_runner SHARED
  src/runner/RosRunnerPlugin.cpp
  src/runner/RosRunner.cpp 
)

add_dependencies(cognitao_ros_runner
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
	  
target_link_libraries(cognitao_ros_runner
  ${catkin_LIBRARIES}
)

#
## Runner Data_source
#

add_library(cognitao_ros_datasource SHARED
 src/data_source/RosDataSourcePlugin.cpp
 src/data_source/RosDataSource.cpp
)

add_dependencies(cognitao_ros_datasource
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
	  
target_link_libraries(cognitao_ros_datasource
  ${catkin_LIBRARIES}	
)

#
# Action server
#
add_library(cognitao_ros_action_server SHARED
  src/server/RosActionServer.cpp
)

add_dependencies(cognitao_ros_action_server
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
	  
target_link_libraries(cognitao_ros_action_server
  ${catkin_LIBRARIES}	
)

install(TARGETS
  cognitao_ros_datasource
  cognitao_ros_runner
  DESTINATION /usr/lib/cognitao/plugins/
)

install(TARGETS cognitao_ros_action_server
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)