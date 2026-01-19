
This page is a work in progress

# 🛠️ Setting up the Simulation

You first need a ros2 jazzy workspace. If you don't have one yet, follow the instructions in the [Setting up ROS2](ros2_setup.md) page.

Then, follow these steps to set up the simulation environment:

navigate to your src folder of your ros2 workspace and clone both repos:

```bash
cd ~/ros2_ws/src
git clone https://github.com/ECN-Nantrobot/nantrobot_robot_sim.git
git clone https://github.com/ECN-Nantrobot/nantrobot_rviz_panel.git
```

Then install all the dependencies:

```bash
cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y
```

Finally, build your workspace:

```bash
colcon build --symlink-install
source install/setup.bash
```
You can now run the simulation by launching the following command:

```bash
ros2 launch nantrobot_robot_sim two_robot_sim.launch.py
```

It should launch gazebo and rviz and spawn two robots.
Rviz should look something like this:
![rviz_simulation](../assets/Software/ros2_simulation/rviz_simulation.png)
