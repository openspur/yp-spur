# libypspur - initialization

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

* **msq_id**: Message queue id
* **ip**: IP address
* **port**: TCP/IP port number

### Return value

On success, 1 is returned.
On error, -1 is returned.
