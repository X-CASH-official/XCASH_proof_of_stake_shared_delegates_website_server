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
#include "network_security_functions.h"
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
Name: network_daemon_test
Description: Test the network_daemon functions
Return: The number of passed network_daemon test
-----------------------------------------------------------------------------------------------------------
*/

void network_daemon_test()
{  
  // Variables
  size_t count;

  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));

  // run the test

  // test the get_current_block_height function
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_current_block_height(data_test,0) == 1)
  {   
    color_print("PASSED! Test for getting the current block height","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the current block height","red");
  }

  // test the get_previous_block_hash function
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (get_previous_block_hash(data_test,0) == 1)
  {   
    color_print("PASSED! Test for getting the previous block hash","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for getting the previous block hash","red");
  }
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_security_test
Description: Test the network_security functions
Return: The number of passed network_security test
-----------------------------------------------------------------------------------------------------------
*/

void network_security_test()
{  
  // define macros
  #define MESSAGE "{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}"
  
  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));

  // create the message
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memcpy(result_test,MESSAGE,strnlen(MESSAGE,BUFFER_SIZE));

  // test the sign_data functions
  if (sign_data(result_test) == 1)
  {   
    color_print("PASSED! Test for sign data to send data securely","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for sign data to send data securely","red");
  }
  
  // test the verify_data functions
  if (verify_data(result_test) == 1)
  {   
    color_print("PASSED! Test for verify data to receive data securely","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for verify data to receive data securely","red");
  }
  #undef MESSAGE
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_wallet_test
Description: Test the network_wallet functions
Return: The number of passed network_wallet test
-----------------------------------------------------------------------------------------------------------
*/

void network_wallet_test()
{ 
  // define macros
  
  // test the get_public_address function
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
}



/*
-----------------------------------------------------------------------------------------------------------
Name: network_test
Description: Runs all of the network test
Return: The number of passed network test
-----------------------------------------------------------------------------------------------------------
*/

int network_functions_test()
{
  // define macros
  #define NETWORK_TOTAL_TEST 5

  // reset the varaibles
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mnetwork test - Total test: %d\033[0m\n",NETWORK_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test
  network_daemon_test();
  network_security_test();
  network_wallet_test();

  // write the end test message
  if (count_test == NETWORK_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mnetwork test - Passed test: %d, Failed test: 0\033[0m\n",NETWORK_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mnetwork test - Passed test: %d, Failed test: %d\033[0m\n",count_test,NETWORK_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }
  return count_test;

  #undef NETWORK_TOTAL_TEST
}