#ifndef DEFINE_MACROS_H_   /* Include guard */
#define DEFINE_MACROS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

#define SERVER_PORT 8000 // The port that is used by the server
#define SEND_DATA_PORT 8000 // The port that is used by all nodes to send and receive data
#define BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH 100 // The maximum length of the block verifiers IP address
#define BUFFER_SIZE 64000 
#define RANDOM_STRING_LENGTH 100 // The length of the random string
#define RESERVE_PROOFS_LENGTH 100 
#define BLOCKCHAIN_RESERVED_BYTES_START "7c424c4f434b434841494e5f52455345525645445f42595445535f53544152547c"
#define BLOCKCHAIN_DATA_SEGMENT_STRING "7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c"
#define DATABASE_CONNECTION "mongodb://localhost:27017" // the database connection string
#define DATABASE_NAME "XCASH_PROOF_OF_STAKE" // The name of the database
#define DATABASE_ARRAY_COUNT 100
#define NETWORK_DATA_NODES_AMOUNT 100 // The amount of block verifiers
#define DATA_HASH_LENGTH 128 // The length of the SHA2-512 hash
#define TOTAL_CONNECTION_TIME_SETTINGS 10 // The total time a client is given to connect to the server and send data
#define TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT 5 // The total time given from when the consensus node sends to the node who the main node is, to the time the main node sends the data to the node.
#define INVALID_PARAMETERS_ERROR_MESSAGE \
"Parameters\n" \
"--fee - The fee (1 would equal 1 percent. You can use up to 6 decimal places.)\n" \
"--minimum_amount The minimum for a public_address to receive a payment (10000 etc. The minimum amount should be in regular units, not atomic units.)\n" \
"--test - Run the test to make sure the program is compatible with your system\n" \
"--test_data_add - Add test data to the databases\n" \
"--test_data_remove - Remove test data from the databases\n"
#endif