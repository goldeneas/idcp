# IDCP (Internet Chat Discovery Protocol)

This repository contains the source code for a chat application built in C, using a hybrid client-server and peer-to-peer (P2P) model. It uses `libuv` for asynchronous networking and `nanopb` for data serialization via Protocol Buffers.

The system consists of two main components:

1.  **Discovery Server:** A central server that manages a list of connected clients and introduces them to each other.

2.  **Client:** A command-line application that connects to the discovery server, requests to "greet" other clients, and then establishes direct UDP-based P2P communication with them.

---

## How It Works

1.  The `server` application is launched and listens for TCP connections on a specified port (default `24744`, defined in `server/src/settings.h`).

2.  A `client` application starts. The user types `/connect` to establish a TCP connection with the discovery server (`client/src/command_handlers.c`).

3.  Upon connection, the server sends the client a "Message of the Day" (MOTD) (`server/src/network.c`) and the client requests a list of other connected users (`client/src/network.c`).

4.  To initiate a chat, **both clients must mutually "greet" each other**.

    * Client 1 (e.g., with ID 1) types `/greet 2` to show interest in Client 2 (`client/src/command_handlers.c`).

    * Client 2 (with ID 2) must also type `/greet 1`.

5.  When the server receives this mutual greet request (`server/src/network_handlers.c`), it acts as an introducer. It sends a `greet_established` message to both Client 1 and Client 2. This message contains the IP address and the dedicated UDP "beacon port" (default `24745`) of the other party (`server/src/network.c`, `client/src/settings.h`).

6.  Once this introduction is complete, both clients add each other to their internal `peer_list` (`client/src/network_handlers.c`).

7.  From this point, any message typed into the client (without a `/` prefix) is broadcast directly to all peers in its `peer_list` via UDP (C2C), bypassing the server entirely (`client/src/command_handlers.c`, `client/src/network.c`).

8.  A keepalive mechanism (`client/src/keepalive.c`) periodically sends UDP packets to peers to maintain the connection.

## Technology Stack

* **Language:** C

* **Core Library:** `libuv` (for asynchronous I/O, event loop, TCP, UDP, TTY, and timers).

* **Serialization:** Google Protocol Buffers, using the `nanopb` C implementation.

* **Protocols:**

    * **C2D (Client-to-Discovery):** TCP-based. Used for connecting, sending greet requests, and asking for the client list.

    * **D2C (Discovery-to-Client):** TCP-based. Used for sending MOTD, client lists, and connection introductions.

    * **C2C (Client-to-Client):** UDP-based. Used for direct peer-to-peer chat messages and keepalives.

## Protocol Definitions

Protocol Buffers are used to define the message structures. The definitions are located in the `proto/` directory:

* `common.proto`: Defines the `common.client` message.

* `c2d_packets.proto`: (Client -> Server) Defines `greet_packet` and `client_list_request_packet`.

* `d2c_packets.proto`: (Server -> Client) Defines `motd_packet`, `client_list_packet`, and `greet_established_packet`.

* `c2c_packets.proto`: (Client <-> Client) Defines `message_packet` and `keepalive_packet`.

## How to Build

The repository uses a `Makefile` for building.

**Dependencies:**

* `gcc` (or compatible C compiler)

* `make`

* `protoc` (Protocol Buffer compiler)

* `nanopb` (protoc plugin and C library)

* `libuv` (development library, e.g., `libuv1-dev` on Debian/Ubuntu)

**Steps:**

1.  **Generate Protocol Files:**

    ```bash
    make proto
    ```

    This will use `protoc` and the `nanopb` plugin to convert the `.proto` files into C source (`.pb.c`) and header (`.pb.h`) files in the `common/src/generated/` directory.

2.  **Build Server and Client:**

    ```bash
    make all
    ```

    This will compile and link all source files, creating two executables in the `build/` directory:

    * `build/server`

    * `build/client`

3.  **Clean Build Artifacts:**

    ```bash
    make clean
    ```

## How to Run

1.  **Start the Server:**
    Open a terminal and run the server executable.

    ```bash
    ./build/server
    ```

    You should see a log message: `Listening on 0.0.0.0:24744`

2.  **Start Client 1:**
    Open a new terminal.

    ```bash
    ./build/client
    ```

3.  **Start Client 2:**
    Open a third terminal.

    ```bash
    ./build/client
    ```

4.  **Connect Clients:**

    * In **Client 1's terminal**, type:

        ```
        /connect
        ```

        You will see the server's MOTD and a list of connected clients (which will be empty or just you). The server will log your connection and assign you ID 1.

    * In **Client 2's terminal**, type:

        ```
        /connect
        ```

        You will see the server's MOTD. The server will assign you ID 2. Both clients will now see an updated client list showing IDs 1 and 2.

5.  **Establish P2P Chat:**

    * In **Client 1's terminal**, greet Client 2:

        ```
        /greet 2
        ```

    * In **Client 2's terminal**, greet Client 1:

        ```
        /greet 1
        ```

    * Once both greets are registered, the server will send the P2P connection details to both clients. You will see a log message in each client: `Client at [IP]:[Port] has accepted our greeting`.

6.  **Chat:**
    Now, any text you type in either client terminal (without the `/` prefix) and press Enter will be sent directly to the other client via UDP.
