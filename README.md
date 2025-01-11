# Bank Account Management System

This is a simple bank account management system implemented using C, sockets, and multithreading. The system allows multiple clients to connect to the server and perform operations such as adding funds to an account, withdrawing funds, checking the balance, and viewing the last 10 operations for an account.

### Features
- **Add Funds**: `AJOUT` command.
- **Withdraw Funds**: `RETRAIT` command.
- **Check Balance**: `SOLDE` command.
- **View Operations**: `OPERATIONS` command.
- **Handle Multiple Clients**: Server can handle multiple clients simultaneously using threads.

### Table of Contents
1. [Prerequisites](#prerequisites)
2. [Cloning the Repository](#cloning-the-repository)
3. [Compiling the Code](#compiling-the-code)
4. [Running the Server and Client](#running-the-server-and-client)
5. [Using the Commands](#using-the-commands)
6. [Adding New Accounts](#adding-new-accounts)
7. [Error Handling](#error-handling)
8. [Closing the Application](#closing-the-application)

---

## Prerequisites

Before you begin, ensure that you have the following installed:

- **C Compiler**: A C compiler like `gcc`.
- **Make**: A build automation tool for managing compilation.
- **Threads**: Ensure your system supports threading (usually supported by default on most systems).
- **Network Access**: This setup uses TCP sockets, so ensure that port 8080 is available (or modify it if necessary).

---

## Cloning the Repository

To clone the repository, use the following command in your terminal:

```bash
git clone <repository-url>
cd <repository-directory>


