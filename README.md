# DellFanControl

Controls Dell Server fan speeds over IPMI. Tested on a Dell R420.

**Setting Up:**

**Make sure you have `lm_sensors` and `ipmitool` installed.**

The IPMI interface has to be exposed to the host operating system, on iDRAC 7 Enterprise this can be enabled by ticking _Enable IPMI over LAN_ in _iDRAC Settings -> Network_

Now you can access IPMI directly from the host machine, using the dedicated `idrac` interface. (The IP is usually 169.254.0.1)

In order to add some obfuscation, credentials to access IPMI are stored in a `.env` file in the program directory. (More secure ways to input credentials are in the works). Format your `.env` like so:

```
HOST=<iDRAC IP>
USER=
PASS=
```

**Building:**

```cpp
make fancontol
./fancontrol <fanspeed>
```

**Usage:**

Because of a limitation in IPMI, fan speeds can only be set to two values. A manual static value from 0-100% can be set, or you can hand fan speed control over to the server (this is loud!).

`./fancontrol <fanspeed>`

Set your desired fan speed in % and fancontrol will maintain that speed unless the server gets too hot (set to 70C). If no argument is provided, fan speed is set to 15%.

By default, the program refreshes the fan speed every 3 seconds and outputs temperature information to the console. It can optionally check for network connectivity and restart NetworkManager if needed.

Extra: `start.sh` is an example of how to run this program in the background
