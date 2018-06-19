# libypspur - miscellaneous

## Gravity compensation

### Name

Spur_tilt_GL, Spur_tilt_LC, Spur_tilt_FS

### Synopsis

```c
int Spur_tilt_GL(double d, double t);
int Spur_tilt_LC(double d, double t);
int Spur_tilt_FS(double d, double t);
```

### Description

Set floor tilt angle and its direction used for gravity compensation.

* **d**: floor tilt orientation _[rad]_
* **t**: tilt angle _[rad]_

### Return value

On success, 1 is returned.
On error, -1 is returned.

***

## Emergency stop

### Name

Spur_freeze, Spur_unfreeze

### Synopsis

```c
int Spur_freeze(void);
int Spur_unfreeze(void);
```

### Description

Ignore any other control command and stop the vehicle.
Unfreeze to resume normal behavior.

### Return value

On success, 1 is returned.
On error, -1 is returned.

***

## Get emergency stop status

### Name

Spur_isfreeze

### Synopsis

```c
int Spur_isfreeze(void);
```

### Description

Get current emergency stop status.

### Return value

On success, 1 is returned if vehicle is in emergency stop mode, and 0 is returned elsewise.
On error, -1 is returned.

***

## Get analog/digital converter value

### Name

YP_get_ad_value

### Synopsis

```c
int YP_get_ad_value(int num);
```

### Description

Get the current value of analog/digital converter.
Note that `num=15` is reserved for digital IO value.

* **num**: port id of the a/d port

### Return value

On success, device dependent unsigned integer is returned.
On error, -1 is returned.

***

## Set digital IO setting

### Name

YP_set_io_dir, YP_set_io_data

### Synopsis

```c
int YP_set_io_dir(unsigned char dir);
int YP_set_io_data(unsigned char data);
```

### Description

Set digital IO port directions and the output state.
Results are device dependent.

* **dir**: 8-bit mask of output direction
* **data**: 8-bit output data

### Return value

On success, 1 is returned.
On error, -1 is returned.
