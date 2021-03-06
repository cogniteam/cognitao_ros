/*
 * MultiGoalActionServer.h
 * @author Lin Azan (lin@cogniteam.com)
 * @date 2020-03-15
 * @copyright Cogniteam (c) 2020
 *
 *
 * Cogniteam LTD CONFIDENTIAL
 *
 * Unpublished Copyright (c) 2010-2020 Cogniteam,        All Rights Reserved.
 *
 * NOTICE:  All information contained  herein  is,  and  remains the property
 * of Cogniteam.   The   intellectual   and   technical   concepts  contained
 * herein are proprietary to Cogniteam and may  be  covered  by  Israeli  and
 * Foreign Patents, patents in process,  and  are  protected  by trade secret
 * or copyright law. Dissemination of  this  information  or  reproduction of
 * this material is strictly forbidden unless  prior  written  permission  is
 * obtained  from  Cogniteam.  Access  to  the  source  code contained herein
 * is hereby   forbidden   to   anyone  except  current  Cogniteam employees,
 * managers   or   contractors   who   have   executed   Confidentiality  and
 * Non-disclosure    agreements    explicitly    covering     such     access
 *
 * The copyright notice  above  does  not  evidence  any  actual  or intended
 * publication  or  disclosure    of    this  source  code,   which  includes
 * information that is confidential  and/or  proprietary,  and  is  a   trade
 * secret, of   Cogniteam.    ANY REPRODUCTION,  MODIFICATION,  DISTRIBUTION,
 * PUBLIC   PERFORMANCE,  OR  PUBLIC  DISPLAY  OF  OR  THROUGH USE   OF  THIS
 * SOURCE  CODE   WITHOUT   THE  EXPRESS  WRITTEN  CONSENT  OF  Cogniteam  IS
 * STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL
 * TREATIES.  THE RECEIPT OR POSSESSION OF  THIS SOURCE  CODE  AND/OR RELATED
 * INFORMATION DOES  NOT CONVEY OR IMPLY ANY RIGHTS  TO  REPRODUCE,  DISCLOSE
 * OR  DISTRIBUTE ITS CONTENTS, OR TO  MANUFACTURE,  USE,  OR  SELL  ANYTHING
 * THAT      IT     MAY     DESCRIBE,     IN     WHOLE     OR     IN     PART
 *
 */


#ifndef MULTI_GOAL_ACTION_SERVER_H_
#define MULTI_GOAL_ACTION_SERVER_H_

#include <thread>
#include <deque>

#include <ros/ros.h>

#include <actionlib/server/action_server.h>
#include <actionlib/action_definition.h>


using namespace std;

namespace actionlib{

/**
 * Can handle more than one client
 */
template <class ActionSpec>
class MultiGoalActionServer{

public:
    //generates typedefs that we'll use to make our lives easier
    ACTION_DEFINITION(ActionSpec);
    typedef typename ActionServer<ActionSpec>::GoalHandle GoalHandle;
    typedef boost::function<void(GoalHandle)> ExecuteCallback;

class GoalQueueItem{

public:

    GoalQueueItem(GoalHandle g) : goal(g), isPreemptReq(false), t(0) {}

    ~GoalQueueItem(){

    isPreemptReq = true;
    if (t)
    t = NULL;
        }
    
public:

    GoalHandle goal;

    bool isPreemptReq;

    thread *t;
    };

class GoalQueue{

public:

    typedef typename std::deque<typename MultiGoalActionServer<ActionSpec>::GoalQueueItem>::iterator iterator;
    std::deque<GoalQueueItem> queue;

public:

    void add(GoalHandle goal){

        queue.push_back(GoalQueueItem(goal));
    }

    iterator find(GoalHandle goal){

        for (iterator i = queue.begin(); i != queue.end(); i++)
                if (goal == i->goal)
                    return i;

            return queue.end();
    }

    iterator begin() { return queue.begin(); }
    
