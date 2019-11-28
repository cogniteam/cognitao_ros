cmake_minimum_required(VERSION 2.8.3)
project(cognitao_ros)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  actionlib_msgs
  dynamic_reconfigure
  message_generation
  actionlib 
)

add_action_files(DIRECTORY ../action_manager/action FILES ActionMsg.action)
find_package(catkin REQUIRED genmsg actionlib_msgs actionlib)

include_directories(
	include
	${catkin_INCLUDE_DIRS} 
	../cognitao.git/include/
)

add_message_files(
  FILES
  EventMsg.msg
)

generate_messages(DEPENDENCIES actionlib_msgs)
catkin_package()


add_executable(cognitao_ros_node
	  src/main.cpp		
)  

add_dependencies(cognitao_ros_node
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)
	 
	  
target_link_libraries(cognitao_ros_node
${catkin_LIBRARIES}
	
)








