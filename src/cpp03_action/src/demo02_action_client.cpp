#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"
using base_interfaces_demo::action::Progress;
using namespace std::chrono_literals;
class ClientNode:public rclcpp::Node{
public:
    ClientNode():Node("action_client_node_cpp"){
        RCLCPP_INFO(this->get_logger(),"成功创建动作通信的客户端");
        this->client=rclcpp_action::create_client<Progress>(this,"action_chatter");
    }
    //发送消息
    void send_goal(int num){
        (void)num;
        //确保连接到服务端
        if(!client->wait_for_action_server(10s)){
            RCLCPP_ERROR(this->get_logger(),"连接服务端失败");
            return;
        }
        //发送具体请求
    }
private:
    rclcpp_action::Client<Progress>::SharedPtr client;
};
int main(int argc,char * argv[]){
    if(argc!=2){
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"提交一个整型数据");
        return -1;
    }
    rclcpp::init(argc,argv);
    auto client=std::make_shared<ClientNode>();
    client->send_goal(atoi(argv[1]));
    rclcpp::spin(client);
    rclcpp::shutdown();
    return 0;
}