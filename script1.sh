
##run a script which download from git the update version of dm project


./command.sh 
echo "finish sh"

##go back to workSpace
cd ../..

##compile ros node which use the above project
catkin_make
echo "finish compilation"

## run the node 
rosrun dm_ros dm_ros_nodes

exit 0
