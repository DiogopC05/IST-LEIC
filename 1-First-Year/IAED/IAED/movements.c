#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj.h"
#include "movements.h"
#include "auxiliary.h"

/**
 * @brief Creates a new movement and adds it to the double linked list of
 * movements.
 * 
 * @param head Pointer to the head of the linked list of movements.
 * @param plate Pointer to the string representing the vehicle's plate.
 * @param parkName Pointer to the string representing the park's name.
 * @param date Struct representing the date of the movement.
 * @param command Character representing the command of the movement.
 * @return Pointer to the newly created movement.
 */
Movement* add_movement(Movement **head, 
                        char *plate, 
                        char *parkName, 
                        Date date, 
                        char command) {

    Movement *newMovement = (Movement*)malloc(sizeof(Movement));
    
    newMovement->plate = strdup(plate);
    newMovement->parkName = strdup(parkName);

    /// Assign the date and command to the new movement
    newMovement->date = date;
    newMovement->command = command;

    newMovement->prev = NULL;
    newMovement->next = NULL;
    
    if (*head == NULL) 
        *head = newMovement;
    
    // If the linked list is not empty, add the new movement in the end
    else {
        Movement *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newMovement;
        newMovement->prev = current;
    }

    return newMovement;
}

/**
 * @brief Frees all movements in a linked list.
 *
 * @param head Pointer to the head of the double linked list of movements.
 */
void free_all_movements(Movement *head) {
    Movement *current = head;
    while (current != NULL) { ///Iterate over the double linked list
        Movement *temp = current;
        current = current->next;
        if (current != NULL) {
            current->prev = NULL;
        }
        free(temp->plate);
        free(temp->parkName);
        free(temp);
    }
}

/**
 * Function to remove all movements from a linked list that match 
 * a specific park name.
 *
 * @param head Pointer to the head of the linked list of movements.
 * @param parkName Pointer to the string representing the park's name.
 */
void remove_movements(Movement **head, char *parkName) {
    /// Start at the head of the list
    Movement *current = *head;

    /// Traverse the list
    while (current != NULL) {
        /// If the current node's parkName matches the given parkName
        if (strcmp(current->parkName, parkName) == 0) {
            /// If the node is at the head of the list
            if (current->prev == NULL) {
                /// Make the next node the new head
                *head = current->next;
                /// Set the prev pointer of the new head to NULL
                if (current->next != NULL) {
                    current->next->prev = NULL;
                }
            } else {
                /// If the node is in the middle or at the end of the list
                /// Update the next pointer of the previous node
                current->prev->next = current->next;
                /// Update the prev pointer of the next node
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
            }
            /// Store the node to be deleted
            Movement *temp = current;
            /// Move to the next node
            current = current->next;
            /// Free the memory allocated for the plate, parkName and node 
            free(temp->plate);
            free(temp->parkName);
            free(temp);
        } else 
            current = current->next;
    }
}

/**
 * @brief Retrieves the date of the last movement in a linked list.
 *
 * @param head Pointer to the head of the linked list of movements.
 * @return The date of the last movement. If the list is empty, returns a 
 * default date.
 */
Date get_last_movement_date(Movement *head) {
    Movement *current = head;
    /// If the list is empty, return a default Date
    if (current == NULL) {
        /// Replace with your actual default Date
        Date defaultDate = {0, 0, 0, {0,0}}; 
        return defaultDate;
    }
    /// Traverse the list to find the last Movement
    while (current != NULL && current->next != NULL) {
        current = current->next;
    }
    
    /// Return the date of the last Movement
    return current->date;
}

/**
 * @brief Retrieves the tail of a linked list of movements.
 *
 * @param head Pointer to the head of the linked list of movements.
 * @return Pointer to the tail of the linked list.
 */
Movement* get_tail(Movement *head) {
    Movement *current = head;
    while (current != NULL && current->next != NULL) {
        current = current->next;
    }
    return current;
}

/**
 * @brief Retrieves the tail of a linked list of movements.
 *
 * @param head Pointer to the head of the linked list of movements.
 * @return Pointer to the tail of the linked list. If the list is empty, 
 * returns NULL.
 */
Movement* find_entry_movement(Movement *head, char *plateVehicle) {
    Movement *current = get_tail(head);
    while (current != NULL) {
        /// Check if the current movement is an entry movement
        if (current->command == COMMAND_E && 
            strcmp(current->plate, plateVehicle) == 0) 
            return current;
        
        current = current->prev;
    }
    return NULL;
}

/**
 * @brief Computes a hash value for a given string.
 *
 * @param str The string to compute the hash value for.
 * @return The computed hash value.
 */
