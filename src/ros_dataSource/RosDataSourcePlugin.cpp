#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CogniTao.h>
#include "../include/cognitao_ros/ros_dataSource/RosDataSource.h"
// extern "C" DataSource *create_datasource()
// {

//     return new RosDataSource();
// }

extern "C" void destroy_datasource(DataSource *object)
{

    delete object;
}

extern "C" const char *get_datasource_type()
{

    return "shared_rosDataSource";
}
int main2()
{
    cout << "shared lib dummy runner example" << endl;
    return 0;
}
