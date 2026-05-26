import rclpy
from rclpy.node import Node
import sys
from rclpy.logging import get_logger
from base_interfaces_demo.action import Progress
from rclpy.action import ActionClient

def main():
    #动态解析传入的参数
    if len(sys.argv) != 2:
        get_logger("rclpy").error("请传入一个整数参数，表示要求和的范围")
        return
    num = int(sys.argv[1])
    # 初始化客户端
    rclpy.init()
    
    temp = ClientNode()
    temp.send_goal(num)
    rclpy.spin(temp)

    rclpy.shutdown()

class ClientNode(Node):
    def __init__(self):
        super().__init__("action_client_node_py")
        self.get_logger().info("动作通信用户端创建")
        self.client=ActionClient(self,Progress,"action_chatter")

    def send_goal(self,num):
        self.get_logger().info("等待服务端连接")
        self.client.wait_for_server()
        self.get_logger().info("服务端连接成功，发送目标请求")
        goal_msg=Progress.Goal()
        goal_msg.num=num
        self.future=self.client.send_goal_async(goal_msg,feedback_callback=self.feedback_callback)
        self.future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self,future):
        goal_handle=future.result()
        if not goal_handle.accepted:
            self.get_logger().error("目标请求被服务端拒绝")
            return
        self.get_logger().info("目标请求被服务端接受，等待结果")

        #处理最终结果
        self.result_future=goal_handle.get_result_async()
        self.result_future.add_done_callback(self.get_result_callback)
    
    def get_result_callback(self,future):
        result=future.result().result
        self.get_logger().info(f"最终结果：{result.sum}")
        
    
    def feedback_callback(self,feedback_msg):
        feedback=feedback_msg.feedback
        self.get_logger().info(f"接收到服务端发送的反馈：{feedback.progress*100:.2f}%")



if __name__ == "__main__":
    main()