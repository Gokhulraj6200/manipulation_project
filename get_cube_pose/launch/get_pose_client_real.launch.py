from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():

    pkg_share_path = get_package_share_directory('get_cube_pose')
    rviz_config_file = os.path.join(pkg_share_path, 'rviz', 'real.rviz')

    return LaunchDescription([
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen',
            arguments=['-d', rviz_config_file]
        ),
        Node(
            package='simple_grasping',
            executable='basic_grasping_perception_node',
            name='basic_grasping_perception_node',
            output='screen',
            parameters=[{'debug_topics': True}],
            remappings=[('/wrist_rgbd_depth_sensor/points', '/camera/depth/color/points')]
        ),
        Node(
            package='get_cube_pose',
            executable='get_pose_client',
            name='get_pose_client',
            output='screen'
        ),
    ])