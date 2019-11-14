
##run a script which download from git the update version of dm project


./cogniTao_Install.sh 
echo "finish sh"

##go back to workSpace
cd ../..

##compile ros node which use the above project
catkin_make
echo "finish compilation"



exit 0
