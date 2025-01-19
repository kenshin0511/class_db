#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// *** Partie 1 : Définition des types et des constantes ***

// Enumération pour les résultats des méta-commandes
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

// Enumération pour les résultats de la préparation des commandes
typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

// Enumération pour les types de commandes SQL-like
typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

// Structure pour représenter une commande SQL-like
typedef struct {
    StatementType type; // Type de commande (`insert`, `select`, etc.)
} Statement;

// Structure pour gérer les entrées utilisateur
typedef struct {
    char* buffer;        // Tampon pour stocker l'entrée utilisateur
    size_t buffer_length; // Taille du tampon
    ssize_t input_length; // Longueur de l'entrée réelle
} InputBuffer;

// *** Partie 2 : Gestion du tampon d'entrée ***

// Fonction pour créer un nouveau tampon d'entrée
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;            // Initialisation du tampon
    input_buffer->buffer_length = 0;        // Longueur initiale à 0
    input_buffer->input_length = 0;         // Pas encore d'entrée
    return input_buffer;
}

// Fonction pour libérer la mémoire du tampon d'entrée
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer); // Libère le tampon
    free(input_buffer);         // Libère la structure
}

// Fonction pour afficher l'invite de commande
void print_prompt() {
    printf("db > ");
}

// Fonction pour lire l'entrée utilisateur
void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) { // Gestion des erreurs de lecture
        printf("Erreur lors de la lecture de l'entrée.\n");
        exit(EXIT_FAILURE);
    }

    // Supprime le saut de ligne final
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = '\0';
}

// *** Partie 3 : Gestion des méta-commandes ***

// Fonction pour gérer les commandes qui commencent par un point (.)
MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer); // Libération des ressources
        exit(EXIT_SUCCESS);              // Quitte le programme
    }
    // Si la commande est inconnue
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}

// *** Partie 4 : Préparation des commandes SQL-like ***

// Fonction pour analyser les commandes de type SQL-like
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT; // Commande inconnue
}

// *** Partie 5 : Exécution des commandes ***

// Fonction pour exécuter les commandes reconnues
void execute_statement(Statement* statement) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            printf("Commande 'insert' exécutée.\n");
            break;
        case STATEMENT_SELECT:
            printf("Commande 'select' exécutée.\n");
            break;
    }
}

// *** Partie 6 : Boucle principale (REPL) ***

// Fonction REPL (Read-Eval-Print Loop)
void repl() {
    InputBuffer* input_buffer = new_input_buffer();

    while (true) {
        print_prompt();               // Affiche l'invite de commande
        read_input(input_buffer);     // Lit l'entrée utilisateur

        if (input_buffer->buffer[0] == '.') { // Gestion des méta-commandes
            switch (do_meta_command(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Commande inconnue : '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        // Préparation des commandes SQL-like
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break; // Commande reconnue
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Mot-clé non reconnu au début de '%s'.\n", input_buffer->buffer);
                continue;
        }

        // Exécution de la commande
        execute_statement(&statement);
        printf("Exécution terminée.\n");
    }
}


