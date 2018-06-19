# libypspur - coordinate system

## Get vehicle pose

### Name

Spur_get_pos_GL, Spur_get_pos_LC

### Synopsis

```c
double Spur_get_pos_GL(double *x, double *y, double *th);
double Spur_get_pos_LC(double *x, double *y, double *th);
```

### Description

Get vehicle position and orientation on desired coordinate system.

* **\*x**: pointer to store x position of the vehicle  _[m]_
* **\*y**: pointer to store y position of the vehicle _[m]_
* **\*th**: pointer to store yaw orientation of the vehicle _[rad]_

### Return value

On success, timestamp (unix time) of the data is returned.
On error, -1 is returned.

***

## Set coordinate system

### Name

Spur_set_pos_GL, Spur_set_pos_LC

### Synopsis

```c
int Spur_set_pos_GL(double x, double y, double th);
int Spur_set_pos_LC(double x, double y, double th);
```

### Description

Set relation between the desired coordinate system and its parent coordinate system to make vehicle position and orientation desired.

* **x**: x position of the vehicle  _[m]_
* **y**: y position of the vehicle _[m]_
* **th**: yaw orientation of the vehicle _[rad]_

### Return value

On success, 1 is returned.
On error, -1 is returned.

***

## Adjust coordinate system

### Name

Spur_adjust_pos_GL, Spur_adjust_pos_LC, Spur_adjust_pos_FS

### Synopsis

```c
int Spur_adjust_pos_GL(double x, double y, double th);
int Spur_adjust_pos_LC(double x, double y, double th);
int Spur_adjust_pos_FS(double x, double y, double th);
```

### Description

Set relation between the root coordinate system (BS) and global (GL) coordinate system to make vehicle position and orientation desired.
Running vehicle control command is updated according to the adjusted coordinate system.

* **x**: x position of the vehicle  _[m]_
* **y**: y position of the vehicle _[m]_
* **th**: yaw orientation of the vehicle _[rad]_

### Return value

On success, 1 is returned.
On error, -1 is returned.

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

* **x**: x position of a point where the desired line  _[m]_
* **y**: y position of the point _[m]_
* **th**: orthogonal yaw orientation of the line _[rad]_

![Parameters for checking whether getting over the line](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/over_line.png)

### Return value

On success, 1 is returned if the vehicle gets over the line, and 0 is returned elsewise.
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

* **x**: x position of the desired point _[m]_
* **y**: y position of the desired point _[m]_
* **d**: tolerance _[m]_

![Parameters for checking whether the vehicle gets near the desired position.](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/near_pos.png)

### Return value

On success, 1 is returned if the vehicle gets near the position and 0 is returned elsewise.
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

* **th**: yaw orientation of the desired orientation _[rad]_
* **d**: tolerance _[rad]_

![Parameters for checking whether the vehicle gets near the desired position.](https://github.com/openspur/yp-spur/raw/master/doc/images/commands/near_angle.png)

### Return value

On success, 1 is returned if the vehicle gets near the orientation, and 0 is returned elsewise.
On error, -1 is returned.
