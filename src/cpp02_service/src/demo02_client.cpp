/*  
  需求：编写客户端，发送两个整型变量作为请求数据，并处理响应结果。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建客户端；
      3-2.等待服务连接；
      3-3.组织请求数据并发送；
    4.创建对象指针调用其功能,并处理响应；
    5.释放资源。

*/
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/addints.hpp"
using base_interfaces_demo::srv::Addints;
using namespace std::chrono_literals;
class ClientNode:public rclcpp::Node{
public:
    ClientNode():Node("add_ints_client_node_cpp"){
        RCLCPP_INFO(this->get_logger(),"成功创建服务通信的客户端");
        //这个地方创建客户端的时候只需要传入服务数据类型然后传入话题就可以了
        //返回值还是一个SharedPtr
        client=this->create_client<Addints>("service_chatter");

        //连接服务器
        
    }
    //判断链接服务器实现，链接成功返回true 否则返回false
    bool connect_server(){
        //在指定超时时间内链接服务器，如果链接上了那么返回true否则返回false
        // client->wait_for_service(s)
        
        //循环一直等待连接上
        //链接不上就一直等待链接上
        //循环以1s为超时时间连接服务器直到连接上服务器再退出服务器
        while(!client->wait_for_service(1s)){
            //对ctrl+c中断信号进行处理，如果接收到了中断信号就退出程序
            //判断是否这个程序在正常执行中，如果正常执行就是没有按下ctrl+c的情况下返回true,否则返回false
            //按下ctrl+c意味着结束程序意味着会rclcpp::shutdown()
            if(!rclcpp::ok()){
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"强行终止客户端");
                return false;
            }
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务链接中");
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"连接上了服务器");
        return true;
    }
    //发送请求
    //编写请求函数
    rclcpp::Client<Addints>::FutureAndRequestId send_request(int num1,int num2){
        auto request=std::make_shared<Addints::Request>();
        request->num1=num1;
        request->num2=num2;
        //发送
        return client->async_send_request(request);
    }
private:
    rclcpp::Client<Addints>::SharedPtr client;
};
int main(int argc,char * argv[]){
    if(argc!=3){
        //这个地方因为没有初始化不能用Node里面的get_logger只能用rclcpp::get_logger
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"请提交两个整型数字");
        return 1;
    }
    rclcpp::init(argc,argv);

    auto node=std::make_shared<ClientNode>();
    // rclcpp::spin(node);
    //调用客户端对象的连接服务器功能
    bool flag=node->connect_server();
    //根据连接结果进一步处理
    if(!flag){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务器链接失败，程序退出");
        return 0;
    }
    //调用请求提交函数，接受并且处理提交结果
    //这个地方atoi的作用是把c风格的字符串转化成int类型的数据
    //这个第一个0的位置的数据是可执行程序的名字
    auto future=node->send_request(std::atoi(argv[1]),std::atoi(argv[2]));
    //处理响应
    //这个地方是判断发送请求的状态码的
    if (rclcpp::spin_until_future_complete(node,future)==rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(node->get_logger(),"响应成功！sum=%d",future.get()->sum);
    }
    else{
        RCLCPP_INFO(node->get_logger(),"响应失败");
    }
    
    rclcpp::shutdown();
    return 0;
}
