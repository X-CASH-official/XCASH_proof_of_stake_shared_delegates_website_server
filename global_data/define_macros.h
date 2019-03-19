#ifndef DEFINE_MACROS_H_   /* Include guard */
#define DEFINE_MACROS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

#define SERVER_PORT 8000 // The port that is used by the server
#define BUFFER_SIZE 64000 
#define DATABASE_CONNECTION "mongodb://localhost:27017" // the database connection string
#define DATABASE_NAME "XCASH_PROOF_OF_STAKE" // The name of the database
#define DATABASE_ARRAY_COUNT 100
#define TOTAL_CONNECTION_TIME_SETTINGS 10 // The total time a client is given to connect to the server and send data
#define TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT 5 // The total time given from when the consensus node sends to the node who the main node is, to the time the main node sends the data to the node.
#define INVALID_PARAMETERS_ERROR_MESSAGE \
"Parameters\n" \
"--fee - The fee (1 would equal 1 percent. You can use up to 6 decimal places.)\n--minimum_amount The minimum for a public_address to receive a payment (10000 etc. The minimum amount should be in regular units, not atomic units.)\n"
#endif