#include <chrono>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/display_robot_state.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>
#include <thread>

static const rclcpp::Logger LOGGER = rclcpp::get_logger("move_group_demo");

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions node_options;
  node_options.automatically_declare_parameters_from_overrides(true);
  auto move_group_node =
      rclcpp::Node::make_shared("move_group_interface_tutorial", node_options);

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(move_group_node);
  std::thread([&executor]() { executor.spin(); }).detach();

  static const std::string PLANNING_GROUP_ARM = "ur_manipulator";
  static const std::string PLANNING_GROUP_GRIPPER = "gripper";

  moveit::planning_interface::MoveGroupInterface move_group_arm(
      move_group_node, PLANNING_GROUP_ARM);
  moveit::planning_interface::MoveGroupInterface move_group_gripper(
      move_group_node, PLANNING_GROUP_GRIPPER);

  const moveit::core::JointModelGroup *joint_model_group_arm =
      move_group_arm.getCurrentState()->getJointModelGroup(PLANNING_GROUP_ARM);
  const moveit::core::JointModelGroup *joint_model_group_gripper =
      move_group_gripper.getCurrentState()->getJointModelGroup(
          PLANNING_GROUP_GRIPPER);

  // Get Current State
  moveit::core::RobotStatePtr current_state_arm =
      move_group_arm.getCurrentState(10);
  moveit::core::RobotStatePtr current_state_gripper =
      move_group_gripper.getCurrentState(10);

  std::vector<double> joint_group_positions_arm;
  std::vector<double> joint_group_positions_gripper;
  current_state_arm->copyJointGroupPositions(joint_model_group_arm,
                                             joint_group_positions_arm);
  current_state_gripper->copyJointGroupPositions(joint_model_group_gripper,
                                                 joint_group_positions_gripper);

  move_group_arm.setStartStateToCurrentState();
  move_group_gripper.setStartStateToCurrentState();

  // Go Home
  RCLCPP_INFO(LOGGER, "Going Home");

  joint_group_positions_arm[0] = 0.00;  // Shoulder Pan
  joint_group_positions_arm[1] = -2.50; // Shoulder Lift
  joint_group_positions_arm[2] = 1.50;  // Elbow
  joint_group_positions_arm[3] = -1.50; // Wrist 1
  joint_group_positions_arm[4] = -1.55; // Wrist 2
  joint_group_positions_arm[5] = 0.00;  // Wrist 3

  move_group_arm.setJointValueTarget(joint_group_positions_arm);

  moveit::planning_interface::MoveGroupInterface::Plan my_plan_arm;
  bool success_arm = (move_group_arm.plan(my_plan_arm) ==
                      moveit::core::MoveItErrorCode::SUCCESS);

  move_group_arm.execute(my_plan_arm);

  // Pregrasp
  RCLCPP_INFO(LOGGER, "Pregrasp Position");

  //   geometry_msgs::msg::Pose target_pose1;
  //   target_pose1.orientation.x = -1.0;
  //   target_pose1.orientation.y = 0.00;
  //   target_pose1.orientation.z = 0.00;
  //   target_pose1.orientation.w = 0.00;
  //   target_pose1.position.x = 0.345;
  //   target_pose1.position.y = 0.131;
  //   target_pose1.position.z = 0.264;
  //   move_group_arm.setPoseTarget(target_pose1);

  //   success_arm = (move_group_arm.plan(my_plan_arm) ==
  //                  moveit::core::MoveItErrorCode::SUCCESS);

  //   move_group_arm.execute(my_plan_arm);

  joint_group_positions_arm[0] = -0.0;     // Shoulder Pan
  joint_group_positions_arm[1] = -1.36136; // Shoulder Lift
  joint_group_positions_arm[2] = 1.53589;  // Elbow
  joint_group_positions_arm[3] = -1.7348;  // Wrist 1
  joint_group_positions_arm[4] = -1.5708;  // Wrist 2
  joint_group_positions_arm[5] = -1.5708;  // Wrist 3

  move_group_arm.setJointValueTarget(joint_group_positions_arm);

  success_arm = (move_group_arm.plan(my_plan_arm) ==
                 moveit::core::MoveItErrorCode::SUCCESS);

  move_group_arm.execute(my_plan_arm);

  // Open Gripper

  RCLCPP_INFO(LOGGER, "Open Gripper!");

  move_group_gripper.setNamedTarget("gripper_open");

  moveit::planning_interface::MoveGroupInterface::Plan my_plan_gripper;
  bool success_gripper = (move_group_gripper.plan(my_plan_gripper) ==
                          moveit::core::MoveItErrorCode::SUCCESS);

  move_group_gripper.execute(my_plan_gripper);

  // Approach
  RCLCPP_INFO(LOGGER, "Approach to object!");

  //   std::vector<geometry_msgs::msg::Pose> approach_waypoints;
  //   target_pose1.position.z -= 0.04;
  //   approach_waypoints.push_back(target_pose1);

  //   target_pose1.position.z -= 0.04;
  //   approach_waypoints.push_back(target_pose1);

  //   moveit_msgs::msg::RobotTrajectory trajectory_approach;
  //   const double jump_threshold = 0.0;
  //   const double eef_step = 0.01;

  //   double fraction = move_group_arm.computeCartesianPath(
  //       approach_waypoints, eef_step, jump_threshold, trajectory_approach);

  //   move_group_arm.execute(trajectory_approach);

  joint_group_positions_arm[0] = -0.0;     // Shoulder Pan
  joint_group_positions_arm[1] = -1.22428; // Shoulder Lift
  joint_group_positions_arm[2] = 1.69297;  // Elbow
  joint_group_positions_arm[3] = -2.04204; // Wrist 1
  joint_group_positions_arm[4] = -1.5708;  // Wrist 2
  joint_group_positions_arm[5] = -1.5708;  // Wrist 3

  move_group_arm.setJointValueTarget(joint_group_positions_arm);

  success_arm = (move_group_arm.plan(my_plan_arm) ==
                 moveit::core::MoveItErrorCode::SUCCESS);

  move_group_arm.execute(my_plan_arm);
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Close Gripper

  RCLCPP_INFO(LOGGER, "Close Gripper!");

  //   joint_group_positions_gripper[2] = 0.650;

  //   move_group_gripper.setJointValueTarget(joint_group_positions_gripper);

  //   success_gripper = (move_group_gripper.plan(my_plan_gripper) ==
  //                      moveit::core::MoveItErrorCode::SUCCESS);

  //   move_group_gripper.execute(my_plan_gripper);

  //   std::this_thread::sleep_for(std::chrono::seconds(2));

  float gripper_value = 0.620;
  while (gripper_value <= 0.668) {
    joint_group_positions_gripper[2] = gripper_value;
    move_group_gripper.setJointValueTarget(joint_group_positions_gripper);
    RCLCPP_INFO(LOGGER, "Closing gripper: %.3f.", gripper_value);
    success_gripper = (move_group_gripper.plan(my_plan_gripper) ==
                       moveit::core::MoveItErrorCode::SUCCESS);
    if (success_gripper) {
      RCLCPP_INFO(LOGGER, "Plan to actuate gipper: SUCCESS.");
      RCLCPP_INFO(LOGGER, "Executing command.");
      move_group_arm.execute(my_plan_gripper);
      gripper_value += 0.001;
    } else {
      RCLCPP_INFO(LOGGER, "Plan to actuate gipper: FAIL.");
      RCLCPP_INFO(LOGGER, "Aborting command.");
      rclcpp::shutdown();
      return 1;
    }
  }
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // Retreat

  RCLCPP_INFO(LOGGER, "Retreat from object!");

  // std::vector<geometry_msgs::msg::Pose> retreat_waypoints;
  // target_pose1.position.z += 0.04;
  // retreat_waypoints.push_back(target_pose1);

  // target_pose1.position.z += 0.04;
  // retreat_waypoints.push_back(target_pose1);

  // moveit_msgs::msg::RobotTrajectory trajectory_retreat;

  // fraction = move_group_arm.computeCartesianPath(
  //     retreat_waypoints, eef_step, jump_threshold, trajectory_retreat);

  // move_group_arm.execute(trajectory_retreat);

  joint_group_positions_arm[0] = -0.0;     // Shoulder Pan
  joint_group_positions_arm[1] = -1.36136; // Shoulder Lift
  joint_group_positions_arm[2] = 1.53589;  // Elbow
  joint_group_positions_arm[3] = -1.7348;  // Wrist 1
  joint_group_positions_arm[4] = -1.5708;  // Wrist 2
  joint_group_positions_arm[5] = -1.5708;  // Wrist 3

  move_group_arm.setJointValueTarget(joint_group_positions_arm);

  success_arm = (move_group_arm.plan(my_plan_arm) ==
                 moveit::core::MoveItErrorCode::SUCCESS);

  move_group_arm.execute(my_plan_arm);

  // Place

  RCLCPP_INFO(LOGGER, "Rotating Arm");

  //   current_state_arm = move_group_arm.getCurrentState(10);
  //   current_state_arm->copyJointGroupPositions(joint_model_group_arm,
  //                                              joint_group_positions_arm);

  //   joint_group_positions_arm[0] = 1.0; // Shoulder Pan

  //   move_group_arm.setJointValueTarget(joint_group_positions_arm);

  //   success_arm = (move_group_arm.plan(my_plan_arm) ==
  //                  moveit::core::MoveItErrorCode::SUCCESS);

  //   move_group_arm.execute(my_plan_arm);

  joint_group_positions_arm[0] = -3.14159; // Shoulder Pan
  joint_group_positions_arm[1] = -1.36136; // Shoulder Lift
  joint_group_positions_arm[2] = 1.53589;  // Elbow
  joint_group_positions_arm[3] = -1.7348;  // Wrist 1
  joint_group_positions_arm[4] = -1.5708;  // Wrist 2
  joint_group_positions_arm[5] = -1.5708;  // Wrist 3

  move_group_arm.setJointValueTarget(joint_group_positions_arm);

  success_arm = (move_group_arm.plan(my_plan_arm) ==
                 moveit::core::MoveItErrorCode::SUCCESS);

  move_group_arm.execute(my_plan_arm);

  // Open Gripper

  RCLCPP_INFO(LOGGER, "Release Object!");

  move_group_gripper.setNamedTarget("gripper_open");

  success_gripper = (move_group_gripper.plan(my_plan_gripper) ==
                     moveit::core::MoveItErrorCode::SUCCESS);

  move_group_gripper.execute(my_plan_gripper);

  rclcpp::shutdown();
  return 0;
}