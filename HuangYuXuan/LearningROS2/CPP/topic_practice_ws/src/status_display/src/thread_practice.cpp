#include <iostream>
#include <memory>
#include <thread>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executor.hpp>
using namespace std;

int main(int argc,char** argv){
    rclcpp::init(argc,argv);
    auto node=make_shared<rclcpp::Node>("Start Thread");
    auto executor = make_shared<rclcpp::executors::MultiThreadedExecutor>();
    executor->add_node(node);
    thread spin_thread([executor,node]()->void{
        //如果有其他线程，这里就是executor->spin();
        rclcpp::spin(node);

    });
    spin_thread.detach();
    executor->cancel();
    rclcpp::shutdown();
    return 0;
}