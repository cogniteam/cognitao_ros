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

generate_messages(DEPENDENCIES 
  std_msgs 
  actionlib_msgs
)

catkin_package()

#########
# Build #
#########

include_directories(
  include
  ${catkin_INCLUDE_DIRS} 
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

add_action_files(
  DIRECTORY 
    action 
  FILES 
    ActionMsg.action
)

add_executable(cognitao_ros_node
  src/client/Ros1RunnerPlugin.cpp
  src/client/Ros1Runner.cpp
)  

add_dependencies(cognitao_ros_node
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

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

target_link_libraries(cognitao_ros_node
  ${catkin_LIBRARIES}
)

add_executable(action_server_not_composable
 src/server/action_server_main.cpp
 src/server/MinimalActionServer.cpp
)

add_dependencies(action_server_not_composable
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(action_server_not_composable
  ${catkin_LIBRARIES}
)

add_executable(action_server 
  src/server_example/minimal_server_example_main.cpp
  src/server/MinimalActionServer.cpp
  src/server_example/MinimalActionServerExample.cpp
)

add_dependencies(action_server
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(action_server
  ${catkin_LIBRARIES}
)