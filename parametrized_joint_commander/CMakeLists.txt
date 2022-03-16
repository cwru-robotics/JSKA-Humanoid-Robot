cmake_minimum_required(VERSION 2.8.3)
project(parametrized_joint_commander)

find_package(catkin REQUIRED COMPONENTS 
  roscpp 
  gazebo_ros 
)

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)

include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

link_directories(${GAZEBO_LIBRARY_DIRS})

find_package(Boost REQUIRED COMPONENTS system thread)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

catkin_package(
  DEPENDS 
    roscpp 
    gazebo_ros 
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp gazebo_ros
)

# Executable
add_executable(joint_commander src/joint_commander.cpp)

target_link_libraries(joint_commander ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

#Fall on collision plugin.
add_library(fall_guy SHARED src/fall_guy.cpp)
target_link_libraries(fall_guy
  ${GAZEBO_LIBRARIES}
)
