#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include <QApplication>
#include <QLabel>
#include <QString>

int main(int argc,char** argv){
    //初始化
    rclcpp::init(argc,argv);
    QApplication app(argc,argv);
    //建立标签指针
    QLabel* label = new QLabel();
    QString message = QString::fromStdString("Display Qt");
    label->setText(message);
    label->show();
    //执行应用,阻塞代码
    app.exec();
    return 0;
}