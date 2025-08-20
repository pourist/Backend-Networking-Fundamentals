# Introduction to UDP (User Datagram Protocol)

## Introduction

The **User Datagram Protocol (UDP)** is a core transport-layer protocol in the [OSI model](/docs/fundamentals-of-networking/osi-model.md) (Layer 4).
While [TCP](tcp.md) provides reliable, connection-oriented communication, UDP is **lightweight and connectionless**, designed for speed rather than guaranteed delivery.

In a [client–server architecture](/docs/fundamentals-of-networking/client-server-architecture.md), UDP is often chosen when applications prioritize **low latency over reliability**.
For example, a game client may continuously send player position updates to a server without waiting for acknowledgments. The occasional lost packet is acceptable compared to the performance cost of retransmissions. This stands in contrast with TCP, which is preferred when accuracy and ordered delivery are essential (e.g., web requests, file transfers).

UDP does not establish a session or track state between sender and receiver. Instead, it simply wraps application data in a datagram, attaches **source and destination ports**, and hands it to the IP layer for delivery.

This makes UDP highly efficient for applications where **low latency** is more important than reliability such as streaming, gaming, or real-time communication.

---

## Key Characteristics of UDP

* **Layer**: Transport (Layer 4).
* **Connectionless**: No handshake or state tracking.
* **Small header**: Only **8 bytes** (vs. TCP’s 20+ bytes).
* **Unreliable delivery**: No guarantees of delivery, order, or retransmission.
* **Multiplexing/demultiplexing**: Uses **ports** to map traffic to applications.
* **Lightweight and fast**: Minimal overhead, ideal for real-time use cases.

---

## UDP Header Structure

The UDP header is just **8 bytes** long and contains four fields:

| Field                | Size (bytes) | Description                                           |
| -------------------- | ------------ | ----------------------------------------------------- |
| **Source Port**      | 2            | Identifies the sending application.                   |
| **Destination Port** | 2            | Identifies the receiving application.                 |
| **Length**           | 2            | Total length of UDP header + data (minimum: 8 bytes). |
| **Checksum**         | 2            | Error detection for both header and data.             |

Unlike TCP, there are no fields for sequencing, acknowledgment, or flow control. This simplicity makes UDP much faster but less reliable.

---

## UDP Datagram Format

### Introduction

Every UDP message is encapsulated in a simple datagram header that is exactly **8 bytes long** (in IPv4). This header is placed inside the data field of an IP packet, making UDP a **Transport Layer protocol** in the [OSI model](/docs/fundamentals-of-networking/osi-model.md).

Unlike TCP, UDP does not provide sequencing or acknowledgment. Its header only carries the **minimum information** needed to deliver data from one application process to another.

---

### UDP Datagram Structure

A UDP datagram consists of two parts:

* **Header (8 bytes)** – Contains metadata about source/destination ports, length, and checksum.
* **Data (variable length)** – The actual payload from the application.

---

### Header Fields

The UDP header is divided into **four 16-bit (2-byte) fields**:

| Field                | Size (bits) | Description                                                                     |
| -------------------- | ----------- | ------------------------------------------------------------------------------- |
| **Source Port**      | 16          | Identifies the sending process (optional: may be 0 if not used).                |
| **Destination Port** | 16          | Identifies the receiving process on the target host.                            |
| **Length**           | 16          | Total size of UDP datagram (header + data), minimum value is 8.                 |
| **Checksum**         | 16          | Used for error detection of header and data. Can be set to 0 in IPv4 if unused. |

---

### Ports and Processes

Both **source and destination ports** are 16-bit values, ranging from **0 to 65,535**.

Each running application (process) on a host is assigned a unique port number for communication.
This enables:

* **Multiplexing**: The sender labels outgoing messages with the correct port.
* **Demultiplexing**: The receiver delivers the message to the correct process based on the destination port.

---

### Example

A source host (`10.0.0.1`) sends data from **port 5555** to a destination host (`10.0.0.2`) on **port 53**.

UDP header fields in this case:

* **Source Port** = `5555`
* **Destination Port** = `53`
* **Length** = size of header (8) + payload
* **Checksum** = error-detection value calculated over header and payload

---

## Use Cases of UDP

UDP is widely used in scenarios where **speed and efficiency** matter more than strict reliability:

* **DNS (Domain Name System)** → quick hostname resolution.
* **Streaming (audio/video)** → tolerates minor packet loss better than buffering.
* **VoIP (Voice over IP)** → real-time conversations.
* **Online gaming** → low-latency interactions.
* **VPNs** → e.g., WireGuard and OpenVPN for fast tunneling.
* **WebRTC** → peer-to-peer real-time communication in browsers.
---
## Pros and Cons of UDP

### Pros

* Simple, lightweight protocol
* Small header size → smaller datagrams
* Uses less bandwidth
* Stateless, minimal overhead
* Low memory consumption
* Low latency (no handshake, retransmission, or ordering required)

### Cons

* No acknowledgment of delivery
* No guarantee of delivery
* Connectionless → anyone can send data without prior setup
* No flow control
* No congestion control
* Packets may arrive out of order
* Limited built-in security

---


## Multiplexing and Demultiplexing

A single host may run **many applications at once**.
While the [IP protocol](/docs/internet-protocol/ip-packet.md) ensures packets reach the right host, it does not specify *which application* should handle the data.

UDP solves this with **ports**:

* **Multiplexing**: The sender labels each datagram with a **source port**, distinguishing which application sent it.
* **Demultiplexing**: The receiver reads the **destination port** to deliver the datagram to the correct application.

---

## Example: Source and Destination Ports

The diagram below illustrates UDP communication between two hosts (`10.0.0.1` and `10.0.0.2`), each running multiple applications:

![Source and Destination Port](/assets/images/source-and-destination-port.png)

* Host **10.0.0.1** runs applications on ports `5555`, `7712`, and `2222`.
* Host **10.0.0.2** runs applications on ports `35`, `68`, and `6978`.

When **App1** on `10.0.0.1` (port `5555`) sends data to an application on `10.0.0.2` (port `53`):

1. UDP header marks:

   * **Source port** = `5555`
   * **Destination port** = `53`
2. On arrival, host `10.0.0.2` examines port `53` and passes the datagram to the application listening there (DNS, in this case).
3. When host `10.0.0.2` replies, the roles of source and destination ports are swapped.

This mechanism allows **many applications to share the same network interface simultaneously**, without interfering with one another.

---

## Summary

* UDP is a **stateless, connectionless protocol** designed for fast, lightweight communication.
* With only an **8-byte header**, it sacrifices reliability for speed.
* **Ports** provide multiplexing and demultiplexing, mapping datagrams to the correct applications.
* Common use cases include DNS, streaming, gaming, VoIP, VPNs, and real-time communications.

---


