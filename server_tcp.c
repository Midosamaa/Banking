#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "comptes.h"

// Variables globales
#define MAX_CLIENTS 3
Client clients[MAX_CLIENTS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex pour synchronisation des threads

// Fonction pour initialiser les clients et leurs comptes
void init_clients() {
    clients[0].id_client = 1;
    clients[0].compte_count = 1;
    clients[0].comptes[0].id_compte = 101;
    strcpy(clients[0].comptes[0].password, "mypassword");
    clients[0].comptes[0].solde = 1000.0;
    clients[0].comptes[0].op_count = 0;

    clients[1].id_client = 2;
    clients[1].compte_count = 1;
    clients[1].comptes[0].id_compte = 102;
    strcpy(clients[1].comptes[0].password, "pass123");
    clients[1].comptes[0].solde = 500.0;
    clients[1].comptes[0].op_count = 0;

    clients[2].id_client = 3;
    clients[2].compte_count = 1;
    clients[2].comptes[0].id_compte = 103;
    strcpy(clients[2].comptes[0].password, "securepass");
    clients[2].comptes[0].solde = 750.0;
    clients[2].comptes[0].op_count = 0;
}

// Fonction pour traiter la commande et générer la réponse
void traiter_commande(char *commande, char *reponse, int client_socket) {
    int id_client, id_compte;
    char password[PASSWORD_LEN];
    float somme;
    Compte *compte;
    Operation operations[MAX_OPERATIONS];
    int op_count;

    if (sscanf(commande, "AJOUT %d %d %s %f", &id_client, &id_compte, password, &somme) == 4) {
        pthread_mutex_lock(&mutex);  // Verrouillage avant traitement
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            if (ajouter_fonds(clients, id_client, id_compte, somme)) {
                printf("OK\n");
                sprintf(reponse, "Montant %.2f ajouté avec succès", somme);
            } else {
                printf("KO\n");
                sprintf(reponse, "KO - Erreur lors de l'ajout");
            }
        } else {
            printf("KO\n");
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);  // Déverrouillage après traitement
    }
    else if (sscanf(commande, "RETRAIT %d %d %s %f", &id_client, &id_compte, password, &somme) == 4) {
        pthread_mutex_lock(&mutex);
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            if (retirer_fonds(clients, id_client, id_compte, somme)) {
                printf("OK\n");
                sprintf(reponse, "Montant %.2f retiré avec succès", somme);
            } else {
                printf("KO\n");
                sprintf(reponse, "KO - Solde insuffisant ou erreur");
            }
        } else {
            printf("KO\n");
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);
    }
    else if (sscanf(commande, "SOLDE %d %d %s", &id_client, &id_compte, password) == 3) {
        pthread_mutex_lock(&mutex);
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            float solde = consulter_solde(clients, id_client, id_compte);
            if (solde >= 0) {
                printf("OK\n");
                sprintf(reponse, "Solde: %.2f", solde);
            } else {
                printf("KO\n");
                sprintf(reponse, "KO - Compte introuvable");
            }
        } else {
            printf("KO\n");
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);
    }
    else if (sscanf(commande, "OPERATIONS %d %d %s", &id_client, &id_compte, password) == 3) {
        pthread_mutex_lock(&mutex);
        if (verifier_identite(clients, MAX_CLIENTS, id_client, id_compte, password)) {
            op_count = consulter_operations(clients, id_client, id_compte, operations);
            if (op_count > 0) {
                printf("OK\n");
                for (int i = 0; i < op_count; i++) {
                    struct tm *tm_info = localtime(&operations[i].date);
                    char buffer[80];
                    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", tm_info);
                    printf("%s %.2f %s\n", buffer, operations[i].montant, operations[i].type);
                }
                sprintf(reponse, "Historique des opérations consulté");
            } else {
                printf("KO\n");
                sprintf(reponse, "KO - Aucune opération disponible");
            }
        } else {
            printf("KO\n");
            sprintf(reponse, "KO - Identifiant ou mot de passe incorrect");
        }
        pthread_mutex_unlock(&mutex);
    }
    else {
        printf("KO\n");
        sprintf(reponse, "KO - Commande invalide");
    }
}

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[1024];
    char reponse[1024];  // Buffer pour la réponse

    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("Client déconnecté.\n");
            } else {
                perror("Erreur de réception");
            }
            break;
        }

        buffer[bytes_received] = '\0'; // Null terminate string
        printf("Reçu du client : %s\n", buffer);

        traiter_commande(buffer, reponse, client_socket);

        if (send(client_socket, reponse, strlen(reponse), 0) == -1) {
            perror("Erreur d'envoi");
            break;
        }
    }

    close(client_socket);
    free(arg);
    pthread_exit(NULL);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erreur création socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Erreur listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions...\n");

    init_clients();

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

        if (*client_socket == -1) {
            perror("Erreur accept");
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, client_socket);
        pthread_detach(thread_id);
    }

    close(server_socket);
    return 0;
}
