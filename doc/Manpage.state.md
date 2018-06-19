# libypspur - vehicle status

## Get vehicle velocity

### Name

Spur_get_vel

### Synopsis

```c
double Spur_get_vel(double *v, double *w);
```

### Description

Get vehicle velocity and angular velocity.

* **\*v**: pointer to store velocity of the vehicle  _[m/s]_
* **\*w**: pointer to store angular velocity of the vehicle  _[rad/s]_

### Return value

On success, timestamp (unix time) of the data is returned.
On error, -1 is returned.

## Get vehicle force

### Name

Spur_get_force

### Synopsis

```c
int Spur_get_force(double *f, double *t);
```

### Description

Get estimated vehicle force and torque.

* **f**: translational force _[N]_
* **t**: torque _[Nm]_

### Return value

On success, 1 is returned.
On error, -1 is returned.