    iterator end() { return queue.end(); }
    
    size_t size() const { return queue.size(); }
    
    void remove(GoalHandle goal){

        iterator i = find(goal);
        if (i != end()){

            queue.erase(i);
        }
    }

    void remove(iterator i){

        if (i != end()){

            queue.erase(i);
            }
        }
    };
public:

    MultiGoalActionServer(ros::NodeHandle n, std::string name, ExecuteCallback execute_callback, bool auto_start) : n_(n), execute_callback_(execute_callback), need_to_terminate_(false){


        //create the action server
        as_ = boost::shared_ptr<ActionServer<ActionSpec>>(
            new ActionServer<ActionSpec>(
                n, name,
                boost::bind(&MultiGoalActionServer::goalCallback, this, _1),
                boost::bind(&MultiGoalActionServer::preemptCallback, this, _1),
                auto_start));

        if (execute_callback_ != NULL){

            execute_thread_ = new thread(boost::bind(&MultiGoalActionServer::executeLoop, this));
        }
    }

    ~MultiGoalActionServer()
    {
        if (execute_thread_)
        {
            cout << " MultiGoalServer exiting" <<endl;
            shutdown();
        }
    }

public:

    bool isPreemptRequested(GoalHandle goal){

        boost::recursive_mutex::scoped_lock lock(lock_);
        typename GoalQueue::iterator g = active_goals.find(goal);
        
        if (g != active_goals.end()){

            return g->isPreemptReq;
        }
        return true;
    }

    bool isActive(GoalHandle gh){

        
        typename GoalQueue::iterator g = active_goals.find(gh);
        
        if (g == active_goals.end())
            return false;
        
        if (!g->goal.getGoal())
            return false;
        
        unsigned int status = g->goal.getGoalStatus().status;
        return status == actionlib_msgs::GoalStatus::ACTIVE || status == actionlib_msgs::GoalStatus::PREEMPTING;
    }

    void setSucceeded(GoalHandle gh, const Result &result = Result(), const std::string &text = std::string("")){

        boost::recursive_mutex::scoped_lock lock(lock_);
        ROS_DEBUG_NAMED("actionlib", "Setting the current goal as succeeded");
        gh.setSucceeded(result, text);
        requestPreemption(gh);
    }
    void setAborted(GoalHandle gh, const Result &result = Result(), const std::string &text = std::string("")){

        boost::recursive_mutex::scoped_lock lock(lock_);
        ROS_DEBUG_NAMED("actionlib", "Setting the current goal as aborted");
        gh.setAborted(result, text);

        requestPreemption(gh);
    }
    void setPreempted(GoalHandle gh, const Result &result = Result(), const std::string &text = std::string("")){

        boost::recursive_mutex::scoped_lock lock(lock_);
        ROS_DEBUG_NAMED("actionlib", "Setting the current goal as canceled");
        gh.setCanceled(result, text);
        requestPreemption(gh);
    }
    void requestPreemption(GoalHandle gh){

        cout << "-------------------requestPreemption-------------------" << std::endl;
        typename GoalQueue::iterator g = active_goals.find(gh);

        if (g != active_goals.end()){

            g->isPreemptReq = true;
            execute_condition_.notify_all();
        }
    }

    void start(){

        as_->start();
    }

    void shutdown(){

        if (execute_callback_){

            
            boost::mutex::scoped_lock terminate_lock(terminate_mutex_);
            need_to_terminate_ = true;
            
            assert(execute_thread_);
            execute_thread_->join();
            delete execute_thread_;
            execute_thread_ = NULL;
        }
    }

    void removePreemptedGoals(std::vector<thread *> &thread_for_delete){

        if (active_goals.size() > 0){

            bool found = true;
            while (found){

                found = false;
                for (typename GoalQueue::iterator i = active_goals.begin(); i != active_goals.end(); i++){

                    if (i->isPreemptReq){

                        std::thread *th = i->t;
                        active_goals.remove(i);
                        thread_for_delete.push_back(th);
                        found = true;
                        break;
                    }
                }
            }
        }
    }

private:

