# Chatroom 

This project is a basic chatroom implementation using Winsock in C++. It consists of two components:

**Server:**
Manages multiple client connections and broadcasts messages between them.

**Client:**
Enables individual users to connect to the server and communicate with other connected users.

# Features

Multi-threaded server to handle multiple clients concurrently.

Broadcasting messages to all connected clients.

Simple logging of messages in a text file (server_chat_history.txt).

Clients can send and receive messages in real-time.

# How to Run

**Step 1:**

• Copy and paste the server code into a file named server.cpp .
  
• Open Microsoft Visual Studio (or another IDE).

• Create a new project for the server code.

• Add the server.cpp file to the project.

• Build the project to compile the server code.

• Start the server executable. The server will start listening on port 12345 and wait for client connections.


**Step 2:**

• Do same and add client.cpp (multiple clients if you want) to the client project.

•When prompted, enter your chat name. Begin typing messages to participate in the chatroom.


# Server Code Breakdown

Initializes Winsock (WSAStartup) and creates a listening socket.

Binds the socket to port 12345 and IP address 0.0.0.0 (accepting connections from all interfaces).

Uses listen to accept incoming client connections.

Each client connection is handled in a separate thread using std::thread.

Receives messages from a client and broadcasts them to all other connected clients.

Logs the messages in server_chat_history.txt.

# Client Code Breakdown

Initializes Winsock (WSAStartup) and connects to the server at 127.0.0.1 (localhost) on port 12345.

***Sending Messages:***

Prompts the user to enter their chat name and allows them to type messages.

Sends the message to the server using the send function.

Exits if the user types quit.

***Receiving Messages:***

Continuously listens for messages from the server using the recv function.

Displays received messages to the user.

Multi-threading:
Uses two threads to handle sending and receiving messages independently.

# Enhancements

Add authentication or user validation.

Support for richer message formats (e.g., emojis, file transfers).

Implement a GUI for a more user-friendly experience.

Include error handling for edge cases (e.g., server crash, network issues).
