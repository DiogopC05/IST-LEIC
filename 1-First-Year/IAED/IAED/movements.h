/**
 * @file movements.h
 * @author Diogo Carreira
 * @date March 2024
 * @brief Contains the data structures and functions for managing park movements.
 */
#ifndef MOVEMENTS_H
#define MOVEMENTS_H

/**
 * @brief Represents a movement in a park.
 *
 * @param plate The license plate of the vehicle involved in the movement.
 * @param parkName The name of the park where the movement occurred.
 * @param date The date when the movement occurred.
 * @param command The command that represents the type of movement (entry or exit).
 * @param prev Pointer to the previous movement in the linked list of movements.
 * @param next Pointer to the next movement in the linked list of movements.
 */
typedef struct Movement {
    char *plate; 
    char *parkName; 
    Date date;  
    char command;      
    struct Movement *prev;
    struct Movement *next;
} Movement;

/**
 * @brief Represents a node in a hash table.
 *
 * @param key The key of the node.
 * @param value The value of the node, which is a movement.
 * @param next Pointer to the next node in the linked list of nodes.
 */
typedef struct Node {
    char *key;
    Movement *value;
    struct Node *next;
} Node;

/**
 * @brief Represents a hash table.
 *
 * @param buckets The array of linked lists of nodes.
 * @param size The number of buckets in the hash table.
 */
typedef struct HashTable {
    Node **buckets;
    int size;
} HashTable;

/**
 * @brief Represents a node in a billing hash table.
 *
 * @param key The key of the node.
 * @param value The value of the node, which is a movement.
 * @param bill The bill associated with the node.
 * @param next Pointer to the next node in the linked list of nodes.
 */
typedef struct BillingNode {
    char *key;
    Movement *value;
    double bill;
    struct BillingNode *next;
} BillingNode;

/**
 * @brief Represents a billing hash table.
 *
 * @param buckets The array of linked lists of nodes.
 * @param size The number of buckets in the billing hash table.
 */
typedef struct BillingHashTable {
    BillingNode **buckets;
    int size;
} BillingHashTable;


Movement*  add_movement(Movement **head, char *plate, char *parkName, Date date, char command);
void free_all_movements(Movement *head);
void remove_movements(Movement **head, char *parkName);
Date get_last_movement_date(Movement *head);
Movement* find_entry_movement(Movement *head, char *plateVehicle);
Movement* get_tail(Movement *head);
unsigned int hash_function(char *str);
HashTable *hash_table_create(int size);
void hash_table_add(HashTable *hash_table, char *key, Movement *value);
Node* hash_table_get(HashTable *hash_table, char *key);
void hash_table_remove(HashTable *hash_table, char *parkName);
void insert_node(Node **bucket, Node *new_node);
void print_movement_details(Node *node);
void hash_table_print(HashTable *hash_table);
void hash_table_free(HashTable *hash_table);
BillingHashTable *bill_hash_table_create(int size);
void bill_hash_table_add(BillingHashTable *hash_table, char *key, Movement *value, double bill);
BillingNode* bill_hash_table_get(BillingHashTable *hash_table, char *key);
void bill_hash_table_remove(BillingHashTable *hash_table, char *parkName);
void bill_hash_table_free(BillingHashTable *billing);

#endif 