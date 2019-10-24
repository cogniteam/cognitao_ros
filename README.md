<div align="center">
  <img src="/ros.jpeg"><br><br>
</div>

-----------------

# DM_ROS
Decision making system (for implementing reactive planning architectures) proxy for ROS.

For more information about CogniTAO decision making system see-[https://git.cogni.io/cognitao/dm/blob/master/README.md] 

- [Getting Started](#getting-started)
- [Integration](#integration)
- [Prerequisites](#prerequisites)
- [RosDataSource](#send\geT-events)
- [Contributing](#contributing)
## Getting atarted
First, create your own workspace.
The next step is to download the project into the src directory.
To get a copy of the project up and running on your local machine use Download button.

If you work on linux you can clone it via terminal to your Home personal folder :
```
git clone "https://git.cogni.io/cognitao/dm_ros.git"
```
The project is now located in your workspace [src/dm_ros].
To get CogniTAO library you need to use the install.sh script which located in[yourWorkspace/src/dm_ros].
In order to run this script you need to get in to[yourWorkspace/src/dm_ros].
For linux users:
```
cd yourWorkspace/src/dm_ros

//run the install.sh script
./install.sh
```
The install.sh script:
- runs another script that downloads the most updated version of the project from the site.
- compile the project.

After the script is run the project will be placed in [yourWorkspace/src/cognitao.git].

## Integration

To use our library in your workspace add to your project file:
```
#include <CogniTAO.h>
```

**CMake**
After writing yout node, dont forget to add to your CMake file the executable file.
```
# CMakeLists.txt
add_executable(dm_ros_node
	  src/main.cpp		
)  
```
## Prerequisites
C++11 support, ROS.

## RosDataSource 
In your node- use the RosDataSource structer.
```
WM::init(new RosDataSource(argc, argv));
```

The RosDataSource object listening to topic called "/wme/out".
When someone publish event to this topic, the RosDataSource get the message and set the key-value parameters.
To publish via terminal:
```
rostopic pub /wme/in dm_ros/EventMsg '{key: COIN, value: ENT}'
```
** Publish COIN event
The setVar() method publishes to a topic called "/wme/in".
The kind of the messages is EventMsg.

EventMsg:
This message has two fields- both sting type.
The first field called- key, the second- value.
```
string key
string value
```


## Contributing

Feel free to contact us at info@cogniteam.com if you wish to contribute code to the library

