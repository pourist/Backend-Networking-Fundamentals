# Address Resolution Protocol (ARP)

## Introduction

In modern networks, devices use **[IP addresses](/docs/fundamentals-of-networking/mac-ip-ports.md)** for logical identification and **MAC addresses** for physical delivery. While the IP address enables routing across networks, the **[MAC address](/docs/fundamentals-of-networking/mac-ip-ports.md)** is required to actually send a frame over a local link (Layer 2 of the [OSI model](/docs/fundamentals-of-networking/osi-model.md)).

The **Address Resolution Protocol (ARP)** is the mechanism that bridges this gap: it maps known IP addresses to their corresponding MAC addresses.

---

## Why ARP is Needed

When a host wants to send data:

1. It knows the **destination IP address** (e.g., `192.168.1.5`).
2. It must place this IP packet inside a **frame** with a **destination MAC address**.
3. If the MAC is unknown, ARP is used to discover it.

This mapping is stored in an **ARP cache (table)** to avoid repeated lookups.

---

## ARP Workflow

### Step 1: Check Subnet

The host first determines if the destination IP is within the **same subnet**:

* If **yes**, it resolves the IP directly to a MAC.
* If **no**, it resolves the **default gateway’s IP** to a MAC, because the packet must be forwarded outside the subnet.

### Step 2: ARP Request

If the MAC address is not in the ARP cache:

* The host sends a **broadcast frame**:
  *“Who has 192.168.1.5? Tell 192.168.1.10.”*

### Step 3: ARP Reply

* The device with the requested IP replies with its MAC address.
* The sender updates its ARP cache and proceeds with transmission.

---

## Example: Host to Host Communication

```
Host A (192.168.1.10, MAC AA-AA-AA-AA-AA-AA)
wants to send to
Host B (192.168.1.20, MAC BB-BB-BB-BB-BB-BB)
```

1. Host A checks ARP cache for `192.168.1.20`.
2. If not found, Host A broadcasts an ARP request.
3. Host B responds: *“192.168.1.20 is at BB-BB-BB-BB-BB-BB.”*
4. Host A caches the mapping and sends the frame.

---

## Security Concerns

Because ARP lacks authentication, it is vulnerable to attacks:

* **ARP Spoofing / Poisoning**
  A malicious device replies with a fake MAC, tricking other hosts into sending traffic through it.
  This enables **man-in-the-middle attacks** or denial of service.

* **Cache Pollution**
  Attackers can flood ARP tables with false entries, disrupting communication.

---

## ARP and Gateways

When sending traffic to a **different subnet**:

* The host resolves the **gateway’s IP** into a MAC address.
* The frame is sent to the gateway, which then forwards it according to routing rules.
  This is often combined with **NAT (Network Address Translation)** at the gateway.

---

## Summary

* ARP maps **IP addresses** to **MAC addresses** at the boundary between the Network (Layer 3) and Data Link (Layer 2) layers of the [OSI model](./osi-model.md).
* It uses **broadcast requests** and **unicast replies**.
* Results are cached in an **ARP table** for efficiency.
* While essential, ARP is **inherently insecure**, making it a target for attacks like **ARP poisoning**.
* Understanding ARP helps backend engineers see how packets actually reach servers in a [client–server architecture](./client-server-architecture.md).

---
