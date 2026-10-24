import rclpy
from rclpy.node import Node
from std_msgs.msg import String  # 类型要和 wang2 保持一致

class WriterNode(Node):
    def __init__(self, name):
        super().__init__(name)
        # 1. 创建发布者，话题名为 "sexy_girl"，类型为 String
        self.pub_novel = self.create_publisher(String, "sexy_girl", 10)
        
        # 2. 创建一个定时器，每 2 秒调用一次 timer_callback 发送消息
        self.timer = self.create_timer(2.0, self.timer_callback)

    def timer_callback(self):
        # 3. 构造并发布消息
        msg = String()
        msg.data = "这是我写的字符串！、；‘】"
        self.pub_novel.publish(msg)
        self.get_logger().info(f"li4 发布了：{msg.data}")

def main(args=None):
    rclpy.init(args=args)
    node = WriterNode("li4")
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()



