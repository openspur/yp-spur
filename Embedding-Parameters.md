Motor controllers like TF-2MD3-R4/R6 by T-frog Project can have embedded parameter file on the controller board.

# Setting parameter name

Open device port to send following commands.
(On Unix/unix-like, `cat /dev/ttyACM? & cat - > /dev/ttyACM?`. On Windows, use TeraTerm)
```
$SETNAME[name_of_the_parameter_file]
$EEPROMSAVE
```

If `-p` option is not specified, ypspur-coordinator uses `[name_of_the_parameter_file].param`.
If `[name_of_the_parameter_file]` is `embedded`, ypspur-coordinator uses controller embedded parameter file.


# Embedding parameter file

Change parameter name to `embedded`.
```
$SETNAMEembedded
$EEPROMSAVE
```

Embed desired parameter file.
```
SETEMBEDDEDPARAM
parameter: value
parameter: value
parameter: value
parameter: value
parameter: value

```

Consecutive two line-feeds indicates the end of the parameter file.
In the case of TF-2MD3-R4/R6, a maximum number of characters is 512 bytes.

# Reading embedded parameter file

```
GETEMBEDDEDPARAM
```
