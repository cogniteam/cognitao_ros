
#include <CogniTAO.h>
#include "ros/ros.h"
#include "../../../../devel/include/cognitao_ros/getvar.h"
#include "../../../../devel/include/cognitao_ros/EventMsg.h"
#include "../../../../devel/include/cognitao_ros/setvar.h"

void doSpin()
{

	ros::spin();
}

class RosDataSource : public MapThreadSafeDataSource
{

public:
	RosDataSource(int argc, char **argv)
	{

		ros::init(argc, argv, "DMS");
		ros::NodeHandle n_;

		event_sub_ = n_.subscribe("/wme/in", 1000,
								  &RosDataSource::callback, this);

		event_pub_ = n_.advertise<cognitao_ros::EventMsg>("/wme/out", 1000);
		serviceGetVar = n_.advertiseService("server_get_var", &RosDataSource::serverGetVar, this);
		serviceSetVar = n_.advertiseService("server_set_var", &RosDataSource::serverSetVar, this);
		spinTHread = std::thread(doSpin);
		spinTHread.detach();
	}
	~RosDataSource()
	{
		spinTHread.join();
	}

	virtual void setVar(std::string variable, std::string value) override
	{
		cout << "enter to setVar " << std::endl;
		sl.lock();
		wm_[variable] = value;
		publishEvent(variable, value);
		sl.unlock();
	}
	virtual std::string getVar(std::string variable) override
	{
		std::string ret = "";
		sl.lock_shared();
		ret = wm_[variable];
		sl.unlock_shared();
		return ret;
	}
	virtual std::string toString() override
	{
		std::ostringstream strs;
		strs << wm_;
		std::string str = strs.str();
		return str;
	}

	void publishEvent(std::string variable, std::string value)
	{

		cognitao_ros::EventMsg msg;

		msg.key = variable;
		msg.value = value;
		cout << "publish msg " << variable << ", " << value << endl;
		event_pub_.publish(msg);
	}
	bool serverGetVar(cognitao_ros::getvar::Request &req,
					  cognitao_ros::getvar::Response &res)
	{
		sl.lock_shared();
		res.value = wm_[req.key];
		std::cout<<"SERVER-GET-VAR "<<req.key<<" IS"<<wm_[req.key] <<endl;
		sl.unlock_shared();
		cout << "service on----> return to server " << res.value << endl;
		return true;
	}

	bool serverSetVar(cognitao_ros::setvar::Request &req,
					  cognitao_ros::setvar::Response &res)
	{
		sl.lock();
		wm_[req.key] = req.value;
		publishEvent(req.key, req.value);
		std::cout<<"SERVER-SET-VAR "<<req.key<<" IS"<<wm_[req.key] <<endl;
		sl.unlock();
		cout << "service on----> set var " << req.key << " to " << req.value << endl;
		return true;
	}
	


void
callback(const cognitao_ros::EventMsg &msg)
{
	WM::setVar(msg.key, msg.value);
}

private:
ros::Publisher event_pub_;

ros::Subscriber event_sub_;

std::thread spinTHread;
ros::ServiceServer serviceGetVar;
ros::ServiceServer serviceSetVar;
}
;
