#include <iostream>
#include <thread>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executors.hpp>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <status_interface/msg/system_status.hpp>
using namespace std;
using  SystemStatus = status_interface::msg::SystemStatus;

class SystemDisplay:public rclcpp::Node{
    //初始化构造函数
    public:

    SystemDisplay(): rclcpp::Node("SystemDisplay"){
        //初始化占位
        label = new QLabel();
        label->setText(get_msg_status(std::make_shared<SystemStatus>()));
        //当订阅到信息时，就会启用这个函数
        subscription_=this->create_subscription<SystemStatus>("sys_status",rclcpp::QoS(10),[&](const SystemStatus::SharedPtr msg)->void{
        QString text=get_msg_status(msg);
        QMetaObject::invokeMethod(label,[this,text]()->void{
            this->label->setText(text);
        },Qt::QueuedConnection);
        });
        label->show();
    };

    private:

        QString get_msg_status(const SystemStatus::SharedPtr msg){
        stringstream showptr;
        showptr<<"========系统可视化========"
        "时间辍:"<<msg->stamp.sec<<
        "主机名字:"<<msg->host_name<<
        "cpu使用率:"<<msg->cpu_percent<<
        "内存使用率:"<<msg->memory_percent<<
        "内存总大小:"<<msg->memory_total<<
        "可用内存大小:"<<msg->memory_available<<
        "网络发送量:"<<msg->net_sent<<
        "网络接受量:"<<msg->net_recv<<
        "==========================";
        return QString::fromStdString(showptr.str());
    }

    //成员变量
    rclcpp::Subscription<SystemStatus>::SharedPtr subscription_;
    QLabel* label;

};

int main(int argc,char** argv){
    //初始化
    rclcpp::init(argc,argv);
    QApplication app(argc,argv);
    auto node =make_shared<SystemDisplay>();
    //初始化多线程执行器
    auto executor = make_shared<rclcpp::executors::MultiThreadedExecutor>();
    //将节点加入到执行器中。执行器现在开始管理这个节点内的所有订阅、定时器等回调
    executor->add_node(node);
    //开新的线程，防止阻塞代码
    thread spin_thread([executor]()->void{
        executor->spin();
    });
    //将后台线程与主线程分离（Detach）。这样主线程不会卡在等待子线程结束，可以继续往下走
    spin_thread.detach();
    //分离后才运行另外个
    app.exec();
    executor->cancel();
    rclcpp::shutdown();
    return 0;
}