unsigned int hash_function(char *str) {
    unsigned int hash = HASH_INIT;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 

    return hash;
}

/**
 * @brief Creates a new hash table with a specified size.
 *
 * @param size The size of the hash table to create.
 * @return Pointer to the newly created hash table.
 */
HashTable *hash_table_create(int size) {
    HashTable *hash_table = malloc(sizeof(HashTable));
    hash_table->buckets = malloc(sizeof(Node*) * size);
    hash_table->size = size;

    /// Initialize all buckets to NULL
    for (int i = 0; i < size; i++) {
        hash_table->buckets[i] = NULL;
    }

    return hash_table;
}

/**
 * @brief Computes a secondary hash value for a given string.
 *
 * @param str The string to compute the hash value for.
 * @return The computed secondary hash value.
 */
unsigned int secondary_hash_function(char *str) {
    unsigned int hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 7) + (hash << 15) - hash;

    return hash;
}

/**
 * @brief Adds a new key-value pair to a hash table.
 *
 * @param hash_table The hash table to add the key-value pair to.
 * @param key The key of the new key-value pair.
 * @param value The value of the new key-value pair.
 */
void hash_table_add(HashTable *hash_table, char *key, Movement *value) {
    int hash = hash_function(key) % hash_table->size;

    Node *new_node = malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->value = value;

    if (hash_table->buckets[hash] == NULL || 
        strcmp(hash_table->buckets[hash]->key, key) == 0) {
        insert_node(&hash_table->buckets[hash], new_node);
    } else {
        int new_hash = secondary_hash_function(key) % hash_table->size;
        insert_node(&hash_table->buckets[new_hash], new_node);
    }
}

/**
 * @brief Inserts a new node into a sorted linked list.
 *
 * @param bucket Pointer to the head of the linked list.
 * @param new_node The new node to insert.
 */
void insert_node(Node **bucket, Node *new_node) {
    Node *current = *bucket;
    Node *previous = NULL;

    while (current != NULL && 
        strcmp(current->value->parkName, new_node->value->parkName) <= 0){
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {
        new_node->next = *bucket;
        *bucket = new_node;
    } else {
        new_node->next = current;
        previous->next = new_node;
    }
}

/**
 * @brief Retrieves a node from a hash table by its key.
 *
 * @param hash_table The hash table to retrieve the node from.
 * @param key The key of the node to retrieve.
 * @return The node with the specified key. If no such node exists, 
 * returns NULL.
 */
Node* hash_table_get(HashTable *hash_table, char *key) {
    /// Compute the hash of the key
    int hash = hash_function(key) % hash_table->size;

    /// Search for the key in the linked list at the bucket index
    Node *current = hash_table->buckets[hash];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            /// Key found, return the value
            return current;
        }
        current = current->next;
    }

    /// If key not found, compute the secondary hash and search again
    int new_hash = secondary_hash_function(key) % hash_table->size;
    current = hash_table->buckets[new_hash];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            /// Key found, return the value
            return current;
        }
        current = current->next;
    }

    /// Key not found in both hashes, return NULL
    return NULL;
}

/**
 * @brief Prints the details of a series of movements.
 *
 * @param node Pointer to the head of the linked list of movements.
 */
void print_movement_details(Node *node) {
    while (node != NULL) {
        Movement *movement = node->value;
        printf("%s ", movement->parkName);
        format_date(movement->date);

        node = node->next;

        if (node != NULL) {
            Movement *nextMovement = node->value;
            if (nextMovement->command != COMMAND_E) {
                printf(" ");
                format_date(nextMovement->date);
            }
        }
        printf("\n");

        if (node != NULL && node->value->command != COMMAND_E) {
            node = node->next;
        }
    }
}

/**
 * @brief Removes all nodes with a specific park name from a hash table.
 *
 * @param hash_table The hash table to remove the nodes from.
 * @param parkName The park name of the nodes to remove.
 */
void hash_table_remove(HashTable *hash_table, char *parkName) {
    // Iterate over all buckets in the hash table
    for (int i = 0; i < hash_table->size; i++) {
        Node *current = hash_table->buckets[i];
        Node *previous = NULL;
        while (current != NULL) {
            // If the current node's parkName matches the given parkName
            if (strcmp(current->value->parkName, parkName) == 0) {
                // If the node is at the beginning of the list
                if (previous == NULL) {
                    hash_table->buckets[i] = current->next;
                } else {
                    // If the node is in the middle or end of the list
                    previous->next = current->next;
                }

                // Store the next node before freeing the current node
                Node *nextNode = current->next;

                // Free the Node and its data
                free(current->key);
                free(current);

                // Move to the next node
                current = nextNode;
            } else {
                previous = current;
                current = current->next;
            }
        }
    }
}

