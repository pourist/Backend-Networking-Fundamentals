# OSI Model

## Introduction

The **Open Systems Interconnection (OSI) model** is a conceptual framework that describes how data is transmitted over a network in **seven distinct layers**.
Even if you never work directly with networking protocols, knowing these layers is essential if your application communicates over a network.

Understanding the OSI model gives you clarity on:

* Where your application fits in the communication process.
* How your data is prepared, transmitted, and reconstructed.
* Which parts of the process are handled by your code versus the network infrastructure.

---

## Why a Communication Model?

Without a standard communication model:

* Every application would need to manage the specifics of the underlying network.
* Developers would have to handle differences between fiber optics, radio links, copper cables, and more.
* Network equipment and protocols would be difficult to innovate or make interoperable.

The OSI model solves this by defining **a universal structure** for data exchange, ensuring applications can work over any network medium without managing low-level transmission details.

---

## The Seven Layers of the OSI Model

| Layer | Name             | Purpose                                                                                | Example Protocols / Functions                                        |
| ----- | ---------------- | -------------------------------------------------------------------------------------- | -------------------------------------------------------------------- |
| 7     | **Application**  | Interfaces with the user-facing software, handling high-level protocols and user data. | [HTTP](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/http.md), FTP, gRPC |
| 6     | **Presentation** | Formats, encrypts, or compresses data for transmission.                                | JSON serialization, TLS encryption       |
| 5     | **Session**      | Establishes, manages, and terminates communication sessions.                           | TLS handshake, session tokens                                        |
| 4     | **Transport**    | Provides reliable delivery, segmentation, and error handling.                          | [TCP](../fundamentals/tcp.md), [UDP](../fundamentals/udp.md)         |
| 3     | **Network**      | Handles addressing and routing of packets between devices.                             | [IP](./mac-ip-ports.md), ICMP                                    |
| 2     | **Data Link**    | Packages packets into frames for transmission over a local link.                       | Ethernet, Wi-Fi                                                      |
| 1     | **Physical**     | Transmits raw bits over the physical medium.                                           | Fiber optics, radio waves, copper cables                             |

---

## How Data Moves Through the Layers

The OSI model follows an **encapsulation** process when sending data: each layer adds its own header (and sometimes a trailer) before passing the data to the next lower layer.
When receiving data, the process is reversed in **decapsulation** , each layer strips off its header and passes the remaining payload upward.

From the sender’s perspective, the process starts at **Layer 7 (Application)** and moves downward until **Layer 1 (Physical)** transmits the data. On the receiving side, the process moves upward from Layer 1 back to Layer 7.

![OSI model overview](/assets/images/osi-model.png)

---

## Example: Sending an [HTTPS](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/https.md) POST Request

Let’s follow the journey of sending [JSON](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/json-xml.md) data to an [API](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/api.md) over [HTTPS](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/https.md).

### 1. On the Client Side (Encapsulation)

1. **Layer 7 – Application**: The application creates an [HTTP](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/http.md) POST request with a JSON payload.
2. **Layer 6 – Presentation**: The JSON is serialized, and [TLS](../fundamentals/https.md) encrypts the request.
3. **Layer 5 – Session**: A secure session is established (TLS handshake).
4. **Layer 4 – Transport**: [TCP](../fundamentals/tcp.md) breaks the data into **segments** and ensures reliable delivery.
5. **Layer 3 – Network**: Each segment is wrapped in an **IP packet** with source and destination [IP](./mac-ip-ports.md) addresses.
6. **Layer 2 – Data Link**: The packet is placed into a **frame** with [MAC](./mac-ip-ports.md) addresses for local delivery.
7. **Layer 1 – Physical**: Frames are converted into electrical pulses, light signals, or radio waves for transmission.

---

### 2. Across the Network

![OSI model in routing context](/assets/images/osi-router.png)

* The first device is often a **switch** (Layer 2), which reads [MAC](./mac-ip-ports.md) addresses to forward frames within the local network.
* Next, a **router** (Layer 3) examines the IP header, determines the next hop, and re-encapsulates the packet in a new Layer 2 frame for the next network segment.
* This process repeats across multiple routers until the data reaches the destination network.

---

### 3. On the Server Side (Decapsulation)

1. **Layer 1 – Physical**: Physical signals are converted back into bits.
2. **Layer 2 – Data Link**: Frames are reconstructed and checked for errors.
3. **Layer 3 – Network**: The IP header is read to confirm the packet is addressed to this server.
4. **Layer 4 – Transport**: [TCP](../fundamentals/tcp.md) reorders segments, acknowledges receipt, and ensures no data is missing.
5. **Layer 5 – Session**: The secure session is maintained, and TLS decryption occurs.
6. **Layer 6 – Presentation**: The JSON data is deserialized into an application-ready format.
7. **Layer 7 – Application**: The server’s HTTP handler processes the request.

---

## OSI vs. TCP/IP in Practice

In the real world, most networks use the **TCP/IP model**, which has four layers:

| TCP/IP Layer   | Corresponding OSI Layers           |
| -------------- | ---------------------------------- |
| Application    | Application, Presentation, Session |
| Transport      | Transport                          |
| Internet       | Network                            |
| Network Access | Data Link, Physical                |

Many implementations merge or skip OSI’s Presentation and Session layers, handling those functions inside the Application layer or the protocol itself.

---

## Summary

* The OSI model provides a **universal language** for describing how data moves through a network.
* **Encapsulation** adds headers as data moves down the layers; **decapsulation** removes them on the way up.
* Understanding these layers helps backend engineers choose optimal protocols, troubleshoot latency, and design efficient [API](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/api.md)s.
* In practice, the OSI model maps to the **TCP/IP stack**, which is what most networks actually use.

---