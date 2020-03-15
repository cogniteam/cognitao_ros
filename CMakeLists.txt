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
# Runner plugin
#

add_library(cognitao_ros_runner SHARED
    src/client/Ros1RunnerPlugin.cpp
    src/client/Ros1Runner.cpp 
)

add_dependencies(cognitao_ros_runner
    ${${PROJECT_NAME}_EXPORTED_TARGETS}
    ${catkin_EXPORTED_TARGETS}
)
	  
target_link_libraries(cognitao_ros_runner
    ${catkin_LIBRARIES}	
)


#
# Runner datasource
#

# add_library(cognitao_ros_datasource SHARED
#     src/client/Ros1RunnerPlugin.cpp
#     src/client/Ros1Runner.cpp 
# )

# add_dependencies(cognitao_ros_datasource
#     ${${PROJECT_NAME}_EXPORTED_TARGETS}
#     ${catkin_EXPORTED_TARGETS}
# )
	  
# target_link_libraries(cognitao_ros_datasource
#     ${catkin_LIBRARIES}	
# )


#
# Action server
#

# add_library(cognitao_ros_action_server SHARED
#     src/client/Ros1RunnerPlugin.cpp
#     src/client/Ros1Runner.cpp 
# )

# add_dependencies(cognitao_ros_action_server
#     ${${PROJECT_NAME}_EXPORTED_TARGETS}
#     ${catkin_EXPORTED_TARGETS}
# )
	  
# target_link_libraries(cognitao_ros_action_server
#     ${catkin_LIBRARIES}	
# )


#
# Action server executable example
#

# add_executable(action_server_example SHARED
#     src/client/Ros1RunnerPlugin.cpp
#     src/client/Ros1Runner.cpp 
# )

# add_dependencies(action_server_example
#     ${${PROJECT_NAME}_EXPORTED_TARGETS}
#     ${catkin_EXPORTED_TARGETS}
# )
	  
# target_link_libraries(action_server_example
#     cognitao_ros_action_server
# )