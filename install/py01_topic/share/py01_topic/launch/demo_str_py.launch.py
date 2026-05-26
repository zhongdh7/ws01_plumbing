from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    publisher = Node(
        package='py01_topic',
        executable='demo_str_py_publisher',
        name='demo_str_py_publisher',
        output='screen',
    )

    listener = Node(
        package='py01_topic',
        executable='demo_str_py_listener',
        name='demo_str_py_listener',
        output='screen',
    )

    return LaunchDescription([publisher, listener])