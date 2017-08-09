cmake_minimum_required(VERSION 2.8.3)
project(navigation_stage)




# Find catkin


find_package(catkin REQUIRED COMPONENTS
  tf
  nav_msgs
  roscpp
  rospy
  std_msgs
  actionlib
  actionlib_msgs
  geometry_msgs
  move_base_msgs
  genmsg
)

#find_package(catkin REQUIRED genmsg actionlib_msgs actionlib)
#add_action_files(DIRECTORY action FILES DoDishes.action)
#generate_messages(DEPENDENCIES actionlib_msgs)


catkin_package(CATKIN_DEPENDS
  tf
  roscpp
	std_msgs
	nav_msgs
	actionlib
  geometry_msgs
  move_base_msgs
	rospy)

  include_directories(
  ${catkin_INCLUDE_DIRS}
)

#catkin_install_python(PROGRAMS scripts/teleop_twist_keyboard.py
#                      DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


add_executable(PreyPath src/PreyPath.cpp)
add_executable(robot_loc src/robot_loc.cpp)
add_executable(RequestGoal src/RequestGoal.cpp)
#add_executable(forward src/forward.cpp)
#add_executable(navigation_stage src/GiveGoal.cpp)

target_link_libraries(PreyPath ${catkin_LIBRARIES})
target_link_libraries(RequestGoal ${catkin_LIBRARIES})
#target_link_libraries(forward ${catkin_LIBRARIES})
target_link_libraries(robot_loc ${catkin_LIBRARIES})

# Install launch files
install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

# Install move_base_config files
install(DIRECTORY move_base_config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

# Install stage_config files
install(DIRECTORY stage_config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

# Install the rviz files
install(FILES multi_robot.rviz single_robot.rviz
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
