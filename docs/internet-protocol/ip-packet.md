# IPv4 Packet Structure

## Introduction

The **Internet Protocol version 4 (IPv4)** packet is the fundamental data unit at the **Network Layer** ([Layer 3](../fundamentals-of-networking/osi-model.md)) in the TCP/IP stack.
Before any piece of data can be sent over an IPv4-based network, it must be **encapsulated** into an IP packet.

An IPv4 packet is composed of two main parts:

1. **Header** – Contains essential metadata about the packet, including:

   * IP version
   * Header length
   * Source and destination IP addresses
   * Fragmentation control
   * Routing and delivery information

2. **Data (Payload)** – The actual content being transported. This could be:

   * An [HTTP](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/http.md) request or response
   * A DNS query
   * Part of a TCP or [UDP](/docs/udp/udp.md) segment

The **header** has a minimum size of **20 bytes**, but it can grow up to **60 bytes** if optional fields are included.
The **payload** can be anywhere from a few bytes up to a maximum of **65,535 bytes** (defined by the **Total Length** field). In reality, payload sizes are much smaller because of constraints like the **Maximum Transmission Unit (MTU)**, for example, Ethernet’s common MTU is **1500 bytes**.

---

### A Simplified View

While we often illustrate IP packets in a simple way:

![Simplified IP Packet](/assets/images/ip-packet.png)

… the actual IPv4 format is more complex and precise.

---

### The Real IPv4 Header Layout

Below is the **actual IPv4 packet structure** as defined in [RFC 791](https://www.rfc-editor.org/rfc/rfc791):

![IPv4 Header Structure](/assets/images/IPv4_Packet-en.svg.png)

The header’s fields are carefully arranged to balance efficiency, flexibility, and control over packet delivery.

---

## Why So Many Fields?

Each field in the IPv4 header serves a specific role:

* **Routers** use some fields (like Destination Address and TTL) to forward packets correctly.
* **Hosts** use fields to validate, reassemble, and pass data to the correct upper-layer protocol.
* **Network troubleshooting tools** (like Wireshark or `tcpdump`) rely on these fields to diagnose connectivity issues.

---

## IPv4 Header Fields Explained

### Version

Specifies the **IP version** being used.
For IPv4 packets, the value is always **4**. For IPv6 packets, it is **6**.
Routers use this to determine how to interpret the rest of the header.

---

### Internet Header Length (IHL)

Defines the length of the IPv4 header in 32-bit words.
Since the header can contain optional fields, its size is not fixed.

* **Minimum value**: 20 bytes (no options)
* **Maximum value**: 60 bytes (with options)

This allows routers and hosts to know **where the payload begins**.

---

### Differentiated Services (DSCP) and Explicit Congestion Notification (ECN)

Originally called the **Type of Service (ToS)** field, this 8-bit section is now split into:

1. **DSCP (Differentiated Services Code Point)**

   * Used for **Quality of Service (QoS)**.
   * Allows prioritization of certain traffic, e.g., voice calls or video streaming, so they receive lower latency compared to bulk file transfers.

2. **ECN (Explicit Congestion Notification)**

   * Signals **network congestion** without dropping packets.
   * Instead of discarding, routers can **mark packets** as congested.
   * The receiver then notifies the sender to slow down, preventing packet loss.

---

### Total Length

Specifies the **entire size of the IP packet**, including both header and payload.

* Maximum value: **65,535 bytes** (limited by the 16-bit field).
* In practice, packet sizes are constrained by the **MTU (Maximum Transmission Unit)** of the link (e.g., 1500 bytes for Ethernet).

---

### Identification, Flags, and Fragment Offset (Fragmentation)

IPv4 packets are designed to fit inside a single frame at the Data Link layer.
But if a packet is too large for the MTU, it must be **fragmented**.

* **Identification**: A 16-bit value assigned to a packet. All fragments of the same original packet share the same ID, so the receiver can reassemble them.
* **Flags**: Indicate whether fragmentation is allowed.

  * `DF` (Don’t Fragment): Prevents fragmentation. If the packet doesn’t fit, it will be dropped and the sender is notified.
  * `MF` (More Fragments): Signals that more fragments follow.
* **Fragment Offset**: Indicates the position of a fragment in the original data, allowing correct reassembly.

Fragments may arrive **out of order**, so the receiving host must carefully reassemble them.
Modern networks often avoid fragmentation by using **Path MTU Discovery (PMTUD)** and setting the **DF flag**.

---

### Time to Live (TTL)

The **TTL** prevents packets from circulating endlessly in routing loops.

* Each router that forwards the packet **decrements TTL by 1**.
* When TTL reaches **0**, the router discards the packet and typically sends back an ICMP “Time Exceeded” message.

Originally defined in **seconds**, TTL is now universally treated as a **hop count**.

---

### Protocol

Indicates which **upper-layer protocol** is encapsulated within the IP packet. Examples include:

* `6` → **TCP**
* `17` → **UDP**
* `1` → **ICMP**

This allows the Network layer to hand the payload to the correct Transport or Control protocol.

---

### Header Checksum

A simple error-checking mechanism for the **header only**.
Each router verifies the checksum before forwarding the packet.
If the checksum is invalid (e.g., header corruption), the packet is discarded.

---

### Source and Destination IP Addresses

These two fields define the **endpoints of communication**:

* **Source Address**: The IPv4 address of the sender.
* **Destination Address**: The IPv4 address of the intended receiver.

They are the **most critical fields** in the header, as routers use the **destination address** to forward the packet toward its final endpoint.

---

### Options and Padding (Optional)

The IPv4 header can include **optional fields** for features like:

* Security parameters
* Routing preferences
* Timestamping

Because these options are rarely used (and can even slow down routing), most IPv4 headers remain at the minimum 20-byte length.
**Padding** ensures the header always aligns to 32-bit boundaries.

---