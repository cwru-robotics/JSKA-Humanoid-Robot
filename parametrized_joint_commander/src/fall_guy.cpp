#include <gazebo/gazebo.hh>
#include <gazebo/sensors/sensors.hh>

#include <ros/ros.h>
#include <std_msgs/Bool.h>

namespace gazebo{
	class FallGuy : public ModelPlugin{
		private:
		//Something to do with contacts or something. Looks like ROS but is not ROS.
		transport::SubscriberPtr contact_sub;
		transport::NodePtr contact_node;
		event::ConnectionPtr updateConnection;
		
		//ROS communication
		ros::NodeHandle nh;
		ros::Publisher statpub;
			
		void ContactCB(ConstContactsPtr &msg){
			for(int i = 0; i < msg->contact_size(); i++){
				//printf("%s vs %s \n", msg->contact(i).collision1().c_str(), msg->contact(i).collision2().c_str());
				if(msg->contact(i).collision1().compare("ground_plane::link::collision") && msg->contact(i).collision2().compare("ground_plane::link::collision")){
					std_msgs::Bool fall_msg;
					statpub.publish(fall_msg);
				}
			}
		}
			
		public: FallGuy() : ModelPlugin(){
		}

		public: void Load(physics::ModelPtr mod, sdf::ElementPtr sdf){
			std::map<std::string, physics::CollisionPtr> collisions;
			std::vector<physics::LinkPtr> link_vec = mod->GetLinks();
			//printf("============%lu===============\n", link_vec.size());
			for(physics::LinkPtr l : link_vec){
				//printf("%s\n", l->GetScopedName().c_str());
				std::vector<physics::CollisionPtr> col_vec = l->GetCollisions();
				//printf("============%lu===============\n", col_vec.size());
				for(physics::CollisionPtr c : col_vec){
					//printf("%s\n", c->GetScopedName().c_str());
					collisions[c->GetScopedName()] = c;
				}
			}
		
			this->contact_node = transport::NodePtr(new transport::Node());
			this->contact_node->Init(mod->GetWorld()->Name());
			if (!collisions.empty()){
				// Create a filter to receive collision information
				physics::ContactManager * mgr = mod->GetWorld()->Physics()->GetContactManager();
				std::string topic = mgr->CreateFilter(mod->GetName(), collisions);
				if (!this->contact_sub){
					this->contact_sub = this->contact_node->Subscribe(topic, &FallGuy::ContactCB, this);
				}
			}
		
			//Set up ROS communication
			int a = 0;//No, it will NOT just accept an argument size of 0 without shenanigans. Annoying.
			ros::init(a, (char **) NULL, "fall_guy_node");
			this->statpub = this->nh.advertise<std_msgs::Bool>(
				"toggle_status", 1, true
			);
			
		}
	};
	GZ_REGISTER_MODEL_PLUGIN(FallGuy)
}
