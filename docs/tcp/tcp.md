# Transmission Control Protocol (TCP)

## Introduction

The **Transmission Control Protocol (TCP)** is one of the most widely used protocols in modern networking. It operates at **Layer 4 (Transport layer)** of the [OSI model](/docs/fundamentals-of-networking/osi-model.md) and provides **reliable, ordered, and error-checked communication** between applications running on hosts connected via a network.

Unlike its counterpart [UDP](/docs/udp/udp.md), which is connectionless and best-effort, TCP establishes a **stateful connection** between two endpoints, ensuring that data is transmitted accurately and in sequence.

---

## Key Characteristics

* **Port addressing** – TCP introduces the concept of **ports** to identify specific processes on a host. This enables multiple applications (e.g., a web server, a database, and an SSH service) to use the same IP address simultaneously without conflict.
* **Connection-oriented** – Communication requires establishing a connection between client and server. This is managed through a **three-way handshake**.
* **Stateful protocol** – Both sides maintain knowledge of the session, including sequence numbers, acknowledgments, and connection state.
* **Reliable delivery** – Segments are sequenced, acknowledged, and retransmitted if lost.
* **Flow and congestion control** – TCP dynamically adapts transmission rates to prevent overwhelming the receiver or the network.
* **Segment structure** – A TCP header is at least **20 bytes** and can grow up to **60 bytes** when options are used.

---

## TCP Connections

In practice, a **TCP connection** is the foundation of most backend [client–server communications](/docs/fundamentals-of-networking/client-server-architecture.md).

* A connection represents an **agreement** between a client and server to exchange data.

* It is uniquely identified by a **4-tuple**:

  * Source IP
  * Source Port
  * Destination IP
  * Destination Port

* Data **cannot be sent outside** an established connection.

* A connection is often referred to as a **socket** (programming abstraction) or a **file descriptor** (in operating system terms).

### The Three-Way Handshake

To establish a session, TCP uses a **three-step process**:

1. **SYN** – The client sends a synchronization request to initiate communication.
2. **SYN-ACK** – The server acknowledges and replies with its own synchronization.
3. **ACK** – The client acknowledges the server’s reply, completing the connection setup.

This ensures that both sides agree on initial sequence numbers and can reliably exchange data.

---

## Example: Establishing a TCP Connection

The figure below illustrates how a TCP connection is created between two applications (`App1` on host `10.0.0.1` and `AppX` on host `10.0.0.2`).

![TCP connection establishment](/assets/images/connection-establishment.png)

1. **SYN** – `App1` (port `5555`) initiates the connection by sending a **synchronization (SYN)** packet to `AppX` (port `53`) on the remote host. This packet proposes an initial sequence number.
2. **SYN-ACK** – `AppX` responds with a **synchronization + acknowledgment (SYN/ACK)** packet. This acknowledges `App1`’s request and includes its own proposed sequence number.
3. **ACK** – `App1` acknowledges the server’s sequence number by sending an **ACK** packet back.

At this point, the **three-way handshake** is complete, and the connection is established.

### File Descriptor Representation

Once established, the connection is uniquely identified by a **4-tuple**:

```
Source IP:Source Port  <->  Destination IP:Destination Port
```

In this example:

```
10.0.0.1:5555  <->  10.0.0.2:22
```

This tuple is internally represented by the operating system as a **file descriptor**. Applications use this descriptor to read and write data across the connection, just as they would with a regular file.

---

## Multiplexing and Demultiplexing

A single host may run many applications simultaneously. TCP enables this through **multiplexing and demultiplexing**:

* **Multiplexing** – Outgoing data from multiple applications is combined and transmitted over the same network interface using different port numbers.
* **Demultiplexing** – Incoming data is separated and delivered to the correct application based on the destination port.

This mechanism allows, for example, a server to handle HTTP traffic on port 80, HTTPS on port 443, and a database service on port 5432 simultaneously.

---

## Data Transmission and Reliability

Once a connection is established, applications can begin exchanging data. TCP ensures that this communication is **reliable and ordered**, unlike [UDP](/docs/udp/udp.md).

### Sending Data

In the example below, `App1` on host `10.0.0.1` sends a command (`ls`) to `AppX` on host `10.0.0.2`.

![TCP sending data](/assets/images/sending-data.png)

