# IP Building Blocks

## Introduction

The **Internet Protocol (IP)** is a **Layer 3** [Network layer](../fundamentals-of-networking/osi-model.md) protocol in the [OSI model](../fundamentals-of-networking/osi-model.md).
Its core job is to **address** and **route** packets so they can travel across networks and reach the correct destination. Whether that’s your neighbor’s computer or a server on the other side of the world.

Every device that participates in IP networking has one or more **IP addresses** assigned to its network interfaces.
These addresses can be assigned **automatically** or **manually**.

---

## [IPv4](./ip-packet.md) Address Structure

An **IPv4** address:

* Is **4 bytes** (32 bits) long.
* Is written in **dot-decimal** format: `a.b.c.d`
* Each section (`a`, `b`, `c`, `d`) is an integer from **0 to 255**.
* Example: `192.168.1.42`

Every IPv4 address has two parts:

1. **Network portion** – Identifies the network segment.
2. **Host portion** – Identifies a device within that network.

The **subnet mask** tells us where the **network** part ends and the **host** part begins.

---

## Subnet Masks and CIDR Notation

A subnet mask can be expressed in **CIDR notation**:

```
192.168.254.0/24
```

* `/24` means: **first 24 bits** are the **network portion**.
* The remaining **8 bits** are for hosts.

**Example:**

| Address      | Binary Form                             | Portion        |
| ------------ | --------------------------------------- | -------------- |
| 192.168.1.42 | **11000000.10101000.00000001**.00101010 | Network + Host |
| Subnet `/24` | 11111111.11111111.11111111.00000000     | Mask           |

With `/24`:

* **Network**: `192.168.1.0` (reserved as the network address, not assignable to a host)
* **Broadcast**: `192.168.1.255` (used to send to all hosts in the subnet)
* **Usable host range**: `192.168.1.1` to `192.168.1.254`

---

## Same Subnet vs. Different Subnet

Devices in the **same subnet** can communicate directly without a router.
If devices are in **different subnets**, their traffic must pass through a **router**.

### Same Subnet Communication

![Same Subnet](/assets/images/same-subnet.png)

* **Network**: `192.168.1.0/24`
* Host A: `192.168.1.3`
* Host B: `192.168.1.2`

Both share the **same network portion** (`192.168.1`), so they send frames directly, using the Layer 2 ([Data Link](../fundamentals-of-networking/osi-model.md)) [MAC address](../fundamentals-of-networking/mac-ip-ports.md) of the destination host.

---

### Different Subnets (Router Required)

![Router In and Out](/assets/images/router-in-and-out.png)

* **Network 1**: `192.168.1.0/24`
* **Network 2**: `192.168.2.0/24`
* **Router Interface (Network 1)**: `192.168.1.100`
* **Router Interface (Network 2)**: `192.168.2.200`

**How Host A (`192.168.1.3`) talks to Host B (`192.168.2.3`):**

1. Host A checks its subnet mask (`/24`) and sees `192.168.2.3` is **outside** its network.
2. It sends the packet to its **default gateway**, the router at `192.168.1.100`.
3. The router examines the **destination IP** and forwards the packet through its `192.168.2.200` interface.
4. Host B sees the packet as coming from `192.168.2.200` and replies the same way — through the router.

Because the router has an IP in **each** subnet, it can forward traffic between them.

---

## Default Gateway

The **default gateway** is the router a host uses when the destination is not in its own subnet.

* It has an IP in each network it connects to.
* All devices in that subnet must know the gateway’s IP to communicate outside the subnet.

---

## Routing Beyond Local Networks

On the internet, packets often pass through **multiple routers**.
Each router:

1. Looks at the **destination IP**.
2. Checks its **routing table** to find the best **next hop**.
3. Forwards the packet toward that hop.

> **Note:** IP is a **connectionless protocol** — it doesn’t guarantee delivery, order, or error correction. That’s handled by higher-layer protocols like TCP.

---

## IPv6 – The Next Generation

While IPv4 uses 32-bit addresses, **IPv6** uses **128-bit** addresses, written in hexadecimal and separated by colons:

```
2001:0db8:85a3:0000:0000:8a2e:0370:7334
```

IPv6 was designed to provide a vastly larger address space and built-in features such as simplified header processing and mandatory support for IPsec.

---

## Summary

* **IP** is the Layer 3 protocol responsible for addressing and routing packets.
* An IPv4 address has a **network** and **host** portion, defined by the **subnet mask**.
* `/CIDR` notation specifies how many bits belong to the network.
* **Same subnet** → direct communication; **different subnets** → router required.
* The **default gateway** is the exit point from a subnet.
* Routers forward packets based on their **routing tables**; IP is **connectionless**.
* IPv6 extends the address space and simplifies certain networking operations.

---
