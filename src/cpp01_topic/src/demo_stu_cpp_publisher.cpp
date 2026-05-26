#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/msg/student.hpp"

using namespace std::chrono_literals;
class PublisherNode: public rclcpp::Node{
private:
    rclcpp::Publisher<base_interfaces_demo::msg::Student>::SharedPtr publisher;
    rclcpp::TimerBase::SharedPtr timer_;
    int count_;
public:
    PublisherNode(const std::string& str):Node(str.c_str()),count_(0)
    {
        RCLCPP_INFO(this->get_logger(),"发布方已创建....");
        publisher=this->create_publisher<base_interfaces_demo::msg::Student>("chatter",10);
        timer_=this->create_wall_timer(1s,std::bind(&PublisherNode::timer_callback,this));
    }
    void timer_callback()
    {
      // 3-3.组织消息并发布。
      auto stu = base_interfaces_demo::msg::Student();
      stu.name = "张三";
      stu.age = count_++;
      stu.height = 1.65;
      RCLCPP_INFO(this->get_logger(), "学生信息:name=%s,age=%d,height=%.2f", stu.name.c_str(),stu.age,stu.height);
      publisher->publish(stu);

    }
};

int main(int argc,char *argv[])
{
    rclcpp::init(argc,argv);

    auto node=std::make_shared<PublisherNode>(std::string("minipublisher_stu_cpp"));

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}