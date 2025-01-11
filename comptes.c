#include "comptes.h"
#include <string.h>
#include <stdio.h>

// Fonction pour vérifier l'identité du client
int verifier_identite(Client clients[], int client_count, int id_client, int id_compte, char *password) {
    if (id_client < 1 || id_client > client_count) return 0;  // ID client invalide

    Client *client = &clients[id_client - 1];  // Client correspondant
    for (int i = 0; i < client->compte_count; i++) {
        if (client->comptes[i].id_compte == id_compte && strcmp(client->comptes[i].password, password) == 0) {
            return 1;  // Identité vérifiée
        }
    }
    return 0;  // Compte ou mot de passe incorrect
}

// Fonction pour ajouter de l'argent à un compte
int ajouter_fonds(Client clients[], int id_client, int id_compte, float somme) {
    Client *client = &clients[id_client - 1];  // Client correspondant
    for (int i = 0; i < client->compte_count; i++) {
        if (client->comptes[i].id_compte == id_compte) {
            client->comptes[i].solde += somme;

            // Ajouter l'opération
            if (client->comptes[i].op_count < MAX_OPERATIONS) {
                strcpy(client->comptes[i].operations[client->comptes[i].op_count].type, "AJOUT");
                client->comptes[i].operations[client->comptes[i].op_count].montant = somme;
                client->comptes[i].operations[client->comptes[i].op_count].date = time(NULL);
                client->comptes[i].op_count++;
            }

            return 1;  // Succès
        }
    }
    return 0;  // Compte introuvable
}

// Fonction pour retirer de l'argent d'un compte
int retirer_fonds(Client clients[], int id_client, int id_compte, float somme) {
    Client *client = &clients[id_client - 1];  // Client correspondant
    for (int i = 0; i < client->compte_count; i++) {
        if (client->comptes[i].id_compte == id_compte) {
            if (client->comptes[i].solde >= somme) {
                client->comptes[i].solde -= somme;

                // Ajouter l'opération
                if (client->comptes[i].op_count < MAX_OPERATIONS) {
                    strcpy(client->comptes[i].operations[client->comptes[i].op_count].type, "RETRAIT");
                    client->comptes[i].operations[client->comptes[i].op_count].montant = somme;
                    client->comptes[i].operations[client->comptes[i].op_count].date = time(NULL);
                    client->comptes[i].op_count++;
                }

                return 1;  // Succès
            } else {
                return 0;  // Solde insuffisant
            }
        }
    }
    return 0;  // Compte introuvable
}

// Fonction pour obtenir le solde d'un compte
float consulter_solde(Client clients[], int id_client, int id_compte) {
    Client *client = &clients[id_client - 1];  // Client correspondant
    for (int i = 0; i < client->compte_count; i++) {
        if (client->comptes[i].id_compte == id_compte) {
            return client->comptes[i].solde;
        }
    }
    return -1;  // Compte introuvable
}

// Fonction pour obtenir l'historique des opérations
int consulter_operations(Client clients[], int id_client, int id_compte, Operation *operations) {
    Client *client = &clients[id_client - 1];  // Client correspondant
    for (int i = 0; i < client->compte_count; i++) {
        if (client->comptes[i].id_compte == id_compte) {
            int op_count = client->comptes[i].op_count;
            for (int j = 0; j < op_count; j++) {
                operations[j] = client->comptes[i].operations[j];
            }
            return op_count;  // Retourne le nombre d'opérations
        }
    }
    return 0;  // Compte introuvable
}
