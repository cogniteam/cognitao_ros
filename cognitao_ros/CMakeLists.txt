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

find_package(catkin REQUIRED )

include_directories(
	include
	${catkin_INCLUDE_DIRS} 
	../cognitao.git/include/
)

add_message_files(
  FILES
  EventMsg.msg
  KeyValue.msg
  GoalMsg.msg

)



add_action_files(DIRECTORY action FILES ActionMsg.action)


add_executable(cognitao_ros_node
	  src/main.cpp		
)  
generate_messages(DEPENDENCIES std_msgs actionlib_msgs )
catkin_package()

add_dependencies(cognitao_ros_node
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)
	 
	  
target_link_libraries(cognitao_ros_node
${catkin_LIBRARIES}
	
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)







