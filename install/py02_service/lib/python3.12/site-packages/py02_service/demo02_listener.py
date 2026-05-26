import rclpy
from rclpy.node import Node
from rclpy.logging import get_logger
from base_interfaces_demo.srv import Addints
import sys

class ListenerNode(Node):
    def __init__(self):
        super().__init__('listener_node')
        self.get_logger().info('Listener node has been started.')
        self.client=self.create_client(Addints,"service_chatter")

    def connect_server(self):
        while not self.client.wait_for_service(1.0):
            #这一段可以不需要
            # if not rclpy.ok():
            #     get_logger("rclpy").error("强制退出")
            #     return False
            get_logger("rclpy").info("正在尝试连接服务端")
        self.get_logger().info("成功连接上服务端口")
        return True
    

    def send_request(self,num1,num2):
        request=Addints.Request()
        request.num1=num1
        request.num2=num2
        self.future=self.client.call_async(request)#这个步骤就是发送请求

def main():
    #这个argv就类似于C++当中的argv字符串数组
    if len(sys.argv) != 3:
        get_logger("rclpy").error("请提交两个整型数据")
        return
    rclpy.init()
    client=ListenerNode()
    # 判断提交的参数是否正确
    if not client.connect_server():
        get_logger("rclpy").info("连接失败")
        return False
    client.send_request(int(sys.argv[1]),int(sys.argv[2]))
    #这一行代码会一直等待，直到服务器处理完请求并且返回结果，结果会在future当中
    rclpy.spin_until_future_complete(client,client.future)
    try: 
        #这个会吧future的结果获取到的结果发送到response当中，如果成功那么就可以正常进行
        response=client.future.result()
        #这个是响应的对象
        client.get_logger().info(f"sum={response.sum}")
    except Exception as e:
        get_logger("rclpy").error("请求失败")
    
    rclpy.shutdown()

if __name__ == '__main__':
    main()