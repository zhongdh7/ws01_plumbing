import rclpy
from rclpy.node import Node
from base_interfaces_demo.action import Progress
from rclpy.action import ActionServer,CancelResponse
import time

def main():
    # 初始化客户端
    rclpy.init()
    
    temp = ServerNode()
    rclpy.spin(temp)

    rclpy.shutdown()

class ServerNode(Node):
    def __init__(self):
        super().__init__("action_server_node_py")
        self.get_logger().info("创建动作通信服务端")

        self.server=ActionServer(self,Progress,"action_chatter",self.execute_callback,cancel_callback=self.default_cancel_callback)

    def execute_callback(self, goal_handle):
        self.get_logger().info("接收到客户端发送的目标请求")
        #生成连续反馈
        num=goal_handle.request.num
        sum=0
        feedback_msg=Progress.Feedback()
        for i in range(1,num+1):
            sum+=i
            feedback_msg.progress=i/num
            goal_handle.publish_feedback(feedback_msg)
            self.get_logger().info(f"目前进度为：{feedback_msg.progress*100:.2f}%")
            time.sleep(1)

        #相应最终结果
        goal_handle.succeed()
        result=Progress.Result()
        result.sum=sum
        self.get_logger().info(f"最终结果为：{result.sum}")
        return result
    def default_cancel_callback(cancel_request):
    
        return CancelResponse.ACCEPT

if __name__ == "__main__":
    main()