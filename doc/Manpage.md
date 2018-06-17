# libypspur

## Initialization

### Name

Spur_init, Spur_initex, Spur_init_socket

### Synopsis

```c
int Spur_init(void);
int Spur_initex(int msq_id);
int Spur_init_socket(const char *ip, int port);
```

### Description

Initialize the library and establish a connection to ypspur-coordinator.

* **msq_id** Message queue id
* **ip** IP address
* **port** TCP/IP port number

### Return value

On success, zero is returned. On error, -1 is returned.

***

## Basic parameter settings

### Name

Spur_set_vel, Spur_set_angvel, Spur_set_accel, Spur_set_angaccel

### Synopsis

```c
int Spur_set_vel(double vel);
int Spur_set_angvel(double angvel);
int Spur_set_accel(double accel);
int Spur_set_angaccel(double angaccel);
```

### Description

* **vel** Maximum linear velocity of the robot _[m/s]_
* **angvel** Maximum angular velocity _[rad/s]_
* **accel** Maximum linear acceleration _[m/ss]_
* **angaccel** Maximum angular acceleration _[rad/ss]_

### Return value

On success, zero is returned. On error, -1 is returned.

***

## Line following

### Name

Spur_line_GL, Spur_line_LC, Spur_line_FS

### Synopsis

```c
int Spur_line_GL(double x, double y, double th);
int Spur_line_LC(double x, double y, double th);
int Spur_line_FS(double x, double y, double th);
```

### Description

Control vehicle velocity and angular velocity to follow desired straight line.

* **x** x position of a point where the desired line passing through _[m]_
* **y** y position of the point _[m]_
* **th** yaw orientation of the line _[rad]_

![Parameters for line following control](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/line.png)

### Return value

On success, zero is returned. On error, -1 is returned.

***

## Circle following

### Name

Spur_circle_GL, Spur_circle_LC, Spur_circle_FS - Circle following

### Synopsis

```c
int Spur_circle_GL(double x, double y, double d);
int Spur_circle_LC(double x, double y, double d);
int Spur_circle_FS(double x, double y, double d);
```

### Description

Control vehicle velocity and angular velocity to follow desired circle.

* **x** x position of the center of the desired circle _[m]_
* **y** y position of the center of the desired circle _[m]_
* **d** radius of the circle _[m]_ (positive: CCW, negative: CW)

![Parameters for circle following control](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/circle.png)

### Return value

On success, zero is returned. On error, -1 is returned.

***

## Angle control (Yaw pose control)

### Name

Spur_spin_GL, Spur_spin_LC, Spur_spin_FS

### Synopsis

```c
int Spur_spin_GL(double th);
int Spur_spin_LC(double th);
int Spur_spin_FS(double th);
```

### Description

Control angular velocity to face desired direction.

* **th** Desired yaw orientation _[rad]_

![Parameters for angle control](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/spin.png)

### Return value

On success, zero is returned. On error, -1 is returned.

***

##  Angle control (Yaw pose control) with translational motion

### Name

Spur_orient_GL, Spur_orient_LC, Spur_orient_FS

### Synopsis

```c
int Spur_orient_GL(double th);
int Spur_orient_LC(double th);
int Spur_orient_FS(double th);
```

### Description

Control velocity and angular velocity to run facing desired direction.

* **th** Desired yaw orientation _[rad]_

![Parameters for angle control with translational motion](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/orient.png)

### Return value

On success, zero is returned. On error, -1 is returned.

***

##  Line following with position control

### Name

Spur_stop_line_GL, Spur_stop_line_LC, Spur_stop_line_FS

### Synopsis

```c
int Spur_stop_line_GL(double x, double y, double th);
int Spur_stop_line_LC(double x, double y, double th);
int Spur_stop_line_FS(double x, double y, double th);
```

### Description

Control vehicle velocity and angular velocity to follow desired straight line and stop at the desired position with best effort.

* **x** x position of a point where the desired line passing through and desired to stop on _[m]_
* **y** y position of the point _[m]_
* **th** yaw orientation of the line _[rad]_

