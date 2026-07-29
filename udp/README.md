# IPC and Network Utilities for KDB

## Intro

This module extends **KDB** with basic UDP messaging capabilities.
It supports receiving and sending UDP messages, including broadcasting and multicast.

## Synopsis
```q
q) // Use the module functions like builtins from the .q namespace
q) .q,:use`dot.udp
q) // Listen on port 5000 and print UDP messages
q) h: ulisten[5000;{0N!(`udp;x;y)}]
q) // Send a UDP message to localhost (to self) on port 5000
q) usend[`localhost;5000;"hi"];
(`udp;`192.168.1.100;0x6869)
q) // Send a UDP message to another machine on the network
q) usend[`192.168.1.110;5000;"hi there"];
q) // Send message to everyone listening on port 5000 on the local network (broadcast)
q) usend[`255.255.255.255;5000;"hello world"];
(`udp;`192.168.1.100;0x68656c6c6f20776f726c64)
q) // Join a multicast group (the address in arbitary in `224.0.0.0` - `239.255.255.255`)
q) h ujoin `224.1.2.3
4i
q) // Send message to multicast group
q) usend[`224.1.2.3;5000;"hello multicast"];
(`udp;`192.168.1.100;0x68656c6c6f206d756c746963617374)
q) // List all network interfaces
q) ifls[]
interface      | address       netmask       broadcast     
---------------| ------------------------------------------
lo             | 127.0.0.1     255.0.0.0                   
enp4s0         | 192.168.1.100 255.255.255.0 192.168.1.255
q) // Send broadcast message to the second interfaces's broadcast address
q) usend[`192.168.1.255;5000;"hello world"];
(`udp;`192.168.1.100;0x68656c6c6f20776f726c64)
q) // Stop listening on port 5000
q) ulisten[-5000;::]
1b
```

## About UDP

UDP (User Datagram Protocol) is an asynchronous, connectionless protocol that provides unreliable message delivery. It is often used for applications that require low latency and do not need guaranteed delivery and message ordering. This means that UDP messages can be lost, duplicated, or arrive out of order.

UDP can also be used to send broadcast messages to all devices or a set of devices on a local network. There are two common ways to do this:

1. By using the network's broadcast address (e.g. ``255.255.255.255``): all devices on the same subnet will receive the message.
2. By using multicast addresses (e.g. ``224.0.0.1``): devices that have joined the multicast group will receive the message.

This module provides functions to:

- listen/unlisten on a specific port for UDP messages
- send UDP messages
- join/leave multicast groups
- list network interfaces

## Functions

### `ulisten`

Starts or stops listening for incoming UDP messages on a port.

- **Syntax**: `ulisten[port; callback]`
- **Arguments**:
  - `port`: long or int.
    - If `port > 0`: starts listening on the specified port.
    - If `port < 0`: stops listening on `neg[port]`.
  - `callback`: dyadic function: `callback:{[address;message]…}`.
    - `address`: symbol, the sender's IP address.
    - `message`: byte vector, the packet payload.
- **Returns**:
  - When starting: The underlying integer socket descriptor.
  - When stopping: A boolean indicating success (`1b`) or failure (`0b`).

### `usend`

Sends a UDP packet containing the message payload to a target host and port.

- **Syntax**: `usend[host; port; message]`
- **Arguments**:
  - `host`: symbol — target IP address (e.g., `localhost`, `127.0.0.1`, `0.0.0.0`, `192.168.1.100`).
  - `port`: A long or int.
  - `message`: char or byte vector of at most 65507 bytes.
- **Returns**: The destination host symbol on success, or KDB error on failure.

### `ujoin`

Joins or leaves a multicast group on a socket.

- **Syntax**: `ujoin[socket; group]`
- **Arguments**:
  - `socket`: long or int, the socket descriptor returned by `ulisten`.
    - if `socket > 0`: joins the specified multicast group.
    - if `socket < 0`: leaves the specified multicast group on `neg socket`.
  - `group`: symbol, the multicast group address in the range of `224.0.0.0` - `239.255.255.255` (e.g. ``224.0.0.1``).
- **Returns**: the input socket descriptor on success, or KDB error on failure.

### `ifls`

A helper function to list all network interdaces/adapters. It can help to figure out a broadcast address if `255.255.255.255` does not work.

- **Syntax**: `ifls[]`
- **Returns**: a keyed table with columns:
  - `interface`: symbol, the name or description of the interface (e.g. `lo0`, `en0`, or adapter name on Windows).
  - `address`: symbol, the IPv4 address.
  - `netmask`: symbol, the subnet mask.
  - `broadcast`: symbol, the broadcast address (if available).

## Examples

### IPC and Pub/Sub over UDP

We can use [`-18!`](https://code.kx.com/q/basics/internal/#-18x-compress-bytes) and [`-9!`](https://code.kx.com/q/basics/internal/#-9x-from-bytes) to serialize and deserialize K objects and implement our own connectionless IPC.

```q
q) h:ulisten[5000;{get -9!y}]
q) send:{[a;m] usend[a;5000;-18!m]}
```
Multicast groups can be used to implement pub/sub without a Tickerplant (or other centralized server).
```q
q) gr.trade: `224.0.0.1       //trade group
q) gr.quote: `224.0.0.2       //quote group
q) h ujoin/ gr`trade`quote;   //"subscribe" to both
q) upd: upsert
q) send[gr.trade;(`upd;`trade;([]sym:`a`b;time:.z.p;price:2?100f;size:2?1000))]
```

### MineCraft monitoring

When the MineCraft Bedrock edition starts up it imediately starts sending UDP broadcast messages on port 19132 to advertise itself on the local network.
We can use `ulisten` to monitor that and join the party.

```q
q) ulisten[19132;{0N!(`mc;x;y)}]
(`mc;`192.168.1.110;0x0100000000386a435900ffff00fefefefefdfdfdfd12345678afded273ce4ea162)
(`mc;`192.168.1.198;0x01000000000001718b00ffff00fefefefefdfdfdfd12345678823e6e6f1cb650a6)
(`mc;`192.168.1.110;0x0100000000386a475100ffff00fefefefefdfdfdfd12345678afded273ce4ea162)
(`mc;`192.168.1.198;0x01000000000001757500ffff00fefefefefdfdfdfd12345678823e6e6f1cb650a6)
```
Similarly, when a Java edition user hosts a "Local to LAN" world, their client continuously broadcasts server information so other players on the same network can see it.
These broadcasts messages are sent to group ``224.0.2.60`` on port ``4445``.
```q
q) ulisten[4445;{0N!(`mc.java;x;"c"$y)}] ujoin `224.0.2.60;
(`mc.java;`192.168.1.110;"[MOTD]MCUser - New World[/MOTD][AD]53758[/AD]")
(`mc.java;`192.168.1.110;"[MOTD]MCUser - New World[/MOTD][AD]53758[/AD]")
(`mc.java;`192.168.1.110;"[MOTD]MCUser - New World[/MOTD][AD]53758[/AD]")
```