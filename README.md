# CPP_Messenger

This project implements a simple messaging application using C++ with encryption and decryption capabilities inspired by RSA encryption principles.

## Features

- **Client-Server Architecture**: Communicate between clients via a central server.
- **Message Encryption and Decryption**: Encrypt messages before sending and decrypt upon receiving using RSA-like cryptography.
- **File Watching**: Automatically detect new messages written to files.

## Prerequisites

- **G++ Compiler**: Ensure you have `g++` installed on your system.
- **Netcat**: This tool is used for network communication.

## Getting Started

### Setting up the Server

1. **Compile and Run the Server**:

    ```sh
    g++ mainServer.cpp -o mainServer
    ./mainServer
    ```

### Setting up the Client

1. **Compile the Client**:

    ```sh
    g++ client.cpp -o client
    ```

2. **Run the Client**:

    ```sh
    ./client <client_id> <file_path>
    ```

    Example:

    ```sh
    ./client client1 messages.txt
    ```

## Usage Example

1. **Sending Messages**:
    - Run the client executable.
    - Follow the prompts to send a message.

2. **Receiving Messages**:
    - Run the client executable.
    - Automatically decrypt and display incoming messages.

## Contributing

1. Fork the repository.
2. Create your feature branch: `git checkout -b my-new-feature`.
3. Commit your changes: `git commit -am 'Add some feature'`.
4. Push to the branch: `git push origin my-new-feature`.
5. Submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
