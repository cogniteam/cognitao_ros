
#include <CogniTAO.h>
#include "ros/ros.h"
#include "/home/lin/dm_ros/devel/include/dm_ros/EventMsg.h"

namespace rosns{

class RosDataSource: public  MapThreadSafeDataSource {


public:

	virtual void setVar(std::string variable,std::string value) override{
		cout<<"enter to setVar "<<std::endl;
		  sl.lock();
		  //cout<< "SET [" << variable << "]["  << value <<"]\n";
		  wm_[variable]=value;
		  publishEvent(variable,value);
		  sl.unlock();

	}
	virtual std::string getVar(std::string variable) override{
		std::string ret="";
		sl.lock_shared();
		ret = wm_[variable];
		sl.unlock_shared();
		//cout<< "READ [" << variable <<"]\n";
		//cout<< "GET [" << ret <<"]\n";
		return ret;

	}
	virtual std::string toString() override {
		std::ostringstream strs;
		strs  << wm_;
		std::string str = strs.str();
		return str;
	}

    void publishEvent(std::string variable,std::string value) {

        dm_ros::EventMsg msg;

        msg.key = variable;
        msg.value = value;
        cout<<"publish msg "<<endl;
        event_pub_.publish(msg);

        //ros::spinOnce();
    }

private:

    ros::NodeHandle n_;
    ros::Publisher event_pub_ = n_.advertise<dm_ros::EventMsg>("/wme", 1000);    

};


}