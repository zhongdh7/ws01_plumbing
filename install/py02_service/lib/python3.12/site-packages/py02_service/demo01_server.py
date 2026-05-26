import rclpy
from rclpy.node import Node
from base_interfaces_demo.srv import Addints

class ServiceNode(Node):
    def __init__(self):
        super().__init__('service_node')
        self.get_logger().info('Service Node have been initiated')
        self.server=self.create_service(Addints,"service_chatter",self.add)
    #编写回调函数
    #回调函数先写request后写response第一个的数据类型是Addints.Request 第二个数据类型是Addints.Response
    def add(self,request,response):
        response.sum=request.num1+request.num2
        self.get_logger().info(f"{request.num1}+{request.num2}={response.sum}")
        return response
def main():
    rclpy.init()
    service_node = ServiceNode()
    rclpy.spin(service_node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