1. **Data encapsulation** – The application data is wrapped in a **TCP segment**, which includes sequence numbers.
2. **Transmission** – The segment is sent from the client’s socket (`10.0.0.1:5555`) to the server’s socket (`10.0.0.2:22`).
3. **Acknowledgment** – Upon receipt, `AppX` acknowledges the data by sending an **ACK** back to `App1`.

This acknowledgment confirms that the data was successfully received and that the receiver’s sequence number is now updated.

---

### Sequencing and Retransmissions

TCP uses **sequence numbers** to ensure that data arrives in the correct order, even if segments are delayed or lost along the way.

![TCP acknowledgment and sequencing](/assets/images/tcp-acknowledgment.png)

1. **Multiple segments** – `App1` sends three segments (`seq1`, `seq2`, `seq3`) in sequence.
2. **Cumulative acknowledgment** – `AppX` replies with an **ACK** confirming receipt up to `seq3`.
3. **Reliability mechanism** – If `App1` does not receive an acknowledgment for a segment within a timeout, it retransmits that segment.

This mechanism ensures that lost or corrupted segments are detected and retransmitted, so the application always receives a **complete, ordered byte stream**.

TCP also imposes a **sending limit (window size)**: the sender cannot transmit an unlimited number of unacknowledged segments. Instead, it must wait for acknowledgments before sending more, balancing reliability with throughput.

---

## Connection Termination

Just as TCP requires a handshake to establish a connection, it also requires a controlled process to **close** a connection. This ensures that both sides have finished transmitting data before resources are released.

### Four-Way Handshake

The process of closing a TCP connection involves **four steps**, often called the **four-way handshake**:

![TCP connection termination](/assets/images/tcp-closing.png)

1. **FIN (from client)** – `App1` (port `5555`) initiates the closure by sending a **FIN** packet to `AppX` (port `53`). This signals that it has no more data to send.
2. **ACK (from server)** – `AppX` acknowledges the request with an **ACK**. At this point, the connection is half-closed: `App1` cannot send more data, but it can still receive.
3. **FIN (from server)** – Once `AppX` has finished sending its data, it transmits its own **FIN** packet.
4. **ACK (from client)** – Finally, `App1` acknowledges the server’s FIN, completing the termination.

---

### TIME\_WAIT State

After the final ACK, the client’s file descriptor does not disappear immediately. Instead, it enters a **TIME\_WAIT** state.

This state ensures:

* Any delayed or duplicated segments still in the network are properly handled.
* Both ends can be sure that the connection has fully closed before releasing resources.

The TIME\_WAIT state typically lasts for **2 × Maximum Segment Lifetime (MSL)** , the maximum time a TCP segment could persist in the network.

---

## Common Use Cases

TCP is ideal when **reliability and order matter**:

* **Web communications** – Browsers and servers rely on TCP for [HTTP](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/http.md) and [HTTPS](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/https.md).
* **Database connections** – Consistent, reliable delivery of queries and responses.
* **File transfer** – Protocols like FTP, SFTP, and SMB depend on TCP.
* **Bidirectional applications** – Chat systems, email (SMTP, IMAP, POP3), and remote access (SSH, RDP).

---

## TCP vs UDP at a Glance

| Feature         | **TCP**                                                 | **UDP**                                  |
| --------------- | ------------------------------------------------------- | ---------------------------------------- |
| **Connection**  | Connection-oriented (requires handshake)                | Connectionless (no setup needed)         |
| **Reliability** | Reliable (retransmissions, acknowledgments, sequencing) | Unreliable (no retransmissions or ACKs)  |
| **Ordering**    | Guarantees ordered delivery                             | Packets may arrive out of order          |
| **Overhead**    | Higher (20–60 byte header)                              | Lower (8 byte header)                    |
| **Latency**     | Higher (due to reliability mechanisms)                  | Lower (no handshake, minimal processing) |
| **Use Cases**   | Web, databases, file transfer, remote login             | Video streaming, VoIP, DNS, gaming       |

---

## Summary

* TCP is the **reliable, connection-oriented Transport Layer protocol** that powers most internet applications.
* It provides **ordered delivery**, **error detection**, **flow control**, and **congestion control**.
* Its lifecycle consists of a **three-way handshake** for connection setup, **data transfer with sequencing and acknowledgments**, and a **four-way handshake** for termination with a **TIME\_WAIT** state.
* Compared to UDP, TCP trades **higher latency and overhead** for **guaranteed delivery and order**, making it essential for applications where correctness matters more than speed.

---