![Parameters for line following with position control](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/stop_line.png)

### Return value

On success, zero is returned. On error, -1 is returned.

***

## Velocity control

### Name

Spur_vel

### Synopsis

```c
int Spur_vel(double v, double w);
```

### Description

Control vehicle velocity and angular velocity.

* **v** velocity of the vehicle _[m/s]_
* **w** angular velocity of the vehicle _[rad/s]_

### Return value

On success, 0 is returned.  On error, -1 is returned.

***

## Stop

### Name

Spur_stop

### Synopsis

```c
int Spur_stop(void);
```

### Description

Stop with set acceleration and angular acceleration.

### Return value

On success, 0 is returned.  On error, -1 is returned.

***

## Free

### Name

Spur_free, Spur_openfree

### Synopsis

```c
int Spur_free(void);
int Spur_openfree(void);
```

### Description

Stop velocity control and make wheels free.
`Spur_free` actively frees wheels compensating friction.
`Spur_openfree` opens motors drive circuits.

### Return value

On success, 0 is returned.  On error, -1 is returned.

***

## Get vehicle pose

### Name

Spur_get_pos_GL, Spur_get_pos_LC

### Synopsis

```c
int Spur_get_pos_GL(double *x, double *y, double *th);
int Spur_get_pos_LC(double *x, double *y, double *th);
```

### Description

Get vehicle position and orientation on desired coordinate system.

* **\*x** pointer to store x position of the vehicle  _[m]_
* **\*y** pointer to store y position of the vehicle _[m]_
* **\*th** pointer to store yaw orientation of the vehicle _[rad]_

### Return value

On success, 0 is returned. On error, -1 is returned.

***

## Get vehicle velocity

### Name

Spur_get_vel

### Synopsis

```c
int Spur_get_vel(double *v, double *w);
```

### Description

Get vehicle velocity and angular velocity.

* **\*v** pointer to store velocity of the vehicle  _[m/s]_
* **\*w** pointer to store angular velocity of the vehicle  _[rad/s]_

### Return value

On success, 0 is returned. On error, -1 is returned.

***

## Checking whether getting over the line

### Name

Spur_over_line_GL, Spur_over_line_LC, Spur_over_line_FS

### Synopsis

```c
int Spur_over_line_GL(double x, double y, double th);
int Spur_over_line_LC(double x, double y, double th);
int Spur_over_line_FS(double x, double y, double th);
```

### Description

Check whether the vehicle gets over the desired line.

* **x** x position of a point where the desired line  _[m]_
* **y** y position of the point _[m]_
* **th** orthogonal yaw orientation of the line _[rad]_

![Parameters for checking whether getting over the line](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/over_line.png)

### Return value

On success, 0 is returned if the vehicle doesn't get over the line and 1 is retured elsewise.
On error, -1 is returned.

***

## Checking whether getting near the position

### Name

Spur_near_pos_GL, Spur_near_pos_LC

### Synopsis

```c
int Spur_near_pos_GL(double x, double y, double d);
int Spur_near_pos_LC(double x, double y, double d);
```

### Description

Check whether the vehicle gets near the desired position.

* **x** x position of the desired point _[m]_
* **y** y position of the desired point _[m]_
* **d** tolerance _[m]_

![Parameters for checking whether the vehicle gets near the desired position.](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/near_pos.png)

### Return value

On success, 0 is returned if the vehicle doesn't get near the position and 1 is retured elsewise.
On error, -1 is returned.

***

## Checking whether getting near the orientation

### Name

Spur_near_angle_GL, Spur_near_angle_LC

### Synopsis

```c
int Spur_near_pos_GL(double th, double d);
int Spur_near_pos_LC(double th, double d);
```

### Description

Check whether the vehicle gets near the desired orientation.

* **th** yaw orientation of the desired orientation _[rad]_
* **d** tolerance _[rad]_

![Parameters for checking whether the vehicle gets near the desired position.](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/near_angle.png)

### Return value

On success, 0 is returned if the vehicle doesn't get near the orientation and 1 is retured elsewise.
On error, -1 is returned.
