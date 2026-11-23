import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('rm_serial_driver'), 'config', 'serial_driver.yaml')

    use_virtual_low_board = LaunchConfiguration('use_virtual_low_board')

    rm_serial_driver_node = Node(
        package='rm_serial_driver',
        executable='rm_serial_driver_node',
        namespace='',
        output='screen',
        emulate_tty=True,
        parameters=[
            config,
            {'use_virtual_low_board': ParameterValue(use_virtual_low_board, value_type=bool)},
        ],
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_virtual_low_board',
            default_value='false',
            description='Read frames from /receive_packet instead of a serial port'),
        rm_serial_driver_node,
    ])
