#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

/*
-----------------------------------------------------------------------------------------------------------
Global Variables
-----------------------------------------------------------------------------------------------------------
*/

// database
mongoc_client_t* database_client;
mongoc_client_pool_t* database_client_thread_pool;

char* xcash_wallet_public_address; // Holds your wallets public address
char* current_block_height; // The current block height
double fee; // the fee
long long int minimum_amount; // the minimum amount to send a payment
struct network_data_nodes_list network_data_nodes_list; // The network data nodes
#endif