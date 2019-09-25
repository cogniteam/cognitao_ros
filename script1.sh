
##run a script which download from git the update version of dm project


./command.sh 
echo "finish sh"

##compile ros node which use the above project
cd ../..

catkin_make
echo "finish compilation"

## run the node 
rosrun dm_ros dm_ros_node

exit 0
