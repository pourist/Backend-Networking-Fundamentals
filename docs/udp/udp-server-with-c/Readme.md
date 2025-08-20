# UDP Receiver Example

This is a minimal C program that demonstrates receiving [UDP](/docs/udp/udp.md) datagrams on a specified [IP](/docs/fundamentals-of-networking/mac-ip-ports.md) address and [port](/docs/fundamentals-of-networking/mac-ip-ports.md).

## Overview
- Creates an IPv4/UDP socket (`socket(AF_INET, SOCK_DGRAM, 0)`).
- Binds the socket to `127.0.0.1:5501`.
- Waits for a single UDP datagram (`recvfrom`).
- Prints the sender's IP/port and the received payload.
- Closes the socket and exits.

## Build
```bash
cc -Wall -Wextra -O2 udp-recieve.c -o udp_receive
````

## Run

```bash
./udp_receive
```

The process will block until a datagram arrives.

## Test

Open a second terminal and send a UDP packet to `127.0.0.1:5501`.

### Option 1: netcat (nc)

```bash
echo "test message" | nc -u -w1 127.0.0.1 5501
```

### Option 2: interactive netcat

```bash
nc -u 127.0.0.1 5501
# type messages and press Enter to send
```

### Option 3: Python one-liner

```bash
python3 - <<'PY'
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.sendto(b"hello from python", ("127.0.0.1", 5501))
PY
```

## Expected Output

On the receiver terminal:

```
Got N bytes from 127.0.0.1:PORT: "test message"
```

where `PORT` is the ephemeral source port chosen by the sender.

---