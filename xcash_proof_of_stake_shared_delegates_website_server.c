#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <microhttpd.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "database_functions.h"
#include "network_wallet_functions.h"
#include "thread_functions.h"
#include "server_functions.h"

#include "xcash_proof_of_stake_shared_delegates_website_server_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // iniltize the random number generator
  srand(time(0));
  
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define DATA "{\"username\":\"XCASH\"}"

  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;  

  // threads
  pthread_t block_height_timer_thread_id;
  pthread_t payment_timer_thread_id;

  // iniltize the global variables
  xcash_wallet_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  current_block_height = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || xcash_wallet_public_address == NULL || current_block_height == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data);
    }
    if (data3 != NULL)
    {
      pointer_reset(data);
    }
    if (xcash_wallet_public_address != NULL)
    {
      pointer_reset(xcash_wallet_public_address);
    }
    if (current_block_height != NULL)
    {
      pointer_reset(current_block_height);
    }
    color_print("Could not allocate the variables on the heap","red");
    exit(0);
  }

  // initialize the network_data_nodes_list struct
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    network_data_nodes_list.network_data_nodes_public_address[count] = (char*)calloc(XCASH_WALLET_LENGTH+1,sizeof(char));
    network_data_nodes_list.network_data_nodes_IP_address[count] = (char*)calloc(BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH,sizeof(char));

    // check if the memory needed was allocated on the heap successfully
    if (network_data_nodes_list.network_data_nodes_public_address[count] == NULL || network_data_nodes_list.network_data_nodes_IP_address[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      pointer_reset_all;
      exit(0);
    }
  }

  // initialize the database connection
  mongoc_init();

  // create a connection to the database
  if (create_database_connection() == 0)
  {
    color_print("Could not create a connection for the database\n","red");
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }

  // create a pool of connections for the database
  mongoc_uri_t* uri_thread_pool;
  bson_error_t error;
  uri_thread_pool = mongoc_uri_new_with_error(DATABASE_CONNECTION, &error);
  if (!uri_thread_pool)
  {
    color_print("Could not create a pool of connections for the database\n","red");
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }
  database_client_thread_pool = mongoc_client_pool_new(uri_thread_pool);
  if (!database_client_thread_pool)
  {
    color_print("Could not create a thread pool for the database\n","red");
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }

  // write the message
  color_print("X-CASH Proof Of Stake Shared Delegates Website Server, Version 1.0.0\n","green");

  // get the wallets public address
  printf("Getting the public address\n\n");
  if (get_public_address(0) == 1)
  {  
    // print the public address
    memcpy(data,"Successfully received the public address:",41);
    memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
    memcpy(data+41+XCASH_WALLET_LENGTH,"\n",1);
    color_print(data,"green");
  }
  else
  {
    color_print("Could not get the wallets public address\n","red");
    pointer_reset_all;
    exit(0);
  }
  
  // check if the program needs to run the test
  if (parameters_count == 2)
  {
    if (strncmp(parameters[1],"--test",BUFFER_SIZE) == 0)
    {
      test();
    }
    else if (strncmp(parameters[1],"--test_data_add",BUFFER_SIZE) == 0)
    {
      memset(data,0,strlen(data));
      insert_document_into_collection_json(DATABASE_NAME,"statistics","{\"username\":\"XCASH\",\"total_blocks\":\"15\",\"block_count\":\"0\",\"total_blocks_found\":\"10\"}",0);
      
      insert_document_into_collection_json(DATABASE_NAME,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}",0);
      insert_document_into_collection_json(DATABASE_NAME,"blocks_found","{\"block_height\":\"5\",\"block_hash\":\"BLOCK_HASH\",\"block_date_and_time\":\"10\",\"block_reward\":\"15\",\"block_count\":\"10\"}",0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"current_total\":\"5\",\"total\":\"10\",\"inactivity_count\":\"15\"}",59);
      insert_document_into_collection_json(DATABASE_NAME,"public_addresses",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\",\"date_and_time\":\"5\",\"total\":\"10\",\"tx_hash\":\"TX_HASH\",\"tx_key\":\"TX_KEY\"}",73);
      insert_document_into_collection_json(DATABASE_NAME,"public_addresses_payments",data,0);
      insert_document_into_collection_json(DATABASE_NAME,"public_addresses_payments",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_created_reserve_proof\":\"",41);
      memcpy(data+41,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+139,"\",\"public_address_voted_for\":\"",30);
      memcpy(data+169,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+267,"\",\"total\":\"10\",\"reserve_proof\":\"15\"}",36);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_5",data,0);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_10",data,0);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_10",data,0);
      insert_document_into_collection_json(DATABASE_NAME,"reserve_proofs_15",data,0);

      color_print("The database test data has been added successfully\n","green");
      pointer_reset_all;
      exit(0);
    }
    else if (strncmp(parameters[1],"--test_data_remove",BUFFER_SIZE) == 0)
    {
      memset(data,0,strlen(data));
      delete_document_from_collection(DATABASE_NAME,"statistics","{\"username\":\"XCASH\"}",0);
      
      delete_document_from_collection(DATABASE_NAME,"blocks_found","{\"block_height\":\"5\"}",0);
      delete_document_from_collection(DATABASE_NAME,"blocks_found","{\"block_height\":\"5\"}",0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(DATABASE_NAME,"public_addresses",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address\":\"",19);
      memcpy(data+19,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+117,"\"}",2);
      delete_document_from_collection(DATABASE_NAME,"public_addresses_payments",data,0);
      delete_document_from_collection(DATABASE_NAME,"public_addresses_payments",data,0);

      memset(data,0,strlen(data));
      memcpy(data,"{\"public_address_voted_for\":\"",29);
      memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
      memcpy(data+127,"\"}",2);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_5",data,0);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_10",data,0);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_10",data,0);
      delete_document_from_collection(DATABASE_NAME,"reserve_proofs_15",data,0);

      color_print("The database test data has been removed successfully\n","green");
      pointer_reset_all;
      exit(0);
    }
    else
    {
      color_print("Invalid parameters\n","red");
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }

  // check if the program has the correct parameters
  if (parameters_count != 5 || strncmp(parameters[1],"--fee",BUFFER_SIZE) != 0 || strncmp(parameters[3],"--minimum_amount",BUFFER_SIZE) != 0)
  {
    color_print("Invalid parameters\n","red");
    printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }

  // convert the parameters to double and int
  sscanf(parameters[2], "%lf", &fee);
  sscanf(parameters[4], "%lld", &minimum_amount);

  // create the statistics collection if it has not already been created
  if (count_documents_in_collection(DATABASE_NAME,"statistics",DATA,0) <= 0)
  {
    insert_document_into_collection_json(DATABASE_NAME,"statistics","{\"username\":\"XCASH\",\"total_blocks\":\"0\",\"block_count\":\"0\",\"total_blocks_found\":\"0\"}",0);
  }

  // start the block_height_timer_thread
  if (pthread_create(&block_height_timer_thread_id, NULL, &block_height_timer_thread, NULL) != 0)
  {
    color_print("Could not start block height timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }
  if (pthread_detach(block_height_timer_thread_id) != 0)
  {
    color_print("Could not start block height timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }

  // start the payment_timer_thread
  if (pthread_create(&payment_timer_thread_id, NULL, &payment_timer_thread, NULL) != 0)
  {
    color_print("Could not start payment timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }
  if (pthread_detach(payment_timer_thread_id) != 0)
  {
    color_print("Could not start payment timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }

  // start the server  
  server = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,SERVER_PORT,NULL,NULL,&create_server,NULL,MHD_OPTION_NOTIFY_COMPLETED, post_request_completed,NULL,MHD_OPTION_END);
  if (server == NULL)
  {
    color_print("Could not start the server","red");
    mongoc_client_destroy(database_client);
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    pointer_reset_all;
    exit(0);
  }
  
  (void) getc (stdin);
  MHD_stop_daemon(server);
  mongoc_client_destroy(database_client);
  mongoc_client_pool_destroy(database_client_thread_pool);
  mongoc_uri_destroy(uri_thread_pool);
  mongoc_cleanup();
  pointer_reset_all;
  return 0;  

  #undef DATA 
  #undef pointer_reset_all
}