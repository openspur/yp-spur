# libypspur

## Initialization

### Name

Spur_init, Spur_initex, Spur_init_socket - Initialization of YP-Spur library

### Synopsis

```
int Spur_init( void )
int Spur_initex( int msq_id )
int Spur_init_socket( const char *ip, int port )
```

### Description

* **msq_id** Message queue id
* **ip** IP address
* **port** TCP/IP port number

### Return value

On success, zero is returned. On error, -1 is returned.

***

## Basic parameter settings

### Name

Spur_set_vel, Spur_set_angvel, Spur_set_accel, Spur_set_angaccel - parameter settings

### Synopsis

```
int Spur_set_vel( double vel )
int Spur_set_angvel( double angvel )
int Spur_set_accel( double accel )
int Spur_set_angaccel( double angaccel )
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

Spur_line_GL, Spur_line_LC, Spur_line_FS - Line following

### Synopsis

```
int Spur_line_GL( double x, double y, double yaw )
int Spur_line_LC( double x, double y, double yaw )
int Spur_line_FS( double x, double y, double yaw )
```

### Description

* **x** x-coordinate of a point where the desired line passing through _[m]_
* **y** y-coordinate of the point _[m]_
* **yaw** orientation of the line _[rad]_

![Parameters for line following control](https://github.com/openspur/yp-spur/wiki/images/commands/line.png)

### Return value

On success, zero is returned. On error, -1 is returned.

***
