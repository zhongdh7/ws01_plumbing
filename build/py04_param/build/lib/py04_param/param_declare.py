import rclpy
from rclpy.node import Node

def main():
    # 初始化客户端
    rclpy.init()
    
    temp = ParamNode("param_device")
    rclpy.spin(temp)
    temp.destroy_node()
    rclpy.shutdown()

class ParamNode(Node):
    def __init__(self,name="param_node_py"):
        super().__init__(name)
        self.get_logger().info("创建参数通信节点python")
        self.timer=self.create_timer(timer_period_sec=1.0,callback=self.timer_callback)
        self.declare_parameter("robot_name","mbot")#创建一个参数，参数名为robot_name，默认值为mbot
    def timer_callback(self):
        robot_name_param=self.get_parameter("robot_name").get_parameter_value().string_value
        self.get_logger().info(f"当前机器人名称：{robot_name_param}")
        # new_name_param=rclpy.parameter.Parameter("robot_name",rclpy.Parameter.Type.STRING,"mbot")#创建一个新的参数对象，参数名为robot_name，类型为字符串，值为mbot
        # self.set_parameters([new_name_param])#设置参数，传入一个参数对象列表
if __name__ == "__main__":
    main()