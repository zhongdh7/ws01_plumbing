"""  
    需求：订阅发布方发布的学生消息，并输出到终端。
"""

# 1.导包；
import rclpy
from rclpy.node import Node
from base_interfaces_demo.msg import Student

# 3.定义节点类；
class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('stu_subscriber_py')
        self.get_logger().info("订阅方已启动，准备接收消息...")
        # 3-1.创建订阅方；
        self.subscription = self.create_subscription(
            Student,
            'topic_stu',
            self.listener_callback,
            10)
        self.subscription  

    # 3-2.处理订阅到的消息。
    def listener_callback(self, stu):
        self.get_logger().info('订阅的消息(py): name=%s,age=%d,height=%.2f' % (stu.name, stu.age, stu.height))


def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)

    # 4.调用spin函数，并传入节点对象；
    minimal_subscriber = MinimalSubscriber()
    rclpy.spin(minimal_subscriber)

    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()