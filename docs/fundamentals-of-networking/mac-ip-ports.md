# MAC, IP, and Ports

## Introduction

Every device connected to a network needs a way to be identified and reached. This happens in **three steps**:

1. **MAC address** – Uniquely identifies the device’s network interface **within a local network**.
2. **IP address** – Allows packets to be **routed across different networks**.
3. **Port** – Ensures the data is delivered to the **correct application** on the device.

Together, they make sure data can start at one program on one machine and reach the intended program on another, even across the world. These concepts map to different layers in the [OSI model](./osi-model.md) and form the foundation of backend and network communication.

---

## MAC: Finding the Right Device on a Local Network

On a local network (LAN), devices use **MAC addresses** — permanent identifiers built into each network interface card (NIC).

When Device A sends a frame to Device B:

![OSI model in routing context](/assets/images/host-to-host.png)


* **Every device** on that network (B, C, D, etc.) physically receives the frame.
* Only B’s NIC accepts it (MAC match); the others discard it.
* But if the network isn’t encrypted (e.g., open Wi‑Fi), devices C and D could **capture** and read the traffic.


**Diagram – Local Frame Broadcast:**

```
   A ─────► [Frame to B] ─────► C (drops)
            (all receive)       D (drops)
                                B (processes)
```

### Security Implication
Because Layer 2 delivery is essentially a *broadcast* in many LAN setups, unencrypted traffic can be intercepted by any device within range. Using secure protocols like [HTTPS](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/https.md) or enabling network encryption prevents attackers from reading sensitive data.

---

## IP: Finding the Right Network (and Device) Across the Internet

The **IP address** is like a postal address:

* The **network part** tells routers which network to send the packet toward.
* The **host part** identifies the specific device within that network.

When data leaves a local network, it travels hop by hop:

1. The sender wraps the data in an IP packet with the **destination IP**.
2. Routers along the way read the IP header and decide where to forward the packet next.
3. At each hop, only the **local delivery info** (Layer 2) changes; the IP stays the same until it reaches the destination network.

**Example:**

In the diagram below, Host `192.168.1.3` (in Network 1) wants to send data to Host `192.168.2.2` (in Network 2). These two networks are connected via a router.

![Host-to-host communication across networks](/assets/images/host-ip.png)

**Process:**

1. Host `192.168.1.3` sees that `192.168.2.2` is not in its local network (`192.168.1.0/24`).
2. It sends the packet to its **default gateway** (the router) using the router’s MAC address for the first hop.
3. The router checks the **destination IP** and determines it belongs to `192.168.2.0/24`.
4. The router forwards the packet into Network 2, where it is delivered to `192.168.2.2`.

At each hop, only the **Layer 2 frame** changes, but the **IP address stays the same** until it reaches the destination network.

For more on how this works, see the [Network layer](./osi-model.md#network).

---

## Port: Finding the Right Application

When the IP packet reaches the right device, the work isn’t done. That device may be running many programs at once:

* A web server on port **80** (HTTP) or **443** (HTTPS)
* An email server on port **25**
* An SSH service on port **22**

**Ports** act like room numbers inside a building, the operating system uses them to pass incoming data to the correct program.

---

## A Journey from One App to Another

Imagine you open a web browser on your laptop and request a page from a server:

1. **Your browser** creates the request and says, “Send this to IP `203.0.113.10`, port `443`.”
2. Your computer sees the destination IP isn’t local, so it sends the packet to its **gateway router**, wrapping it with the router’s MAC address for the first hop.
3. **Routers** along the path strip off the old MAC address, add a new one for the next hop, and forward the packet toward the destination but the **IP address and port stay the same**.
4. When the packet reaches the **destination network**, the final router delivers it to the server’s MAC address.
5. The **server’s operating system** reads the port number and hands the data to the program listening on port `443` — its HTTPS server.
6. The response makes the same trip in reverse.

---

## Summary

* **MAC addresses** deliver data locally within one network — but all devices on that network can see it, which is why encryption is important.
* **IP addresses** allow data to travel between different networks.
* **Ports** make sure the data ends up at the right application on the device.
* Data moves in layers: local delivery (MAC), global delivery (IP), and application selection (Port).
