# libypspur - joint control

## Get joint status

### Name

Spur_get_joint_ang, Spur_get_joint_vel, Spur_get_joint_torque

### Synopsis

```c
double Spur_get_joint_ang(int id, double *a);
double Spur_get_joint_vel(int id, double *v);
double Spur_get_joint_torque(int id, double *t);
```

### Description

Get current joint state.

* **id**: joint (motor) id
* **\*a**: pointer to store angle of the joint _[rad]_
* **\*v**: pointer to store angular velocity of the joint _[rad/s]_
* **\*t**: pointer to store torque of the joint _[Nm]_

### Return value

On success, 1 is returned.
On error, -1 is returned.

***

## Set joint control parameter

### Name

Spur_set_joint_vel, Spur_set_joint_accel

### Synopsis

```c
int Spur_set_joint_vel(int id, double v);
int Spur_set_joint_accel(int id, double a);
```

### Description

Set maximum angular velocity and acceleration of the joint.

* **id**: joint (motor) id
* **a**: angular acceleration of the joint _[rad/ss]_
* **v**: angular velocity of the joint _[rad/s]_

### Return value

On success, 1 is returned.
On error, -1 is returned.

***

## Joint control

### Name

Spur_joint_ang, Spur_joint_vel, Spur_joint_torque, Spur_joint_ang_vel

### Synopsis

```c
int YP_joint_ang(int id, double a);
int YP_joint_vel(int id, double v);
int YP_joint_torque(int id, double t);
int YP_joint_ang_vel(int id, double a, double v);
```

### Description

Control joint angle, velocity, and torque.
`YP_joint_ang_vel` controls both angle and velocity simultaneously to make velocity desired at the desired angle.

* **id**: joint (motor) id
* **a**: angular position of the joint _[rad]_
* **v**: angular velocity of the joint _[rad/s]_
* **t**: torque of the joint _[Nm]_

### Return value

On success, 1 is returned.
On error, -1 is returned.
