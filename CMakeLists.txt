cmake_minimum_required(VERSION 2.8.3)
project(dm_ros)


add_compile_options(-std=c++11)

#execute_process(
# 	COMMAND bash /home/lin/dm_ws/src/dm_ros/command.sh 
# 	RESULT_VARIABLE TFLITE_SCRIPT_RESULT 
#)




find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs 
)




find_package(catkin REQUIRED COMPONENTS
roscpp
dynamic_reconfigure
std_msgs
message_generation
)


include_directories(
	include
	${catkin_INCLUDE_DIRS} 
	/home/lin/dm_ws/src/cognitao.git/include/
	)

catkin_package()




add_executable(dm_ros_nodes
	  src/main.cpp		
)  

add_dependencies(dm_ros_nodes
${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS}
)
	 
	  
target_link_libraries(dm_ros_nodes
${catkin_LIBRARIES}
	##LinStaticLibrary -L/home/lin/dm_ws/src/dm.git/engine/build
)





