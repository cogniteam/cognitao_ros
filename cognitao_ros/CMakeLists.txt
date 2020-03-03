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
  #cognitao_ros
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


add_executable(cognitao_ros_node
	  src/main.cpp		
)  
add_library(cognitao_ros_shared SHARED src/main.cpp)
generate_messages(DEPENDENCIES std_msgs actionlib_msgs )
catkin_package()

add_dependencies(cognitao_ros_node
cognitao_ros_shared
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)
	 
	  
target_link_libraries(cognitao_ros_node
${catkin_LIBRARIES}
	
)

add_executable(action_server_not_composable src/action_server_main.cpp)
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

add_executable(action_server src/minimal_server_example_main.cpp)
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

	





