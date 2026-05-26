#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/addints.hpp"

using namespace std::placeholders;
class PublisherNode:public rclcpp::Node{
private:
  rclcpp::Service<base_interfaces_demo::srv::Addints>::SharedPtr server;
  void add(const base_interfaces_demo::srv::Addints::Request::SharedPtr req,const base_interfaces_demo::srv::Addints::Response::SharedPtr res)
  {
    res->sum=req->num1+req->num2;
    RCLCPP_INFO(this->get_logger(),"%d+%d=%d",req->num1,req->num2,res->sum);
  }

public:
  PublisherNode():Node("add_ints_server_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"建立服务通信的服务端口.....");
    server=this->create_service<base_interfaces_demo::srv::Addints>("service_chatter",std::bind(&PublisherNode::add,this,_1,std::placeholders::_2));
  }
};
int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PublisherNode>());
  rclcpp::shutdown();
  return 0;
}
