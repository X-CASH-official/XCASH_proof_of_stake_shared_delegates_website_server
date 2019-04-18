#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_wallet_functions.h"
#include "string_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "network_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: send_wallet_http_request_test
Description: Test the send_http_request and other HTTP request wallet functions
Return: The number of passed send_wallet_http_request test
-----------------------------------------------------------------------------------------------------------
*/

int send_wallet_http_request_test()
{  
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  if (data == NULL || data2 == NULL || data3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    color_print("Could not allocate the variables on the heap","red");
    exit(0);
  }

  // define macros
  #define SEND_WALLET_HTTP_REQUEST_TOTAL_TEST 4
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"
  
  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34msend_wallet_http_request test - Total test: %d\033[0m\n",SEND_WALLET_HTTP_REQUEST_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test

  // test the send_http_request and get_public_address functions
  memset(xcash_wallet_public_address,0,strnlen(xcash_wallet_public_address,BUFFER_SIZE));
  if (get_public_address(0) == 1)
  {   
    color_print("PASSED! Test for sending an HTTP request and getting the public address of the opened X-CASH wallet","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sending an HTTP request and getting the public address of the opened X-CASH wallet","red");
  }

  // test the get_current_block_height function
  memset(data,0,strnlen(data,BUFFER_SIZE));
  if (get_current_block_height(data,0) == 1)
  {   
    color_print("PASSED! Test for getting the current block height","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the current block height","red");
  }

  // test the get_previous_block_information function
  memset(data,0,strnlen(data,BUFFER_SIZE));
  if (get_previous_block_information(data,data2,data3,0) == 1)
  {   
    color_print("PASSED! Test for getting the previous block hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the previous block hash","red");
  }

  // test the check_found_block function
  if (check_found_block(0) > 0)
  {   
    color_print("PASSED! Test for checking if your wallet found the last block","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for checking if your wallet found the last block","red");
  }

  // write the end test message
  if (count_test == SEND_WALLET_HTTP_REQUEST_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32msend_wallet_http_request test - Passed test: %d, Failed test: 0\033[0m\n",SEND_WALLET_HTTP_REQUEST_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31msend_wallet_http_request test - Passed test: %d, Failed test: %d\033[0m\n",count_test,SEND_WALLET_HTTP_REQUEST_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  pointer_reset(data);
  return count_test;

  #undef MESSAGE
  #undef SEND_WALLET_HTTP_REQUEST_TOTAL_TEST
}