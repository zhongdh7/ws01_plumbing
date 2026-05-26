#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"
using base_interfaces_demo::action::Progress;
using namespace std::chrono_literals;
using namespace std::placeholders;
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
        auto goal=Progress::Goal();
        goal.num=num;
        rclcpp_action::Client<Progress>::SendGoalOptions options;
        options.goal_response_callback=std::bind(&ClientNode::goal_response_callback,this,std::placeholders::_1);
        options.feedback_callback=std::bind(&ClientNode::feedback_callback,this,std::placeholders::_1,std::placeholders::_2);
        options.result_callback=std::bind(&ClientNode::result_callback,this,std::placeholders::_1);
        auto future=client->async_send_goal(goal,options);
    }
    //处理目标之的服务端的响应
    void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle){
        //判断目标值是否被服务端接受
        if(!goal_handle){
            RCLCPP_ERROR(this->get_logger(),"目标被服务端拒绝");
        }
        else{
            RCLCPP_INFO(this->get_logger(),"目标处理中");
        }
    }
    //处理连续反馈
    void feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle,
        const std::shared_ptr<const Progress::Feedback> feedback){
        (void)goal_handle;
        double progress=feedback->progress;
        RCLCPP_INFO(this->get_logger(),"当前进度:%.2f%%",progress*100);
    }
    //处理最终结果
    void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult& result){
        // (void)result;
        //通过状态码判断结果状态
        rclcpp_action::ResultCode code=result.code;
        switch(code){
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(),"最终结果:%d",result.result->sum);
                break;
            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(),"目标执行失败");
                break;
            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_ERROR(this->get_logger(),"目标被取消");
                break;
            default:
                RCLCPP_ERROR(this->get_logger(),"未知状态码");
                break;
        }

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