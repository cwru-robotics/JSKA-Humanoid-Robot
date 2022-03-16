#include <ros/ros.h>

#include <sensor_msgs/JointState.h>

#include <sensor_msgs/Range.h>

bool halt;
void dist_cb(const sensor_msgs::RangeConstPtr r){
	if(//r-> range >0.5 &&  r-> range <0.8){
r->range < 2.0){
		ROS_WARN("BE CAREFUL! THERE IS AN OBJECT IN FRONT OF YOU!");
		system("paplay /usr/share/sounds/freedesktop/stereo/dialog-error.oga");
		halt = true;
} else {halt = false;} 	
}

int main(int argc, char** argv){

	ros::init(argc, argv, "joint_controller_node");
	ros::NodeHandle nh;
	
	halt = false;
	
	sensor_msgs::JointState joint_state_message;
	joint_state_message.name = {
		"slide_joint_z",
		"slide_joint_y",
		"slide_joint_x",
		"right_hipjoint_rotx",
		"right_hipjoint_roty",
		"right_hipjoint_rotz",
		"left_hipjoint_rotx",
		"left_hipjoint_roty",
		"left_hipjoint_rotz",
		"right_kneejoint_roty",
		"right_kneejoint_rotz",
		"left_kneejoint_roty",
		"left_kneejoint_rotz",
		"right_anklejoint_rotx",
		"right_anklejoint_roty",
		"right_anklejoint_rotz",
		"left_anklejoint_rotx",
		"left_anklejoint_roty",
		"left_anklejoint_rotz"
	};
	
	ros::Subscriber s1 = nh.subscribe("/sonar_left_thigh", 1, dist_cb);
	ros::Subscriber s2 = nh.subscribe("/sonar_right_thigh", 1, dist_cb);
	ros::Subscriber s3 = nh.subscribe("/sonar_pelvis", 1, dist_cb);
	
	ros::Publisher jcn_pub = nh.advertise<sensor_msgs::JointState>("Detection", 1);
	ros::Time start_time = ros::Time::now();
	
	//Initial values
	double slide_joint_z		= 0.0;
	double slide_joint_y		= 0.0;
	double slide_joint_x		= 0.0;
	double right_hipjoint_rotx	= 0.0;
	double right_hipjoint_roty	= 0.0;
	double right_hipjoint_rotz	= 0.0;
	double left_hipjoint_rotx	= 0.0;
	double left_hipjoint_roty	= 0.0;
	double left_hipjoint_rotz	= 0.0;
	double right_kneejoint_roty	= 0.0;
	double right_kneejoint_rotz	= 0.0;
	double left_kneejoint_roty	= 0.0;
	double left_kneejoint_rotz	= 0.0;
	double right_anklejoint_rotx	= 0.0;
	double right_anklejoint_roty	= 0.0;
	double right_anklejoint_rotz	= 0.0;
	double left_anklejoint_rotx	= 0.0;
	double left_anklejoint_roty	= 0.0;
	double left_anklejoint_rotz	= 0.0;
	
	double t_old = start_time.sec + start_time.nsec*10e-10;
	while(ros::ok()){
		
		ros::Duration t_in_duration_form = ros::Time::now() - start_time;

		double t = t_in_duration_form.sec + (t_in_duration_form.nsec * 10e-10);//Absolute time since start
		double delta_t = t - t_old;//Relative time since last loop
		
		/*=====================================================================
		Parametrized joints begin here. Each of these variables should be assigned a value as a function of t (time in seconds) or delta_t (time since last update in seconds).
		*/
		
		//Example sinusoid.
		//slide_joint_z = sin(t);
		
		//Example piecewise function. Fmod is the c++ floating-point modulo (%) operator.
		/*if(fmod(t, 2.0) < 1.0){
			slide_joint_y = fmod(t, 2.0);
		} else{
			slide_joint_y = 1.0 - fmod(t, 2.0);
		}*/
		
		//Example linear ramp
		slide_joint_x = 0.118 * t;
		slide_joint_z = -1.1;
		
		
		
		///WALK CYCLE ELEMENTS
		//Right leg joints are zero when times are 5-10, 15-20, etc.
		if(fmod(t, 10.0) > 5){
			right_hipjoint_roty = 0.0;
			right_kneejoint_roty = 0.0;
			right_anklejoint_roty = 0.0;
		} else{
			right_hipjoint_roty = -0.5 * sin((M_PI / 5.0) * t);
			right_kneejoint_roty = sin((M_PI/5.0) * t);
			right_anklejoint_roty = 0.5 * sin((M_PI / 5.0) * t);
		}
		
		//Left leg joints are zero when times are 0-5, 10-15, etc.
		if(fmod(t, 10.0) < 5){
			left_hipjoint_roty = 0.0;
			left_kneejoint_roty = 0.0;
			left_anklejoint_roty = 0.0;
		} else{
			left_hipjoint_roty = 0.5 * sin((M_PI / 5.0) * t);
			left_kneejoint_roty = -sin((M_PI/5.0) * t);
			left_anklejoint_roty = 0.5 * sin((M_PI / 5.0) * t);
		}
		
		/*
		Parametrized joints end here. The rest of this is just bookkeeping.
		====================================================================*/
		
		joint_state_message.position = {
			slide_joint_z,		// /Detection/position[0]
			slide_joint_y,		// /Detection/position[1]
			slide_joint_x,		// /Detection/position[2]
			right_hipjoint_rotx,	// /Detection/position[3]
			right_hipjoint_roty,	// /Detection/position[4]
			right_hipjoint_rotz,	// /Detection/position[5]
			left_hipjoint_rotx,	// /Detection/position[6]
			left_hipjoint_roty,	// /Detection/position[7]
			left_hipjoint_rotz,	// /Detection/position[8]
			right_kneejoint_roty,	// /Detection/position[9]
			right_kneejoint_rotz,	// /Detection/position[10]
			left_kneejoint_roty,	// /Detection/position[11]
			left_kneejoint_rotz,	// /Detection/position[12]
			right_anklejoint_rotx,	// /Detection/position[13]
			right_anklejoint_roty,	// /Detection/position[14]
			right_anklejoint_rotz,	// /Detection/position[15]
			left_anklejoint_rotx,	// /Detection/position[16]
			left_anklejoint_roty,	// /Detection/position[17]
			left_anklejoint_rotz	// /Detection/position[18]
		};
		joint_state_message.header.stamp = ros::Time::now();
		if(!halt){
			jcn_pub.publish(joint_state_message);
		}
		
		t_old = t;
	
		ros::spinOnce();
		ros::Duration(0.1).sleep();//Update once every tenth of a second.
	}
}
