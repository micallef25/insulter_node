#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "file_parser.h"


void make_word_node(std::string word_bytes){

  int start_pos;
  int end_pos;
  std::string token = "/";
  std::string ws = " ";
  std::string character;
//  start_pos = word_bytes.find(token);
//  end_pos = word_bytes.find(" ",1);

  //std::cout << word_bytes << std::endl;
  //std::cout << "start_pos" << start_pos << std::endl;
  //std::cout << "end_pos" << end_pos << std::endl;
  // character = word_bytes.substr(start_pos+1,end_pos-1);
  // std::cout << character << std::endl;
  while ((start_pos = word_bytes.find(token)) != std::string::npos)
		{
       end_pos = word_bytes.find(ws,1);
       character = word_bytes.substr(start_pos+1,end_pos);
		   word_bytes.erase(start_pos,end_pos);
       if((start_pos = character.find(ws)) != std::string::npos){
         character.erase(start_pos,start_pos+ws.length());
       }
		   std::cout << character << std::endl;
    }
// std::cout << std::endl;


}

void initialize_insults(){

  int pos;
  std::ifstream config_file;
  std::string file_info;
  std::string file = "/home/ivyleaguesloth/BullyBot/catkin_ws/src/insulter/src/PhraseALator.Dic";
  // open the file as input only
	config_file.open(file.c_str(), std::ios::in);// | std::ios:: binary);// | ios::ate);
			if(config_file.is_open()){
        // read line by line
        while(getline(config_file,file_info)){
					// std::cout << file_info << std::endl;
          //typical line looks like sky = \SE \KE \OHIH \FAST \IH
          pos = file_info.find("=");
          if(pos != std::string::npos){
          // now \SE \KE \OHIH \FAST \IH
            file_info.erase(0,pos+1);
            make_word_node(file_info);
          }


        }
			}
      else{
        printf("error opening file\n");
      }
}

/**
* This tutorial demonstrates simple sending of messages over the ROS system.
*/
int main(int argc, char **argv)
{
/**
* The ros::init() function needs to see argc and argv so that it can perform
* any ROS arguments and name remapping that were provided at the command line.
* For programmatic remappings you can use a different version of init() which takes
* remappings directly, but for most command-line programs, passing argc and argv is
* the easiest way to do it.  The third argument to init() is the name of the node.
*
* You must call one of the versions of ros::init() before using any other
* part of the ROS system.
*/
  std::cout << "yo " << std::endl;
  initialize_map();
  initialize_insults();
	ros::init(argc, argv, "insulter");
/**
* NodeHandle is the main access point to communications with the ROS system.
* The first NodeHandle constructed will fully initialize this node, and the last
* NodeHandle destructed will close down the node.
*/
	ros::NodeHandle n;
/**
* The advertise() function is how you tell ROS that you want to
* publish on a given topic name. This invokes a call to the ROS
* master node, which keeps a registry of who is publishing and who
* is subscribing. After this advertise() call is made, the master
* node will notify anyone who is trying to subscribe to this topic name,
* and they will in turn negotiate a peer-to-peer connection with this
* node.  advertise() returns a Publisher object which allows you to
* publish messages on that topic through a call to publish().  Once
* all copies of the returned Publisher object are destroyed, the topic
* will be automatically unadvertised.
*
* The second parameter to advertise() is the size of the message queue
* used for publishing messages.  If messages are published more quickly
* than we can send them, the number here specifies how many messages to
* buffer up before throwing some away.
*/
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  // publish at 10hz
  ros::Rate loop_rate(.1);
/**
* A count of how many messages we have sent. This is used to create
* a unique string for each message.
*/
	int count = 0;
 	while (ros::ok())
  {
/**
* This is a message object. You stuff it with data, and then publish it.
*/
 	std_msgs::String msg;
 	std::stringstream ss;
 	ss << "hello world " << count;
  msg.data = ss.str();

 	ROS_INFO("%s", msg.data.c_str());
/**
* The publish() function is how you send messages. The parameter
* is the message object. The type of this object must agree with the type
* given as a template parameter to the advertise<>() call, as was done
* in the constructor above.
*/
   chatter_pub.publish(msg);

   ros::spinOnce();
   loop_rate.sleep();
   ++count;
	}
}