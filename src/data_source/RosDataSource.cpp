/**
 * @brief ROS 1 cognitao runner 
 * 
 * @file RosDataSource.cpp
 * 
 * @author Lin Azan (lin@cogniteam.com)
 * @date 2020-03-15
 * @copyright Cogniteam (c) 2020
 *    
 * MIT License
 *   
 * Permission is hereby granted, free of charge, to any person obtaining a  copy
 * of this software and associated documentation files (the 'Software'), to deal
 * in the Software without restriction, including without  limitation the rights
 * to use, copy, modify, merge,  publish,  distribute,  sublicense,  and/or sell
 * copies of the Software, and  to  permit  persons  to  whom  the  Software  is 
 * furnished to do so, subject to the following conditions:
 *   
 * The   above   copyright   notice   and  this   permission   notice  shall  be
 * included in all copies or substantial portions of the Software.
 *   
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
 * IMPLIED, INCLUDING BUT NOT LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN  NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE  LIABLE  FOR  ANY  CLAIM,  DAMAGES  OR  OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#include <cognitao_ros/data_source/RosDataSource.h>


RosDataSource::RosDataSource(){
    if (!ros::isInitialized()){ 
        int n = 0;
        ros::init(n, nullptr, "cognitao_ros", ros::init_options::AnonymousName);
    }

    nh_ptr_ = std::unique_ptr<ros::NodeHandle>(new ros::NodeHandle);
    
    event_sub_ = nh_ptr_->subscribe(
            "/cognitao_ros/world_model/updates", 1000,
                    &RosDataSource::onDataSourceEvent, this);


    event_pub_ = nh_ptr_->advertise<cognitao_ros::Event>(
            "/cognitao_ros/world_model/updates", 1000);
            
            
    static ros::AsyncSpinner spinner(1); // Use 1 threads
    spinner.start();
}

RosDataSource::~RosDataSource(){
    shutdown();
    ros::shutdown();
}

bool RosDataSource::publishUpdateEvent(const string &name, const string &value){
    cognitao_ros::Event msg;

    msg.key = name;
    msg.value = value;

    event_pub_.publish(msg);

    return true;
}
void RosDataSource::onDataSourceEvent(const cognitao_ros::Event &msg){
    DataSource::variableUpdated(msg.key, msg.value);
}

void RosDataSource::shutdown()
{
    //cout << "ROS:: closing publisher " << event_pub_.getTopic() << endl;
    event_pub_.shutdown(); 
    //cout << "ROS:: closing subscriber " << event_sub_.getTopic() << endl;
    event_sub_.shutdown();
    //cout << "ROS:: waiting for shutdown... "<< std::flush;
    for(int i =0; i<5; i++ )
    {
        cout << i << " "<< std::flush;
        sleep(1);
    }
    cout<< endl;
}