# URDF directory
Holds URDF files that describe the robot.
Uses [XACRO](http://wiki.ros.org/xacro)(a feature in ROS) for macros and such to make the URDF simpler.

To generate a URDF file from XACRO file, run `./create_urdf.sh`.

Preview the robot with `roslaunch rviz.launch` and `roslaunch joints.launch` in two terminals.

To preview the current robot state, just launch rviz.launch.


# link & joint naming conventions
![](naming_convention.jpeg)
**ATT'N: This is wrong. The URDF is fixed.**

# making PCC robot
This should be done through the ./create_urdf program, but this section demonstrates how robot.urdf.xacro is constructed.

create *robot.urdf.xacro* with contents;
```xml
<?xml version="1.0"?>

<robot xmlns:xacro="http://www.ros.org/wiki/xacro" name="robot">
  <xacro:include filename="macro_definitions.urdf.xacro" />
  <xacro:empty_link name="base_link"/>
  <xacro:PCC id="0" parent="base_link" child="0_mid" length="0.5" mass="1000"/>
  <xacro:empty_link name="middle"/>
  <xacro:PCC id="1" parent="middle" child="end_link" length = "0.5" mass="1000" />
  <xacro:empty_link name="end_link"/>
</robot>
```
Note: the arm library can automatically generate *robot.urdf.xacro* using the length and mass parameters set in the code. This is done in *create_urdf.cpp*.

convert to URDF with `./create_urdf.sh` 

view with
`roslaunch rviz.launch` and `roslaunch joints.launch`

![](rviz.png)
