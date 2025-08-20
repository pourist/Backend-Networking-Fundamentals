# Client–Server Architecture

## Introduction

The **client–server architecture** is a fundamental design pattern in networked computing.
In this model, client machines request services or resources from a central server.
The server handles the heavy lifting, processing requests, executing complex computations, and managing data while clients focus on user interaction and lightweight processing.

This approach allows organizations to use affordable client hardware while centralizing intensive workloads on more capable servers.

---

## Definition

![Alt text for accessibility](/assets/images/client-server.png)


Client–server architecture is a distributed application structure in which:

* **Clients** initiate communication and send requests.
* **Servers** process these requests and return responses.

A client can be any device or application such as a web browser, mobile app, or desktop application — that interacts with a server over a network.

Example workflow:

1. The client sends a request (e.g., an [HTTP](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/http.md) POST for an [API](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/api.md) endpoint).
2. The server processes the request using its computational resources.
3. The server sends back the result to the client.

---

## Key Characteristics

* **Task separation** – The client focuses on presentation and minimal processing, while the server handles computation and data management.
* **Centralization** – Most business logic, data storage, and heavy computation reside on the server.
* **Network dependency** – The communication between client and server depends on a reliable network and well-defined communication protocols.

---

## Benefits

| Benefit                          | Description                                                                                            |
| -------------------------------- | ------------------------------------------------------------------------------------------------------ |
| **Reduced client hardware cost** | Clients can run on inexpensive, commodity machines since heavy computation is offloaded to the server. |
| **Centralized maintenance**      | Updates and fixes occur on the server, reducing the need to manage multiple client installations.      |
| **Lightweight clients**          | Clients only perform essential local tasks, improving responsiveness and reducing dependencies.        |
| **Scalability**                  | Servers can be upgraded independently to support more clients or heavier workloads.                    |

---

## Communication Model

A functioning client–server system requires a **communication model** to define:

* **Protocol**: The set of rules for request/response (e.g., [HTTP](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/http.md), WebSocket, gRPC).
* **Data format**: The structure of exchanged data (e.g., JSON, XML, binary).
* **Connection type**: Whether communication is connection-oriented (TCP) or connectionless ([UDP](/docs/udp/udp.md)).

For example:

* A [REST](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/rest-soap.md) [API](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/api.md) over [HTTP](https://github.com/pourist/API-and-Web-Services-Fundamentals/blob/main/docs/fundamentals/http.md) uses a request/response model with stateless communication.
* A real-time chat app may use WebSockets to maintain an open, bidirectional channel.

---

## Example Scenario

Imagine a **photo editing web app**:

* **Client**: A web browser providing a UI for users to upload images and apply filters.
* **Server**: Performs CPU-intensive image processing using optimized libraries.
* **Benefit**: The client device doesn’t need specialized software or high-end hardware — the server does the processing and returns the results.

---

## Summary

The client–server architecture separates responsibilities between clients (user-facing) and servers (processing and data).
By centralizing heavy workloads on servers, organizations can reduce costs, simplify maintenance, and improve scalability. However, the architecture depends on a reliable communication model to ensure efficient and secure data exchange.

---
