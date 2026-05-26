"""  
    需求：以某个固定频率发送文本学生信息，包含学生的姓名、年龄、身高等数据。
"""
# 1.导包；
import rclpy
from rclpy.node import Node
from base_interfaces_demo.msg import Student

# 3.定义节点类；
class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('stu_publisher_py')
        self.get_logger().info("发布方已启动，正在发布消息...")
        # 3-1.创建发布方；
        self.publisher_ = self.create_publisher(Student, 'topic_stu', 10)
        # 3-2.创建定时器；
        timer_period = 0.5 
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.i = 0

    # 3-3.组织消息并发布。
    def timer_callback(self):
        stu = Student()
        stu.name = "李四"
        stu.age = self.i
        stu.height = 1.70
        self.publisher_.publish(stu)
        self.get_logger().info('发布的学生消息(py): name=%s,age=%d,height=%.2f' % (stu.name, stu.age, stu.height))
        self.i += 1


def main(args=None):
    # 2.初始化 ROS2 客户端；
    rclpy.init(args=args)
    # 4.调用spin函数，并传入节点对象；
    minimal_publisher = MinimalPublisher()
    rclpy.spin(minimal_publisher)
    # 5.释放资源。
    rclpy.shutdown()


if __name__ == '__main__':
    main()