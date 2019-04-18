#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "string_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_block_height
Description: Gets the current block height of the network
Parameters:
  result - The string where you want the current block height to be saved to
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_current_block_height(char *result, const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    return 0;
  }

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get current block height",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,"count",result) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
    
  pointer_reset(data); 
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_previous_block_information
Description: Gets the information of the previous block of the network
Parameters:
  block_date_and_time - The string where you want the previous block date and time to be saved to
  block_hash - The string where you want the previous block hash to be saved to
  block_reward - The string where you want the previous block reward to be saved to
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_information(char *block_hash, char *block_reward, char *block_date_and_time, const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int block_height;

  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    } 
    color_print("Could not allocate the variables on the heap","red");
    exit(0);
  }

  // get the current block height
  memset(data2,0,strnlen(data2,BUFFER_SIZE));
  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get current block height",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset_all;  
    return 0;
  }
  
  if (parse_json_data(data,"count",data2) == 0)
  {
    pointer_reset_all;
    return 0;
  }

  // get the previous block height
  sscanf(data2, "%d", &block_height);
  block_height--;
  memset(data2,0,strnlen(data2,BUFFER_SIZE));
  sprintf(data2,"%d",block_height);

  // create the message
  memset(data,0,strnlen(data,BUFFER_SIZE));
  memcpy(data,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block\",\"params\":{\"height\":",66);
  memcpy(data+66,data2,strnlen(data2,BUFFER_SIZE));
  memcpy(data+66+strnlen(data2,BUFFER_SIZE),"}}",2);

  // get the previous block information
  memset(data2,0,strnlen(data2,BUFFER_SIZE));
  if (send_http_request(data2,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,RECEIVE_DATA_TIMEOUT_SETTINGS,"get previous block information",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset_all;   
    return 0;
  }
  memset(data,0,strnlen(data,BUFFER_SIZE));

  if (parse_json_data(data2,"hash",block_hash) == 0)
  {
    pointer_reset_all; 
    return 0;
  }

  if (parse_json_data(data2,"reward",block_reward) == 0)
  {
    pointer_reset_all; 
    return 0;
  }

  if (parse_json_data(data2,"timestamp",block_date_and_time) == 0)
  {
    pointer_reset_all; 
    return 0;
  }
      
  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: check_found_block
Description: Checks to see if your wallet address found the previous block on the network
Parameters:
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if you did not find the previous block on the network, 2 if you did find the previous block on the network
-----------------------------------------------------------------------------------------------------------
*/

int check_found_block(const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* result = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int block_height;
  size_t count;
  size_t counter;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(result); \
  result = NULL; 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || result == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (result != NULL)
    {
      pointer_reset(result);
    }
    color_print("Could not allocate the variables on the heap","red");
    exit(0);
  }

  // get the current block height
  memset(result,0,strnlen(result,BUFFER_SIZE));
  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get current block height",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset_all;   
    return 0;
  }
  
  if (parse_json_data(data,"count",result) == 0)
  {
    pointer_reset_all; 
    return 0;
  }

  // get the previous block height
  sscanf(result, "%d", &block_height);
  block_height--;
  memset(result,0,strnlen(result,BUFFER_SIZE));
  sprintf(result,"%d",block_height);

  // create the message
  memset(data,0,strnlen(data,BUFFER_SIZE));
  memcpy(data,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block\",\"params\":{\"height\":",66);
  memcpy(data+66,result,strnlen(result,BUFFER_SIZE));
  memcpy(data+66+strnlen(result,BUFFER_SIZE),"}}",2);

  // get the previous block blob
  memset(result,0,strnlen(result,BUFFER_SIZE));
  if (send_http_request(result,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,data,RECEIVE_DATA_TIMEOUT_SETTINGS,"get previous block information",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset_all;   
    return 0;
  }
  memset(data,0,strnlen(data,BUFFER_SIZE));
  
  if (parse_json_data(result,"blob",data) == 0)
  {
    pointer_reset_all; 
    return 0;
  }

  // convert the public_address to hexadecimal
  memset(result,0,strnlen(result,BUFFER_SIZE));
  for (count = 0, counter = 0; count < XCASH_WALLET_LENGTH; count++, counter += 2)
  {
    sprintf(result+counter,"%02x",xcash_wallet_public_address[count] & 0xFF);
  }
  memcpy(result+196,"|",1);

  if (strstr(data,result) != NULL)
  {
    pointer_reset_all; 
    return 2;
  }
      
  pointer_reset_all; 
  return 1;

  #undef pointer_reset_all
}