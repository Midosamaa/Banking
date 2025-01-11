#ifndef COMPTES_H
#define COMPTES_H

#include <time.h>

#define MAX_OPERATIONS 10
#define PASSWORD_LEN 20
#define MAX_CLIENTS 3

// Structure pour stocker les informations sur chaque opération
typedef struct {
    char type[10];  // "AJOUT" ou "RETRAIT"
    float montant;
    time_t date; // Date de l'opération
} Operation;

// Structure pour stocker les informations de chaque compte
typedef struct {
    int id_compte;
    char password[PASSWORD_LEN];
    float solde;
    int op_count;  // Nombre d'opérations effectuées
    Operation operations[MAX_OPERATIONS];  // Historique des opérations
} Compte;

// Structure pour stocker les informations sur chaque client
typedef struct {
    int id_client;
    int compte_count;
    Compte comptes[MAX_OPERATIONS];  // Limité à MAX_OPERATIONS comptes
} Client;

// Fonction pour vérifier l'identité du client
int verifier_identite(Client clients[], int client_count, int id_client, int id_compte, char *password);

// Fonction pour ajouter de l'argent à un compte
int ajouter_fonds(Client clients[], int id_client, int id_compte, float somme);

// Fonction pour retirer de l'argent d'un compte
int retirer_fonds(Client clients[], int id_client, int id_compte, float somme);

// Fonction pour obtenir le solde d'un compte
float consulter_solde(Client clients[], int id_client, int id_compte);

// Fonction pour obtenir l'historique des opérations
int consulter_operations(Client clients[], int id_client, int id_compte, Operation *operations);

#endif
