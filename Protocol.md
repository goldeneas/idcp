# lntroduction

This document specifies the characteristics of the Internet Chat Discovery Protocol (ICDP).  
The ICDP is a protocol used to allow **E2E-Encrypted**, **Anonymous**, **Decentralized**, **P2P** chat communication.

# Architecture

The ICDP Protocol specifies two types of nodes: the **Discovery** node and the **Client** node.  
A Client node can connect to a Discovery node through a TCP connection; this connection allows the Client node to be greeted by other Client nodes connected on the same Discovery node. Once a two-way greet is confirmed by the Discovery node between two Clients, they can start communicating.  

##### Example

Suppose we have a Discovery node A, a Client node B and a Client node C both connected to the Discovery node A.  
Suppose Client B greets Client C through the Discovery node A.  
If Client C wants to talk to Client B, it will also send its greet to Client C thorugh the Discovery node; otherwise the greet will be ignored.

## Discovery Node

The Discovery node has the following responsibilities:

- Provide fair Client greeting
- Let Client nodes establish a connection
- Establish E2E Encryption with Client nodes

## Client Node

The Client node has the following responsibilities:

- Open a TCP connection to a known Discovery node
- Listen for messages coming from the Discovery node
- Be ready to open a connection to other Client nodes if required
- Establish E2E Encryption with other Client nodes

# Packets

This section specifies the packets sent and received by each node type.  
Each packet can be sent:
- Discovery -> Client [D2C]
- Client -> Discovery [C2D]
- Client <=> Dicovery [C2D+D2C]  
- Client -> Client [C2C]

Each packet will be specified in JSON format.

## C2D

**Greet Request**

        {
            destination_name: string, [max_size = 256]
            source_port: number,
        }

**Client List Request**

        {

        }

## D2C

**MOTD**

        {
            server_name: string, [max_size = 256]
            motd: string, [max_size = 1024]
        }

**Greet Established**

        {
            client_address: string,
            client_port: number,
        }

**Client List**

        {
            clients: string[], [max_size = 16, max_count = 1024]
        }

## C2C

**Message**

        {
            text: string, [max_size = 256]
            size: uint,
        }
