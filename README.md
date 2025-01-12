
# Bank Account Management System

This is a simple bank account management system implemented using C, TCP sockets, and multithreading. The system allows multiple clients to connect to the server and perform operations such as adding funds to an account, withdrawing funds, checking the balance, and viewing the last 10 operations for an account.

## Features
- **Add Funds**: `AJOUT` command.
- **Withdraw Funds**: `RETRAIT` command.
- **Check Balance**: `SOLDE` command.
- **View Operations**: `OPERATIONS` command.
- **Handle Multiple Clients**: Server can handle multiple clients simultaneously using threads.

## Table of Contents
1. [Prerequisites](#prerequisites)
2. [Cloning the Repository](#cloning-the-repository)
3. [Compiling the Code](#compiling-the-code)
4. [Running the Server and Client](#running-the-server-and-client)
5. [Using the Commands](#using-the-commands)
6. [Adding New Accounts](#adding-new-accounts)
7. [Error Handling](#error-handling)
8. [Closing the Application](#closing-the-application)

## Prerequisites
Before you begin, ensure that you have the following installed:
- **C Compiler**: A C compiler like `gcc`.
- **Make**: A build automation tool for managing compilation.
- **Threads**: Ensure your system supports threading (usually supported by default on most systems).
- **Network Access**: This setup uses TCP sockets, so ensure that port 8080 is available (or modify it if necessary).

## Cloning the Repository
To clone the repository, use the following command in your terminal:

```bash
git clone <repository-url>
cd <repository-directory>
```

Make sure to replace `<repository-url>` with the actual URL of the repository.

## Compiling the Code
The project includes a Makefile to automate the compilation process.

Open a terminal in the project directory (where the Makefile is located).

To compile the server and client programs, run:

```bash
make
```

This will generate two executables:
- `server_tcp` (Server)
- `client_tcp` (Client)

The Makefile takes care of compiling all necessary files (`server_tcp.c`, `client_tcp.c`, `comptes.c`) and linking them together.

## Running the Server and Client

### Running the Server
In your terminal, navigate to the directory where the server code (`server_tcp.c`) is located.

Start the server by running the following command:

```bash
./server_tcp
```

The server will start and listen for incoming connections. You should see:

```
Serveur en attente de connexions...
```

The server is now ready to accept connections from clients.

### Running the Client
In a new terminal window, navigate to the directory where the client code (`client_tcp.c`) is located.

Start the client by running:

```bash
./client_tcp
```

The client will connect to the server. You will be prompted to enter a command, and you can use the following commands (described below).

## Using the Commands
Once the client is running, you can use the following commands to interact with the server:

### AJOUT (Add funds to an account)
Usage:

```bash
AJOUT <id_client> <id_compte> <password> <somme>
```

Example:

```bash
AJOUT 1 101 mypassword 500
```

- `id_client`: The ID of the client.
- `id_compte`: The account ID.
- `password`: The account password.
- `somme`: The amount to add to the account.

If the operation is successful, the server will respond with:

```
Montant 500.00 ajouté avec succès
```

If there is an error, such as an invalid password, the server will respond with:

```
KO - Identifiant ou mot de passe incorrect
```

### RETRAIT (Withdraw funds from an account)
Usage:

```bash
RETRAIT <id_client> <id_compte> <password> <somme>
```

Example:

```bash
RETRAIT 1 101 mypassword 200
```

If successful, the server will respond with:

```
Montant 200.00 retiré avec succès
```

If there is an error (e.g., insufficient funds), the server will respond with:

```
KO - Solde insuffisant ou erreur
```

### SOLDE (Check the balance of an account)
Usage:

```bash
SOLDE <id_client> <id_compte> <password>
```

Example:

```bash
SOLDE 1 101 mypassword
```

The server will respond with the balance:

```
Solde: 1000.00
```

If there is an error (e.g., account not found), the server will respond with:

```
KO - Compte introuvable
```

### OPERATIONS (View the last 10 operations of an account)
Usage:

```bash
OPERATIONS <id_client> <id_compte> <password>
```

Example:

```bash
OPERATIONS 1 101 mypassword
```

The server will respond with the list of operations:

```
2025-01-10 12:30:45 500.00 AJOUT
2025-01-09 10:20:00 200.00 RETRAIT
```

If there are no operations, the server will respond with:

```
KO - Aucune opération disponible
```

## Adding New Accounts
To add new accounts, you will need to modify the `init_clients()` function in the `server_tcp.c` file. Here's how to do it:

1. Open the `server_tcp.c` file.
2. Find the `init_clients()` function, which initializes the client accounts. It looks like this:

```c
void init_clients() {
    clients[0].id_client = 1;
    clients[0].compte_count = 1;
    clients[0].comptes[0].id_compte = 101;
    strcpy(clients[0].comptes[0].password, "mypassword");
    clients[0].comptes[0].solde = 1000.0;
    clients[0].comptes[0].op_count = 0;
    ...
}
```

To add a new client, follow this structure:
- Increment the client index (`clients[0]`, `clients[1]`, etc.).
- Set the `id_client`, `compte_count` (number of accounts for that client), `id_compte`, `password`, initial balance (`solde`), and `op_count` (number of operations).

Example for adding a new client:

```c
clients[3].id_client = 4;  // New client ID
clients[3].compte_count = 1;
clients[3].comptes[0].id_compte = 104;  // New account ID
strcpy(clients[3].comptes[0].password, "newpassword");  // Account password
clients[3].comptes[0].solde = 200.0;  // Initial balance
clients[3].comptes[0].op_count = 0;  // No operations yet
```

3. Save the file and recompile the project using `make`.

Now, when you start the server, this new account will be available for interaction.

## Error Handling
- **Invalid Command Format**: If the client enters a command in the wrong format, the server will respond with:

```
KO - Commande invalide
```

- **Wrong Parameters**: If the parameters (e.g., client ID, account ID, or password) are wrong, the server will respond with a relevant error message:

```
KO - Identifiant ou mot de passe incorrect
```

- **Insufficient Funds**: If there are not enough funds to perform a withdrawal, the server will respond with:

```
KO - Solde insuffisant ou erreur
```

## Closing the Application
- To stop the client, press `Ctrl+C` in the client terminal.
- To stop the server, press `Ctrl+C` in the server terminal.

## Conclusion
You have now learned how to use the Bank Account Management System. This system allows multiple clients to interact with the server to manage bank accounts and perform operations like adding funds, withdrawing funds, checking the balance, and viewing operations.

If you need to add more accounts, simply modify the `init_clients()` function as described. Enjoy experimenting with the system!
