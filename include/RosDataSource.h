
#include <CogniTAO.h>
#include "ros/ros.h"
#include "/home/lin/dm_ros/devel/include/dm_ros/EventMsg.h"


void doSpin(){

	ros::spin();
}

class RosDataSource: public  MapThreadSafeDataSource {


public:

	RosDataSource() {

		ros::NodeHandle n_;

		event_sub_ = n_.subscribe("/wme_in", 1000,
				&RosDataSource::callback, this);

		event_pub_ = n_.advertise<dm_ros::EventMsg>("/wme_out", 1000);  

		std::thread spinTHread(doSpin);
		spinTHread.detach();

		

	}
	~RosDataSource(){

	}

	


	virtual void setVar(std::string variable,std::string value) override{
		cout<<"enter to setVar "<<std::endl;
		sl.lock();
		//cout<< "SET [" << variable << "]["  << value <<"]\n";
		wm_[variable]=value;
		//WM::setVar(variable,value);
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
        cout<<"publish msg "<<variable<<", "<<value<<endl;
        event_pub_.publish(msg);

    }

	

	void callback(const dm_ros::EventMsg& msg) {
		cout<<"set var from callback "<<endl;
		WM::setVar(msg.key, msg.value);
	}


private:

    ros::Publisher event_pub_;   

	ros::Subscriber event_sub_;

};


