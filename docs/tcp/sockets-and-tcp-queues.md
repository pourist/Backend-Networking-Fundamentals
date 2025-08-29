# Sockets and TCP Queues

## Introduction

**Sockets** are the operating system’s API for network I/O, providing a standard interface for applications to send and receive data.
They form the bridge between higher-level communication models such as the [OSI model](/docs/fundamentals-of-networking/osi-model.md) and [client–server architecture](/docs/fundamentals-of-networking/client-server-architecture.md), and the low-level mechanics of the kernel and network stack.

---

## What is a Socket?

A **socket** is an operating system abstraction for a network endpoint.

* Created with `socket()`.
* Referenced by a **file descriptor** (like files and pipes).
* Behavior determined by **address family** and **type** (e.g., `AF_INET + SOCK_STREAM` for IPv4 TCP).

Think of a socket as a **virtual cable end**: your program reads and writes bytes, while the kernel handles delivery across the network.

---

## Control Path vs. Data Path

* **Control path**: Functions like `bind()`, `listen()`, `connect()`, and `setsockopt()` configure socket state.
* **Data path**: Functions like `send()`/`write()` and `recv()`/`read()` move bytes through kernel-managed buffers.

---

## Addresses, Ports, and Binding

Each TCP socket is uniquely identified by a **four-tuple**:

```
(local IP, local port, remote IP, remote port)
```

Binding associates a socket with a local IP and port:

```c
int fd = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 0.0.0.0 → all interfaces

bind(fd, (struct sockaddr*)&addr, sizeof(addr));
```

* `INADDR_ANY` (IPv4) or `in6addr_any` (IPv6) listens on **all local interfaces**.
* Binding to a specific address restricts connections to that interface.
* Modern hosts often have multiple NICs, multiple IPs per NIC, or container-assigned addresses.

**Analogy**: If a machine is a building with many entrances (IPs), binding chooses which **door** and **doorbell** (port) you wait at.

Cross-ref: see [Client–Server Architecture](/docs/fundamentals-of-networking/client-server-architecture.md) for how servers expose services on well-known ports.

---

## Listening vs. Connected Sockets

TCP sockets serve two distinct roles:

* **Listening socket (server)**

  * Created with `socket()`, bound with `bind()`, and put in passive mode with `listen()`.
  * Does not carry application data.
  * Accepts incoming connections and produces connected sockets.

* **Connected socket (client or server)**

  * Client obtains one via `connect()`.
  * Server obtains one via `accept()` on a listening socket.
  * Used for bidirectional data transfer with `send()` and `recv()`.

**Minimal TCP Server (IPv4):**

```c
int srv = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = htonl(INADDR_ANY);

bind(srv, (struct sockaddr*)&addr, sizeof(addr));
listen(srv, 128);  // backlog hint

for (;;) {
  int cli = accept(srv, NULL, NULL);
  // handle cli with read()/write(), then close(cli)
}
```

---

## The TCP Handshake

TCP uses a **three-way handshake** to establish a connection:

1. Client → SYN
2. Server → SYN-ACK
3. Client → ACK

The kernel on each host implements this automatically.
Your server code only calls `listen()` and `accept()`, while the kernel manages the handshake per [RFC 793](https://www.rfc-editor.org/rfc/rfc793) and later updates.

---

## Understanding the Queues

When a server calls `listen(fd, backlog)`, the kernel manages two conceptual queues:

1. **SYN backlog (pending / half-open queue)**

   * Holds connections mid-handshake (server sent SYN-ACK, awaiting final ACK).
   * If full, new SYNs may be dropped or handled with SYN cookies.

2. **Accept queue (completed / established queue)**

   * Holds fully established connections waiting for `accept()`.
   * If full (e.g., app is slow), new connections may be refused or dropped.

**Lifecycle:**

* SYN arrives → kernel creates a pending entry (SYN backlog).
* Handshake completes → entry moves to accept queue.
* Application calls `accept()` → kernel returns a connected socket.

Think: the listening socket is a **factory**. Each call to `accept()` gives you a new “product” (connected socket), while the factory continues to operate.

---

## What Does `backlog` Control?

The `backlog` parameter in `listen()` hints at the size of the accept queue (sometimes influencing pending queue size too).

```c
int listen(int sockfd, int backlog);
```

* Too small → bursts of connections may be refused.
* Too large → consumes memory, but clamped by kernel limits (`somaxconn` on Linux).

> Example: `listen(fd, 1_000_000)` does **not** actually create one million slots — the OS enforces caps.

---

## Per-Socket Buffers: Send and Receive

Every connected TCP socket maintains **kernel buffers**:

* **Send buffer**: holds outgoing data until transmitted and acknowledged.
* **Receive buffer**: stores incoming data until the application reads it.

These buffers:

* Smooth rate differences between app and network.
* Are flow-controlled by TCP (window size, congestion control).
* Can be tuned via `SO_SNDBUF` and `SO_RCVBUF`.

---

## Common Options and Limits

* **SO\_REUSEADDR** → rebind quickly after restart (even if TIME\_WAIT).
* **SO\_REUSEPORT** (if supported) → multiple processes can bind the same (IP, port).
* **Binding**:

  * `0.0.0.0` (or `::`) → all addresses.
  * Specific IP → only that NIC/address.
* **Limits**:

  * `backlog` clamped by system.
  * File descriptor limits (`ulimit -n`) cap concurrent sockets.

High-performance servers often use readiness APIs:

* `epoll` (Linux), `kqueue` (BSD/macOS), or `IOCP` (Windows).

---

## Client Side: Making a Connection

```c
int fd = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in dst = {0};
dst.sin_family = AF_INET;
dst.sin_port = htons(8080);
inet_pton(AF_INET, "203.0.113.10", &dst.sin_addr);

connect(fd, (struct sockaddr*)&dst, sizeof(dst));
// then: send()/recv() on fd
```

* Kernel picks an **ephemeral port** and interface if you don’t pre-bind.
* Clients never call `listen()` or `accept()`.

---

## UDP Contrast

UDP (`SOCK_DGRAM`) differs fundamentally:

* No handshakes or accept queues.
* Communication via `sendto()` / `recvfrom()`.
* Application must handle loss, ordering, and retries.
* Useful for low-latency, lossy-tolerant use cases (e.g., DNS, telemetry).

See [UDP](/docs/udp/udp.md) for details.

---

## Practical Checklist

**Server:**

1. `socket()`
2. `setsockopt(SO_REUSEADDR|SO_REUSEPORT)` (optional, recommended)
3. `bind(local_ip, port)`
4. `listen(backlog)`
5. Loop: `accept()` → handle socket with `recv()`/`send()`

**Client:**

1. `socket()`
2. (optional) `bind(local_ip)` if specific source required
3. `connect(remote_ip, port)`
4. Exchange data with `send()`/`recv()`

---

## Summary

* A socket is a file-descriptor–based endpoint for network I/O.
* **Servers** use a listening socket to accept connections, producing connected sockets for data transfer.
* TCP handshakes are handled entirely by the kernel.
* Two queues exist:

  * **SYN backlog** (pending handshakes).
  * **Accept queue** (ready connections).
* `backlog` in `listen()` hints at capacity but is capped by system limits.
* Each connected socket has **send/receive buffers** managed by the kernel.

This knowledge bridges **client–server architecture** with the OS and kernel-level mechanics of TCP, helping engineers size queues, bind effectively, and build robust network code.

---
