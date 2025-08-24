# TCP Segment

## Introduction

The **TCP segment** is the fundamental unit of data transmission in the [Transmission Control Protocol](./tcp.md).
Every piece of application data is divided into segments, each of which carries **control information** (headers) along with the **payload**.
Segments are then encapsulated into [IP packets](/docs/internet-protocol/ip-packet.md) before being transmitted across the network.

* **Minimum size:** 20 bytes (header only, no options)
* **Maximum size:** 60 bytes (header with options)

---

## TCP Segment Structure

A TCP segment contains several fields that enable reliable, ordered, and flow-controlled communication.

![TCP Segment Structure](/assets/images/tcp-segment.png)

### Header Fields

| Field                     | Size     | Description                                                                     |
| ------------------------- | -------- | ------------------------------------------------------------------------------- |
| **Source Port**           | 16 bits  | Identifies the sending application process.                                     |
| **Destination Port**      | 16 bits  | Identifies the receiving application process.                                   |
| **Sequence Number**       | 32 bits  | Indicates the byte number of the first byte of data in this segment.            |
| **Acknowledgment Number** | 32 bits  | If the ACK flag is set, this field specifies the next expected sequence number. |
| **Header Length**         | 4 bits   | Size of the TCP header (in 32-bit words).                                       |
| **Reserved**              | 6 bits   | Reserved for future use, must be set to zero.                                   |
| **Flags**                 | 6 bits   | Control bits: `SYN`, `ACK`, `RST`, `FIN`, `PSH`, `URG`.                         |
| **Window Size**           | 16 bits  | Flow control: how much data the receiver can accept.                            |
| **Checksum**              | 16 bits  | Error-checking for both header and data.                                        |
| **Urgent Pointer**        | 16 bits  | If URG flag is set, indicates where urgent data ends.                           |
| **Options**               | Variable | Used for extra features (e.g., Maximum Segment Size, Timestamps).               |
| **Data**                  | Variable | The actual payload from the application.                                        |

---

## Flags

TCP uses **flags** (control bits) to manage connection state and data flow:

* **SYN** – Initiates a connection (synchronize sequence numbers).
* **ACK** – Acknowledges received data.
* **RST** – Resets a connection immediately.
* **FIN** – Gracefully terminates a connection.
* **PSH** – Pushes data to the receiving application immediately.
* **URG** – Marks data as urgent (uses the Urgent Pointer).

---

## Flow Control and Congestion Control

### Flow Control (Receiver-Side)

Flow control ensures that the **sender does not overwhelm the receiver**.
This is implemented through the **Window Size** field in the TCP header:

* The receiver advertises how many bytes it can handle (receive window).
* The sender must not exceed this limit before receiving an acknowledgment.
* Prevents buffer overflow and dropped data on the receiver’s side.

Example:
If the receiver’s window size = **4096 bytes**, the sender cannot send more than 4096 unacknowledged bytes at once.

---

### Congestion Control (Network-Side)

Congestion control prevents **overloading the network itself** rather than the receiver.
TCP dynamically adjusts its sending rate based on perceived congestion:

* **Slow Start** – Begin with a small congestion window (cwnd), then grow exponentially.
* **Congestion Avoidance** – Increase cwnd linearly as long as no packet loss occurs.
* **Fast Retransmit** – Retransmit a missing segment after receiving duplicate ACKs.
* **Fast Recovery** – Reduce cwnd but avoid going back to slow start entirely.

These mechanisms adapt TCP’s throughput to network conditions, balancing speed and stability.

---

## Maximum Segment Size (MSS)

The **Maximum Segment Size (MSS)** defines the largest chunk of data that can be carried in a single TCP segment.

* Depends on the underlying **MTU (Maximum Transmission Unit)** of the network.
* Common default MTU for Ethernet is **1500 bytes**.
* After subtracting IP (20 bytes) and TCP (20 bytes) headers, the default MSS is typically **1460 bytes**.
* Some networks and applications use smaller defaults (e.g., **512 bytes**) to improve reliability.

---
