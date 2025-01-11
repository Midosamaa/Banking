#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PASSWORD_LEN 20
#define MAX_BUFFER_SIZE 1024

void print_commands() {
    printf("\nVoici une liste des commandes possibles :\n");
    printf("1. AJOUT <id_client> <id_compte> <password> <somme> - Ajouter de l'argent à un compte.\n");
    printf("2. RETRAIT <id_client> <id_compte> <password> <somme> - Retirer de l'argent d'un compte.\n");
    printf("3. SOLDE <id_client> <id_compte> <password> - Consulter le solde d'un compte.\n");
    printf("4. OPERATIONS <id_client> <id_compte> <password> - Voir les 10 dernières opérations d'un compte.\n");
    printf("\nEntrez la commande souhaitée (exemple: AJOUT 1 101 mypassword 500) : ");
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erreur création socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];

    while (1) {
        print_commands();
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Enlever le caractère de nouvelle ligne

        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Erreur envoi");
            break;
        }

        int bytes_received = recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("Erreur réception");
            break;
        }

        buffer[bytes_received] = '\0';  // Null terminate the response
        printf("Réponse du serveur : %s\n", buffer);
    }

    close(client_socket);
    return 0;
}
