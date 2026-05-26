#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"
using base_interfaces_demo::action::Progress;
using namespace std::placeholders;
using namespace std::chrono_literals;
class ServerNode:public rclcpp::Node{
public:
    ServerNode():Node("action_server_node_cpp"){
        RCLCPP_INFO(this->get_logger(),"成功创建动作通信的服务器");
        this->action_server = rclcpp_action::create_server<Progress>(
            this,
            "action_chatter",
            std::bind(&ServerNode::handle_goal,this,_1,_2),
            std::bind(&ServerNode::handle_cancel,this,_1),
            std::bind(&ServerNode::handle_accepted,this,_1));
        
    }

    
    // 3-2.处理请求数据；
    // 这个函数用于确定是否需要处理数据，传过来的请求是否合理，是否需要拒绝请求等。
    //这个地方的goal就是用户段传输的请求
    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &uuid,//传过来的这个请求的编号
        std::shared_ptr<const Progress::Goal> goal//就是客户端发送过来的具体请求内容
    ){
        // 业务逻辑：判断请求是否合法，是否需要拒绝请求等
        if (goal->num<=1){
            RCLCPP_WARN(this->get_logger(),"请求的数字不合法，必须大于1");
            return rclcpp_action::GoalResponse::REJECT;//拒绝请求
        }
        RCLCPP_INFO(this->get_logger(),"提交的目标数据合理");
        (void)uuid;
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;//接受请求并且执行
    }

    // 处理取消请求的回调函数
    //这个传入的参数就是对请求进行操作的一个对象
    rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(),"接收到任务取消请求");
        return rclcpp_action::CancelResponse::ACCEPT;//接受取消请求
    }

    // 生成连续反馈和最终反馈的回调函数
    // 这个和上面的一样这个goal_handle也是对请求进行操作的一个对象
    void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>>goal_handle){
        //获取目标之，然后遍历遍历的过程中进行累加，每次累加都发送一次反馈，最后发送一次结果
        int num=goal_handle->get_goal()->num;//获取目标数据
        auto feedback=std::make_shared<Progress::Feedback>();//创建一个反馈消息
        int sum=0;
        auto result=std::make_shared<Progress::Result>();//创建结果消息
        rclcpp::Rate rate(1s);//设置循环频率为1Hz
        for (int i=1;i<=num;i++){
            sum+=i;
            double progress=(double)i/(double)num;//计算进度
            feedback->progress=progress;//设置反馈消息的内容
            goal_handle->publish_feedback(feedback);//发布反馈消息
            RCLCPP_INFO(this->get_logger(),"已经计算到%d，当前进度:%.2f%%",i,progress*100);

            //是否当前有任务取消的请求
            bool flag=goal_handle->is_canceling();
            if(flag){
                result->sum=sum;//设置结果消息的内容
                goal_handle->canceled(result);//设置请求取消并且发送结果消息
                RCLCPP_ERROR(this->get_logger(),"任务被取消了");
                return;
            }

            rate.sleep();//按照设置的频率休眠
        }

        //生成最终的响应    
        if(rclcpp::ok()){
            
            result->sum=sum;//设置结果消息的内容
            goal_handle->succeed(result);//设置请求成功并且发送结果消息
            RCLCPP_INFO(this->get_logger(),"计算完成，结果是%d",sum);
        }
    }
    void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
        // (void)goal_handle;
        //新建一个线程来处理这个请求，避免阻塞主线程
        std::thread(std::bind(&ServerNode::execute,this,goal_handle)).detach();
    }
private:
    rclcpp_action::Server<Progress>::SharedPtr action_server;
    
};
int main(int argc,char ** argv){
    rclcpp::init(argc,argv);
    auto server_node=std::make_shared<ServerNode>();
    rclcpp::spin(server_node);
    rclcpp::shutdown();
    return 0;
}