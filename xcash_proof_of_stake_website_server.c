#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <microhttpd.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "database_functions.h"
#include "thread_functions.h"
#include "server_functions.h"

#include "xcash_proof_of_stake_website_server_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // Variables
  pthread_t block_height_timer_thread_id;
  pthread_t payment_timer_thread_id;

  // iniltize the global variables
  xcash_wallet_public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (xcash_wallet_public_address == NULL)
  {
    color_print("Could not allocate the variables on the heap","red");
    exit(0);
  }

  // initialize the database connection
  mongoc_init();

  // create a connection to the database
  if (create_database_connection() == 0)
  {
    color_print("Could not create a connection for the database\n","red");
    mongoc_cleanup();
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
    exit(0);
  }
  database_client_thread_pool = mongoc_client_pool_new(uri_thread_pool);
  if (!database_client_thread_pool)
  {
    color_print("Could not create a thread pool for the database\n","red");
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    exit(0);
  }

  // write the message
  color_print("X-CASH Proof Of Stake Website Server, Version 1.0.0\n","green");
  
  // check if the program needs to run the test
  if (parameters_count == 2)
  {
    if (strncmp(parameters[1],"--test",BUFFER_SIZE) == 0)
    {
      test();
    }
    else
    {
      color_print("Invalid parameters\n","red");
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
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
    exit(0);
  }

  // convert the parameters to double and int
  sscanf(parameters[2], "%lf", &fee);
  sscanf(parameters[4], "%lld", &minimum_amount);

  // start the block_height_timer_thread
  if (pthread_create(&block_height_timer_thread_id, NULL, &block_height_timer_thread, NULL) != 0)
  {
    color_print("Could not start block height timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    exit(0);
  }
  if (pthread_detach(block_height_timer_thread_id) != 0)
  {
    color_print("Could not start block height timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    exit(0);
  }

  // start the payment_timer_thread
  if (pthread_create(&payment_timer_thread_id, NULL, &payment_timer_thread, NULL) != 0)
  {
    color_print("Could not start payment timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
    exit(0);
  }
  if (pthread_detach(payment_timer_thread_id) != 0)
  {
    color_print("Could not start payment timer","red");
    mongoc_client_pool_destroy(database_client_thread_pool);
    mongoc_uri_destroy(uri_thread_pool);
    mongoc_cleanup();
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
    exit(0);
  }

  (void) getc (stdin);
  MHD_stop_daemon(server);
  mongoc_client_destroy(database_client);
  mongoc_client_pool_destroy(database_client_thread_pool);
  mongoc_uri_destroy(uri_thread_pool);
  mongoc_cleanup();
  return 0;   
}