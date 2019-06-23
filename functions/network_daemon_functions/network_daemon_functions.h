#ifndef NETWORK_DAEMON_FUNCTIONS_H_   /* Include guard */
#define NETWORK_DAEMON_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_current_block_height(char *result, const int MESSAGE_SETTINGS);
int get_previous_block_hash(char *result, const int MESSAGE_SETTINGS);
int get_previous_block_information(char *block_hash, char *block_reward, char *block_date_and_time, const int MESSAGE_SETTINGS);
int check_found_block(const int MESSAGE_SETTINGS);
#endif