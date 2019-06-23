#ifndef DEFINE_MACROS_FUNCTIONS_H_   /* Include guard */
#define DEFINE_MACROS_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

#define XCASH_DAEMON_PORT 18281 // The X-CASH Daemon RPC port
#define XCASH_WALLET_PORT 18285 // The X-CASH Wallet RPC port
#define XCASH_WALLET_PREFIX "XCA" // The prefix of a XCA address
#define XCASH_WALLET_LENGTH 98 // The length of a XCA address
#define XCASH_SIGN_DATA_PREFIX "SigV1" // The prefix of a xcash_proof_of_stake_signature for the signed data
#define XCASH_SIGN_DATA_LENGTH 93 // The length of a xcash_proof_of_stake_signature for the signed data
#define READ_FILE_BUFFER_SIZE 32768 // Buffer size for file reading (32 KB)
#define POST_DATA_MAXIMUM_ITEM_SIZE 1024 // post data item maximum size (1 KB)
#define SERVER_DATA_SIZE 33554432 // Server data size (32 MB)
#define SOCKET_CONNECTION_TIMEOUT_SETTINGS 1000 // The time in milliseconds, to wait before a connection is cancelled
#define SOCKET_DATA_TIMEOUT_SETTINGS 1 // The time in between read calls where there is no data
#define RECEIVE_DATA_TIMEOUT_SETTINGS 5 // The maximum amount of time to wait for the total data, once data has been read
#define SOCKET_END_STRING "|END|" // End string when sending data between nodes, to signal the end of sending data
#endif