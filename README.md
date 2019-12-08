# MPC_AutonomousPlanning

MPC path planning and obstacle avoidance for an autonomous vehicle, part of the final project for the course titled ESE680-007: Autonomous Racing, taken during fall 2019 at the University of Pennsylvania.

# Dependencies
1. Install Google cartographer for building the map of the desired racetrack: https://github.com/googlecartographer 
2. Install the particle filter used for localization: https://github.com/mit-racecar/particle_filter
3. Install the racecar simulator from the course's repo: https://github.com/mlab-upenn/f110-fall2019-skeletons
4. Generate waypoints using wapoint logger: https://github.com/mlab-upenn/f110-fall2019-skeletons

# How to use this repo
1. Generate the yaml and pgm files for the racetrack map using google cartographer. Save and update the files appropriately inside the particle filter folders. Alternatively, use the default maps inside "map" folder.
2. Generate desired waypoints using the waypoint\_logger.py file from Dependencies(3). Save the waypoint file inside the waypoint\_CSVs folder of this repo. Alternatively, use the default waypoints inside "waypoint\_CSVs" folder.
3. Update the parameter file called "mpc\_planning\_params" inside this repo to the correct file address of the desired waypoint file.
3. Update the drive and lidar topics appropriately.
4. Optional - update the map files in the racecar simulator to test the MPC capabilities on the simulator.

Note: We use CVXGEN for MPC optimization, and is used under an academic license. 

# How to run this package

* Launch the particle filter 
~~~
    roslaunch particle_filter localize.launch
~~~
* Localize the vehicle on the RVIZ GUI
~~~
    rosrun rviz rviz
~~~
* Launch the MPC package
~~~
    roslaunch mpc_auto mpc_auto.launch
~~~
# In action

Static obstacle avoidance in action, with a nominal velocity of 6 m/s:

<p align="center">
  <img src=media/levine.gif>
</p>

Static obstacle avoidance with a nominal velocity of 4m/s:

<p align="center">
  <img src=media/skirkanich.gif>
</p>

# Contact Information
For questions and queries email me at anmolkathail@gmail.com