    void goalCallback(GoalHandle goal){

        boost::recursive_mutex::scoped_lock lock(lock_);
        ROS_DEBUG_NAMED("actionlib", "A new goal has been recieved by the multi goal action server");
        ROS_INFO("actionlib: "
                 "A new goal has been recieved by the multi goal action server");
        new_goals.add(goal);
        execute_condition_.notify_all();

    }

    void preemptCallback(GoalHandle preempt){

        boost::recursive_mutex::scoped_lock lock(lock_);
        ROS_DEBUG_NAMED("actionlib", "A preempt has been received by the MGActionServer");
        ROS_INFO("actionlib: "
                 "A preempt has been received by the MGActionServer");
        typename GoalQueue::iterator i = new_goals.find(preempt);
        if (i != new_goals.end()) {

            i->goal.setCanceled(Result(), "Cancled before activation");
            new_goals.remove(i);
            std::cout << "TaskServer: goal was preempted without activation" << std::endl;
            return;
        }
        requestPreemption(preempt);
    }

    void executeLoop()
    {

        ros::Duration loop_duration = ros::Duration().fromSec(.1);
        std::vector<thread *> thread_for_delete;

        while (n_.ok()){

            while (thread_for_delete.size() > 0)
            {
                thread_for_delete[0]->join();
                delete thread_for_delete[0];
                thread_for_delete.erase(thread_for_delete.begin());
            }

            {
                boost::mutex::scoped_lock terminate_lock(terminate_mutex_);
                if (need_to_terminate_)
                    break;
            }

            boost::recursive_mutex::scoped_lock lock(lock_);

            bool reportSize = false;
            size_t agc = active_goals.size();
            size_t stopped_tasks = 0;
            removePreemptedGoals(thread_for_delete);
            if (agc != active_goals.size())
            {
                stopped_tasks = agc - active_goals.size();
                reportSize = true;
            }

            size_t started_tasks = 0;

            while (new_goals.size() > 0)
            {
                GoalHandle goal = new_goals.begin()->goal;
                new_goals.remove(new_goals.begin());
                active_goals.add(goal);
                typename GoalQueue::iterator igoal = active_goals.find(goal);
                goal.setAccepted("Goal activated by MGServer");
                igoal->t = new thread(boost::bind(execute_callback_, goal));
                started_tasks++;
            }

            if (started_tasks)
            {
                reportSize = true;
            }

            if (reportSize)
            {
                ROS_INFO("TaskServer: stopped %i, started %i, active %i",
                         (int)stopped_tasks, (int)started_tasks, (int)active_goals.size());
            }

            execute_condition_.timed_wait(lock, boost::posix_time::milliseconds(loop_duration.toSec() * 1000.0f));
        }

        while (new_goals.size() > 0)
            new_goals.remove(new_goals.begin());
        while (active_goals.size() > 0)
        {
            thread_for_delete.push_back(active_goals.begin()->t);
            active_goals.remove(active_goals.begin());
        }
        while (thread_for_delete.size() > 0)
        {
            thread_for_delete[0]->join();
            delete thread_for_delete[0];
            thread_for_delete.erase(thread_for_delete.begin());
        }
    }

private:

    ros::NodeHandle n_;

    boost::shared_ptr<ActionServer<ActionSpec>> as_;
    //GoalHandle current_goal_, next_goal_;
    
    GoalQueue active_goals, new_goals;
    //bool new_goal_, preempt_request_, new_goal_preempt_request_;
    
    boost::recursive_mutex lock_;
    
    ExecuteCallback execute_callback_;
    
    boost::condition execute_condition_;
    
    thread *execute_thread_;
    
    boost::mutex terminate_mutex_;
    
    bool need_to_terminate_;
};
} // namespace actionlib
#endif
