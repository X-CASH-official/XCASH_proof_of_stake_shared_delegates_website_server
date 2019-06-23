#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "variables.h"
#include "structures.h"

#include "define_macros_functions.h"
#include "database_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_wallet_functions.h"
#include "string_functions.h"
#include "thread_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/



/*
-----------------------------------------------------------------------------------------------------------
Name: block_height_timer_thread
Description: checks for a new block
-----------------------------------------------------------------------------------------------------------
*/

void* block_height_timer_thread()
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* block_reward = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* block_height = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* datacopy1;
  char* datacopy2;
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  int count;
  int count2;
  int count3;
  int counter;
  int public_address_count;
  long long int number;
  long long int block_reward_number;
  long long int total_votes;
  long long int block_reward_for_public_address;
  struct database_multiple_documents_fields database_data;

  // define macros
  #define DATA "{\"username\":\"XCASH\"}"

  #define BLOCK_HEIGHT_TIMER_THREAD_ERROR(message) \
  fprintf(stderr,"\033[1;31mError:\nTime = %ld\n%s\033[0m\n\n",time(NULL),message); \
  sleep(1); \
  goto start;

  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(data4); \
  data4 = NULL; \
  free(block_reward); \
  block_reward = NULL; \
  free(block_height); \
  block_height = NULL; 

  #define pointer_reset_database_array \
  for (count = 0; count < public_address_count; count++) \
  { \
    for (counter = 0; counter < 4; counter++) \
    { \
      pointer_reset(database_data.item[count][counter]); \
      pointer_reset(database_data.value[count][counter]); \
    } \
  }

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || data4 == NULL || block_reward == NULL || block_height == NULL)
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
    if (data4 != NULL)
    {
      pointer_reset(data4);
    }
    if (block_reward != NULL)
    {
      pointer_reset(block_reward);
    }
    if (block_height != NULL)
    {
      pointer_reset(block_height);
    }    
    color_print("Could not allocate the variables on the heap","red");
    exit(0);
  }

  for (;;)
  {
    start:
    // pause 200 milliseconds and then check the time. If it is a possible block time check if their is a new block
    usleep(200000);
    time(&current_date_and_time);
    current_UTC_date_and_time = gmtime(&current_date_and_time);
    if (current_UTC_date_and_time->tm_min % 5 == 0)
    {
      // try for the next 5 seconds and if not then a new block is not going to be added to the network
      for (count = 0; count < 5; count++)
      {
        get_current_block_height(data,0);
        if (memcmp(data,current_block_height,strlen(current_block_height)) != 0)
        {      
          // replace the current_block_height variable
          memset(current_block_height,0,strlen(current_block_height));
          memcpy(current_block_height,data,strnlen(data,BUFFER_SIZE));

          // check if you found the previous block in the network
          if (check_found_block(0) == 2)
          {
            // get the block information of the previous block
            memset(block_reward,0,strnlen(block_reward,BUFFER_SIZE));
            memset(data3,0,strnlen(data3,BUFFER_SIZE));
            memset(data4,0,strnlen(data4,BUFFER_SIZE));
            if (get_previous_block_information(data4,block_reward,data3,0) == 0)
            {
              BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not get the previous block information.\nCould not check if you found the last block.\n\n");
            }
            // convert the block_reward to a number
            sscanf(block_reward, "%lld", &block_reward_number);
      
            // get the block height of the previous block
            memset(data,0,strnlen(data,BUFFER_SIZE));
            memset(block_height,0,strnlen(block_height,BUFFER_SIZE));
            if (get_current_block_height(data,0) == 0)
            {
              BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not get the previous block height.\nCould not check if you found the last block.\n\n");
            }
            // convert the block_count_total to a number
            sscanf(data, "%lld", &number);
            // update the block_count_total
            number--;
            // convert the number to a string
            sprintf(block_height,"%lld",number);

            // get the blocks count
            memset(data2,0,strlen(data2));
            if (read_document_field_from_collection(DATABASE_NAME,"statistics",DATA,"block_count",data2,0) == 0)
            {
              BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not update the database statistics.\n\n");
            }

            // add the blocks data
            memset(data,0,strlen(data));
            memcpy(data,"{\"block_height\":\"",17);
            memcpy(data+strlen(data),block_height,strnlen(block_height,BUFFER_SIZE));
            memcpy(data+strlen(data),"\",\"block_hash\":\"",16);
            memcpy(data+strlen(data),data4,strnlen(data4,BUFFER_SIZE));
            memcpy(data+strlen(data),"\",\"block_date_and_time\":\"",25);
            memcpy(data+strlen(data),data3,strnlen(data3,BUFFER_SIZE));
            memcpy(data+strlen(data),"\",\"block_reward\":\"",18);
            memcpy(data+strlen(data),block_reward,strnlen(block_reward,BUFFER_SIZE));
            memcpy(data+strlen(data),"\",\"block_count\":\"",17);
            memcpy(data+strlen(data),data2,strnlen(data2,BUFFER_SIZE));
            memcpy(data+strlen(data),"\"}",2);
            insert_document_into_collection_json(DATABASE_NAME,"blocks_found",data,0);
      
            // create the message
            memset(data,0,strlen(data));
            memcpy(data,"{\"public_address_voted_for\":\"",29);
            memcpy(data+29,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
            memcpy(data+127,"\"}",2); 

            // get the count of how many public addresses voted for the delegate
            for (count = 1; count <= 50; count++)
            { 
              memset(data2,0,strlen(data2));
              memcpy(data2,"reserve_proofs_",15);
              sprintf(data2+15,"%d",count);
              counter = count_documents_in_collection(DATABASE_NAME,data2,data,0);
              public_address_count += counter;
            }

            // error check
            if (public_address_count == 0)
            {
              continue;
            }

            // initialize the database_multiple_documents_fields struct 
            for (count = 0; count < public_address_count; count++)
            {
              for (counter = 0; counter < 4; counter++)
              {
                database_data.item[count][counter] = (char*)calloc(RESERVE_PROOFS_LENGTH,sizeof(char));
                database_data.value[count][counter] = (char*)calloc(RESERVE_PROOFS_LENGTH,sizeof(char));
              }
            }
            database_data.document_count = 0;
            database_data.database_fields_count = 0;

            // create the message
            memset(data3,0,strnlen(data3,BUFFER_SIZE));
            memcpy(data3,"\"public_address_voted_for\" : \"",30);
            memcpy(data3+30,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
            memcpy(data3+128,"\"",1);

            // get all of the reserve proofs for the public address
            for (count = 1; count <= 50; count++)
            { 
              memset(data2,0,strlen(data2));
              memcpy(data2,"reserve_proofs_",15);
              sprintf(data2+15,"%d",count);

              counter = count_documents_in_collection(DATABASE_NAME,data2,data3,0);
              if (counter > 0)
              {
                if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,data2,data3,&database_data,1,counter,0,"",0,0) == 0)
                {
                  pointer_reset_database_array;
                  continue;
                }
              }
            }

            // get the total votes
            total_votes = 0;
            for (count = 0; count < database_data.document_count; count++)
            {
              sscanf(database_data.value[count][2], "%lld", &number);
              total_votes += number;
            }

            // split the block reward with all of the public_addresses and the fee
            for (count = 0; count < database_data.document_count; count++)
            {
              // get the votes
              sscanf(database_data.value[count][2], "%lld", &number);
              block_reward_for_public_address = (long long int) (((number / total_votes) * block_reward_number)) | 0;
              // create the message
              memset(data2,0,strnlen(data2,BUFFER_SIZE));
              memcpy(data2,"{\"public_address\":\"",19);
              memcpy(data2+19,database_data.value[count][0],XCASH_WALLET_LENGTH);
              memcpy(data2+117,"\"}",2);
              memset(data3,0,strnlen(data3,BUFFER_SIZE));
              if (read_document_field_from_collection(DATABASE_NAME,"public_addresses",data2,"current_total",data3,0) == 0)
              {
                BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not split the block reward with all of the public address that voted for the delegate.\n\n");
              }
              sscanf(data3, "%lld", &number);
              number += block_reward_for_public_address;
              memset(data3,0,strnlen(data3,BUFFER_SIZE));
              sprintf(data3,"%lld",number);
              memset(data4,0,strnlen(data4,BUFFER_SIZE));
              memcpy(data4,"{\"current_total\":\"",18);
              memcpy(data4+18,data3,strnlen(data3,BUFFER_SIZE));
              memcpy(data4+18+strnlen(data3,BUFFER_SIZE),"\"}",2);
              if (update_document_from_collection(DATABASE_NAME,"public_addresses",data2,data4,0) == 0)
              {
                BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not split the block reward with all of the public address that voted for the delegate.\nCould not check if you found the last block.\n\n");
              } 
            }
            // update the statistics
            memset(data2,0,strlen(data2));
            if (read_document_field_from_collection(DATABASE_NAME,"statistics",DATA,"total_blocks_found",data2,0) == 0)
            {
              BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not update the database statistics.\n\n");
            }
            sscanf(data2, "%lld", &number);
            number++;      
            // create the message
            memset(data3,0,strlen(data3));
            memcpy(data3,"{\"total_blocks_found\":\"",23);
            sprintf(data3+23,"%lld",number);
            memcpy(data3+strlen(data3),"\"}",2);
            if (update_document_from_collection(DATABASE_NAME,"statistics",DATA,data3,0) == 0 && update_document_from_collection(DATABASE_NAME,"statistics",DATA,"{\"block_count\",\"0\"}",0) == 0)
            {
              BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not update the database statistics.\n\n");
            }  
            pointer_reset_database_array;
          }
          else
          {
            // update the statistics
            memset(data2,0,strlen(data2));
            if (read_document_field_from_collection(DATABASE_NAME,"statistics",DATA,"block_count",data2,0) == 0)
            {
              BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not update the database statistics.\n\n");
            }
            sscanf(data2, "%lld", &number);
            number++;      
            // create the message
            memset(data3,0,strlen(data3));
            memcpy(data3,"{\"block_count\":\"",16);
            sprintf(data3+16,"%lld",number);
            memcpy(data3+strlen(data3),"\"}",2);
            if (update_document_from_collection(DATABASE_NAME,"statistics",DATA,data3,0) == 0)
            {
              BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not update the database statistics.\n\n");
            }     
          }    
          // update the statistics
          memset(data2,0,strlen(data2));
          if (read_document_field_from_collection(DATABASE_NAME,"statistics",DATA,"total_blocks",data2,0) == 0)
          {
            BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not update the database statistics.\n\n");
          }
          sscanf(data2, "%lld", &number);
          number++;      
          // create the message
          memset(data3,0,strlen(data3));
          memcpy(data3,"{\"total_blocks\":\"",17);
          sprintf(data3+17,"%lld",number);
          memcpy(data3+strlen(data3),"\"}",2);
          if (update_document_from_collection(DATABASE_NAME,"statistics",DATA,data3,0) == 0)
          {
            BLOCK_HEIGHT_TIMER_THREAD_ERROR("Could not update the database statistics.\n\n");
          }         
          goto start;
        }
        sleep(1);
      }
    }
  }
  pointer_reset_all;
  pthread_exit(0);

  #undef DATA 
  #undef BLOCK_HEIGHT_TIMER_THREAD_ERROR
  #undef pointer_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: payment_timer_thread