/**
 * @brief Frees all memory allocated for a hash table.
 *
 * @param hash_table The hash table to free.
 */
void hash_table_free(HashTable *hash_table) {
    // Iterate over each bucket in the hash table
    for (int i = 0; i < hash_table->size; i++) {
        Node *node = hash_table->buckets[i];

        // Free the linked list in each bucket
        while (node != NULL) {
            Node *next_node = node->next;
            free(node->key);
            free(node);
            node = next_node;
        }
    }
    // Free the array of buckets
    free(hash_table->buckets);

    // Free the hash table itself
    free(hash_table);
}

BillingHashTable *bill_hash_table_create(int size) {
    BillingHashTable *hash_table = malloc(sizeof(BillingHashTable));
    hash_table->buckets = malloc(sizeof(BillingNode*) * size);
    hash_table->size = size;

    // Initialize all buckets to NULL
    for (int i = 0; i < size; i++) {
        hash_table->buckets[i] = NULL;
    }

    return hash_table;
}

/**
 * @brief Adds a new node to a billing hash table.
 *
 * @param hash_table The billing hash table to add the node to.
 * @param key The key of the new node.
 * @param value The value of the new node.
 * @param bill The bill associated with the new node.
 */
void bill_hash_table_add(BillingHashTable *hash_table, 
                        char *key, 
                        Movement *value, 
                        double bill) {

    // Compute the hash of the key
    int hash = hash_function(key) % hash_table->size;

    // Create a new node
    BillingNode *new_node = malloc(sizeof(BillingNode));
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->bill = bill;
    new_node->next = NULL;

    // If the bucket is empty, add the new node directly
    if (hash_table->buckets[hash] == NULL) {
        hash_table->buckets[hash] = new_node;
    } else {
        // Otherwise, append the new node to the end of the linked list
        BillingNode *current = hash_table->buckets[hash];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

/**
 * @brief Retrieves a node from a billing hash table by its key.
 *
 * @param hash_table The billing hash table to retrieve the node from.
 * @param key The key of the node to retrieve.
 * @return The node with the specified key. If no such node exists, 
 * returns NULL.
 */
BillingNode* bill_hash_table_get(BillingHashTable *hash_table, char *key) {
    // Compute the hash of the key
    int hash = hash_function(key) % hash_table->size;

    // Search for the key in the linked list at the bucket index
    BillingNode *current = hash_table->buckets[hash];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // Key found, return the node
            return current;
        }
        current = current->next;
    }

    // Key not found, return NULL
    return NULL;
}

/**
 * @brief Removes all nodes with a specific park name from a billing 
 * hash table.
 *
 * @param hash_table The billing hash table to remove the nodes from.
 * @param parkName The park name of the nodes to remove.
 */
void bill_hash_table_remove(BillingHashTable *hash_table, char *parkName) {      
    /// Compute the hash of the parkName   
    int hash = hash_function(parkName) % hash_table->size;

    /// Search for the parkName in the linked list at the bucket index
    BillingNode *current = hash_table->buckets[hash];
    BillingNode *previous = NULL;
    while (current != NULL) {
        /// If the current node's key matches the given parkName
        if (strcmp(current->key, parkName) == 0) {
            /// If the node is at the beginning of the list
            if (previous == NULL) {
                hash_table->buckets[hash] = current->next;
            } else {
                /// If the node is in the middle or at the end of the list
                previous->next = current->next;
            }

            /// Store the next node before freeing the current node
            BillingNode *nextNode = current->next;

            /// Free the key and the BillingNode 
            free(current->key);
            free(current);

            /// Move to the next node
            current = nextNode;
        } else {
            previous = current;
            current = current->next;
        }
    }
}

/**
 * @brief Frees all memory allocated for a billing hash table.
 *
 * @param billing The billing hash table to free.
 */
void bill_hash_table_free(BillingHashTable *billing) {
    /// Iterate over each bucket in the hash table
    for (int i = 0; i < billing->size; i++) {
        BillingNode *node = billing->buckets[i];

        /// Free the linked list in each bucket
        while (node != NULL) {
            BillingNode *next_node = node->next;

            /// Free the key and the value
            free(node->key);
           
            /// Free the node itself
            free(node);

            node = next_node;
        }
    }

    // Free the array of buckets
    free(billing->buckets);

    // Free the hash table itself
    free(billing);
}