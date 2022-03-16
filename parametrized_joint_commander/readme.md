To install, unzip and put in your ros_ws/src folder. Run catkin_make, and restart the terminal.

To run, use the following on the command line:

rosrun parametrized_joint_commander joint_commander

Or add the following to any launch file.
<node name="anything" pkg="parametrized_joint_commander" type="joint_commander"/>

The motions of each joint are set up in src/joint_commander.cpp. I've included a few basic examples. Changing them requires rerunning catkin_make after the changes are made, but not restarting the terminal.

If you'd like, you can use the tool rqt_plot to visualize the joint commands- rosrun rqt_plot rqt_plot. This will produce a window with a text bar on top; the names of each of the joint commands which can go in here are included in lines 89 through 107 in the cpp file.