Description: checks if it is time to send the payments
-----------------------------------------------------------------------------------------------------------
*/

void* payment_timer_thread()
{
  // Constants
  const bson_t* current_document;

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* current_total = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* total = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* inactivity_count = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* payment_tx_hash = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* payment_tx_key = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message;
  char* datacopy1;
  char* datacopy2;
  long long int minimum_amount_atomic_units = minimum_amount * 1000000;
  long long int number;
  long long int amount_of_payments;
  long long int updated_total;
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;  
  mongoc_collection_t* collection;
  mongoc_cursor_t* document_settings;
  bson_t* document = NULL;  

  // define macros
  #define DATA "{\"username\":\"XCASH\"}"
  #define DATABASE_COLLECTION "public_addresses"
  #define PAYMENT_TIMER_THREAD_ERROR(message) \
  fprintf(stderr,"\033[1;31mError:\nTime = %ld\n%s\033[0m\n\n",time(NULL),message); \
  bson_destroy(document); \
  mongoc_cursor_destroy(document_settings); \
  mongoc_collection_destroy(collection); \
  sleep(60); \
  goto start2;

  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(public_address); \
  public_address = NULL; \
  free(current_total); \
  current_total = NULL; \
  free(total); \
  total = NULL; \
  free(inactivity_count); \
  inactivity_count = NULL; \
  free(payment_tx_hash); \
  payment_tx_hash = NULL; \
  free(payment_tx_key); \
  payment_tx_key = NULL; 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL || public_address == NULL || current_total == NULL || total == NULL || inactivity_count == NULL || payment_tx_hash == NULL || payment_tx_key == NULL)
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
    if (public_address != NULL)
    {
      pointer_reset(public_address);
    }
    if (current_total != NULL)
    {
      pointer_reset(current_total);
    }
    if (total != NULL)
    {
      pointer_reset(total);
    } 
    if (inactivity_count != NULL)
    {
      pointer_reset(inactivity_count);
    }
    if (payment_tx_hash != NULL)
    {
      pointer_reset(payment_tx_hash);
    }
    if (payment_tx_key != NULL)
    {
      pointer_reset(payment_tx_key);
    }    
    color_print("Could not allocate the variables on the heap","red");
    exit(0);
  }

  for (;;)
  {
    start2:

    // reset the amount of payments
    amount_of_payments = 0;

    // check if it is UTC 00:00
    time(&current_date_and_time);
    current_UTC_date_and_time = gmtime(&current_date_and_time);
    if (current_UTC_date_and_time->tm_hour == 0 && current_UTC_date_and_time->tm_min == 0)
    {
      fprintf(stderr,"\033[1;32mIt is UTC 00:00\nSending the daily payments.\n\n");

      // set the collection
      collection = mongoc_client_get_collection(database_client, DATABASE_NAME, DATABASE_COLLECTION);

      document = bson_new();
      if (!document)
      {
        PAYMENT_TIMER_THREAD_ERROR("Could not create a document for the database.\nCould not send payments.");
      } 
      document_settings = mongoc_collection_find_with_opts(collection, document, NULL, NULL);
      while (mongoc_cursor_next(document_settings, &current_document))
      {
        message = bson_as_canonical_extended_json(current_document, NULL);
        memset(data,0,strnlen(data,BUFFER_SIZE));
        memcpy(data,message,strnlen(message,BUFFER_SIZE));
        bson_free(message); 

        // parse the document
        datacopy1 = data;
        datacopy1 = strstr(datacopy1,"public_address") + 17;
        datacopy2 = strstr(datacopy1,"\"");
        memcpy(public_address,datacopy1,strnlen(datacopy1,BUFFER_SIZE) - strnlen(datacopy2,BUFFER_SIZE));
        datacopy1 = strstr(datacopy1,"current_total") + 16;
        datacopy2 = strstr(datacopy1,"\"");
        memcpy(current_total,datacopy1,strnlen(datacopy1,BUFFER_SIZE) - strnlen(datacopy2,BUFFER_SIZE));
        datacopy1 = strstr(datacopy1,"total") + 8;
        datacopy2 = strstr(datacopy1,"\"");
        memcpy(total,datacopy1,strnlen(datacopy1,BUFFER_SIZE) - strnlen(datacopy2,BUFFER_SIZE));
        datacopy1 = strstr(datacopy1,"inactivity_count") + 19;
        datacopy2 = strstr(datacopy1,"\"");
        memcpy(inactivity_count,datacopy1,strnlen(datacopy1,BUFFER_SIZE) - strnlen(datacopy2,BUFFER_SIZE));

        const size_t PUBLIC_ADDRESS_LENGTH = strnlen(public_address,BUFFER_SIZE);
        const size_t CURRENT_TOTAL_LENGTH = strnlen(current_total,BUFFER_SIZE);
        const size_t TOTAL_LENGTH = strnlen(total,BUFFER_SIZE);
        const size_t INACTIVITY_COUNT_LENGTH = strnlen(inactivity_count,BUFFER_SIZE);

        // check if the current_total is over the minimum amount
        sscanf(current_total, "%lld", &number);
        if (number >= minimum_amount_atomic_units)
        {
          // send the payment
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memset(data3,0,strnlen(data3,BUFFER_SIZE));
          memset(payment_tx_hash,0,strnlen(payment_tx_hash,BUFFER_SIZE));
          memset(payment_tx_key,0,strnlen(payment_tx_key,BUFFER_SIZE));
          if (send_payment(public_address, current_total, payment_tx_hash, payment_tx_key, 0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not send a payment.\nCould not send payments.");
          }

          const size_t TX_HASH_LENGTH = strnlen(payment_tx_hash,BUFFER_SIZE);
          const size_t TX_KEY_LENGTH = strnlen(payment_tx_key,BUFFER_SIZE);

          // create the message
          memset(data,0,strnlen(data,BUFFER_SIZE));
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memcpy(data,"{\"public_address\":\"",19);
          memcpy(data+19,public_address,PUBLIC_ADDRESS_LENGTH);
          memcpy(data+19+PUBLIC_ADDRESS_LENGTH,"\"}",2);
          
          // reset the current total in the public_addresses collection
          memcpy(data2,"{\"current_total\":\"0\"}",21);
          if (update_document_from_collection(DATABASE_NAME,"public_addresses",data,data2,0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not reset the current_total for a payment in the database.\nCould not send payments.");
          } 

          // add the current total to the total in the public_addresses collection
          memset(data3,0,strnlen(data3,BUFFER_SIZE));
          sscanf(total, "%lld", &updated_total);
          updated_total += number;
          sprintf(data3,"%lld",updated_total);
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memcpy(data2,"{\"total\":\"",10);
          memcpy(data2+10,data3,strnlen(data3,BUFFER_SIZE));
          memcpy(data2+10+strnlen(data3,BUFFER_SIZE),"\"}",2);
          if (update_document_from_collection(DATABASE_NAME,"public_addresses",data,data2,0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not add the current_total to the total for a payment in the database.\nCould not send payments.");
          } 

          // get the current date and time
          sprintf(data3,"%lld",(long long int)time(NULL));
          const size_t TIME_LENGTH = strnlen(data3,BUFFER_SIZE);

          // add the payment to the public_addresses_payments in the collection
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memcpy(data2,"{\"public_address\":\"",19);
          memcpy(data2+19,public_address,PUBLIC_ADDRESS_LENGTH);
          memcpy(data2+19+PUBLIC_ADDRESS_LENGTH,"\",\"total\":\"",11);
          memcpy(data2+30+PUBLIC_ADDRESS_LENGTH,current_total,CURRENT_TOTAL_LENGTH);
          memcpy(data2+30+PUBLIC_ADDRESS_LENGTH+CURRENT_TOTAL_LENGTH,"\",\"date_and_time\":\"",19);
          memcpy(data2+49+PUBLIC_ADDRESS_LENGTH+CURRENT_TOTAL_LENGTH,data3,TIME_LENGTH);
          memcpy(data2+49+PUBLIC_ADDRESS_LENGTH+CURRENT_TOTAL_LENGTH+TIME_LENGTH,"\",\"tx_hash\":\"",13);
          memcpy(data2+62+PUBLIC_ADDRESS_LENGTH+CURRENT_TOTAL_LENGTH+TIME_LENGTH,payment_tx_hash,TX_HASH_LENGTH);
          memcpy(data2+62+PUBLIC_ADDRESS_LENGTH+CURRENT_TOTAL_LENGTH+TIME_LENGTH+TX_HASH_LENGTH,"\",\"tx_key\":\"",12);
          memcpy(data2+74+PUBLIC_ADDRESS_LENGTH+CURRENT_TOTAL_LENGTH+TIME_LENGTH+TX_HASH_LENGTH,payment_tx_key,TX_KEY_LENGTH);
          memcpy(data2+74+PUBLIC_ADDRESS_LENGTH+CURRENT_TOTAL_LENGTH+TIME_LENGTH+TX_HASH_LENGTH+TX_KEY_LENGTH,"\"}",2);
          if (insert_document_into_collection_json(DATABASE_NAME,"public_addresses_payments",data2,0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not add the payment to the database.\nCould not send payments.");
          }
          amount_of_payments++;
        }

        /*
        set the inactivity_count
        if the address has a 0 current_total before sending any payments the inactivity_count ++
        if any address has not a 0 current_total and has an inactivity_count > 0 then reset it to 0
        if any address has an inactivity_count of 30 and a current_total of 0 before sending a payment, remove this document from the collection
        */

        if (strncmp(current_total,"0",BUFFER_SIZE) == 0)
        {
          // add one to the inactivity_count
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memset(data3,0,strnlen(data3,BUFFER_SIZE));
          if (read_document_field_from_collection(DATABASE_NAME,"public_addresses",data,"inactivity_count",data3,0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not read the inactivity_count from the database.\nCould not send payments.");
          }
          // convert the total_xcash to a number
          sscanf(data3, "%lld", &number);
          number++;
          // update the amount_of_payments in the database
          memset(data3,0,strnlen(data3,BUFFER_SIZE));
          sprintf(data3,"%lld",number);
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memcpy(data2,"{\"inactivity_count\":\"",21);
          memcpy(data2+21,data,strnlen(data3,BUFFER_SIZE));
          memcpy(data2+21+strnlen(data3,BUFFER_SIZE),"\"}",2);
          if (update_document_from_collection(DATABASE_NAME,"public_addresses",data,data2,0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not update the inactivity_count for the database.\nCould not send payments.");
          }
        }



        if (strncmp(current_total,"0",BUFFER_SIZE) != 0)
        {
          // set the inactivity_count to 0
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memset(data3,0,strnlen(data3,BUFFER_SIZE));
          // update the amount_of_payments in the database
          memcpy(data2,"{\"inactivity_count\":\"0\"}",24);
          if (update_document_from_collection(DATABASE_NAME,"public_addresses",data,data2,0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not update the inactivity_count for the database.\nCould not send payments.");
          }
        }



        if (strncmp(current_total,"0",BUFFER_SIZE) == 0)
        {
          memset(data2,0,strnlen(data2,BUFFER_SIZE));
          memset(data3,0,strnlen(data3,BUFFER_SIZE));
          if (read_document_field_from_collection(DATABASE_NAME,"public_addresses",data,"inactivity_count",data3,0) == 0)
          {
            PAYMENT_TIMER_THREAD_ERROR("Could not read the inactivity_count for the database.\nCould not send payments.");
          }
          if (strncmp(data3,"30",BUFFER_SIZE) == 0)
          {
            // remove the document from the database
            if (delete_document_from_collection(DATABASE_NAME,"public_addresses",data,0) == 0)
            {
              PAYMENT_TIMER_THREAD_ERROR("Could not remove a document from the database.\nCould not send payments.");
            }
          }          
        }
      }
    }    
    sleep(60);
  }
  pointer_reset_all;
  pthread_exit(0);

  #undef DATA
  #undef DATABASE_COLLECTION
  #undef PAYMENT_TIMER_THREAD_ERROR
  #undef pointer_reset_all
}