#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "database_functions.h"
#include "thread_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "database_functions_test.h"


/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: database_test
Description: Test the database functions
Return: The number of passed database test
-----------------------------------------------------------------------------------------------------------
*/

int database_test()
{   
  // Variables  
  int count;
  int counter;

  // define macros
  #define DATABASE_TEST 11
  #define DATA_COUNT 5
  #define DATABASE_COLLECTION "XCASH_PROOF_OF_STAKE_TEST_DATA"
  #define MESSAGE "{\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"}"
  #define MESSAGE_SETTINGS "{\"message_settings\": \"XCASH_PROOF_OF_STAKE_DATA\"}"
  #define DATABASE_COLLECTION_STATISTICS_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}"
    
  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mdatabase test - Total test: %d\033[0m\n",DATABASE_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test
  
  // insert a document into the collection using json data
  if (insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for inserting a document into a collection using json data","red");
  }
  else
  {
    color_print("PASSED! Test for inserting a document into a collection using json data","green");
    count_test++;
  }  

  // update a document in the collection
  if (update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,MESSAGE_SETTINGS,0) == 0)
  {
    color_print("FAILED! Test for updating a document in a collection","red");
  }
  else
  {
    color_print("PASSED! Test for updating a document in a collection","green");
    count_test++;
  }

  // read a document in the collection
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (read_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,data_test,0) == 1)
  {
    if (strstr(data_test,"XCASH_PROOF_OF_STAKE_DATA") != NULL)
    {
      color_print("PASSED! Test for reading a document from a collection","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection","red");
  }

  // read a document in the collection and parse a field
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  if (read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,"message_settings",data_test,0) == 1)
  {
    if (strncmp(data_test,"XCASH_PROOF_OF_STAKE_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading a document from a collection and parsing a field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection and parsing a field","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection and parsing a field","red");
  }

  // read a document in the collection and parse all fields
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  struct database_document_fields database_data;

  // initialize the database_document_fields struct 
  for (count = 0; count < 14; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    
    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      color_print("Could not allocate the variables on the heap","red");
      exit(0);
    }
  }
  database_data.count = 0;

  if (read_document_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"{\"username\":\"XCASH\"}",&database_data,0) == 1)
  {
    if (strncmp(database_data.item[0],"username",BUFFER_SIZE) == 0 && strncmp(database_data.value[0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_data.value[4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[6],"15",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[7],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[8],"most_VRF_node_public_and_private_key_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[8],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[9],"most_VRF_node_random_data_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_data.value[9],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[10],"most_VRF_node_random_data_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[10],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[11],"total_XCASH_proof_of_stake_rounds",BUFFER_SIZE) == 0 && strncmp(database_data.value[11],"15",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[12],"total_coins_in_proof_of_stake",BUFFER_SIZE) == 0 && strncmp(database_data.value[12],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_data.item[13],"total_circulating_supply_percentage_in_proof_of_stake",BUFFER_SIZE) == 0 && strncmp(database_data.value[13],"10",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading a document from a collection and parsing all fields","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading a document from a collection and parsing all fields","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading a document from a collection and parsing all fields","red");
  }

  for (count = 0; count < 14; count++)
  {
    pointer_reset(database_data.item[count]);
    pointer_reset(database_data.value[count]);
  }

  // read multiple documents in the collection and parse all fields
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 14; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
   
      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        color_print("Could not allocate the variables on the heap","red");
        exit(0);
      }
    }
  }
  database_multiple_documents_fields.document_count = 0;
  database_multiple_documents_fields.database_fields_count = 0;

  if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,2,0) == 1)
  {
    if (strncmp(database_multiple_documents_fields.item[0][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][6],"15",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][7],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][8],"most_VRF_node_public_and_private_key_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][8],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][9],"most_VRF_node_random_data_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][9],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][10],"most_VRF_node_random_data_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][10],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][11],"total_XCASH_proof_of_stake_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][11],"15",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][12],"total_coins_in_proof_of_stake",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][12],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[0][13],"total_circulating_supply_percentage_in_proof_of_stake",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[0][13],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][0],"username",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][0],"XCASH",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][1],"most_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][2],"most_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][2],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][3],"best_block_verifier_online_percentage_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][3],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][4],"best_block_verifier_online_percentage",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][4],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][5],"most_block_producer_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][5],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][6],"most_block_producer_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][6],"15",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][7],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][8],"most_VRF_node_public_and_private_key_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][8],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][9],"most_VRF_node_random_data_total_rounds_delegate_name",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][9],"DELEGATE_NAME",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][10],"most_VRF_node_random_data_total_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][10],"10",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][11],"total_XCASH_proof_of_stake_rounds",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][11],"15",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][12],"total_coins_in_proof_of_stake",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][12],"5",BUFFER_SIZE) == 0 &&
        strncmp(database_multiple_documents_fields.item[1][13],"total_circulating_supply_percentage_in_proof_of_stake",BUFFER_SIZE) == 0 && strncmp(database_multiple_documents_fields.value[1][13],"10",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for reading multiple documents from a collection and parsing all fields","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for reading multiple documents from a collection and parsing all fields","red");
    }    
  }
  else
  {
    color_print("FAILED! Test for reading multiple documents from a collection and parsing all fields","red");
  }

  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 14; counter++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][counter]);
      pointer_reset(database_multiple_documents_fields.value[count][counter]);
    }
  }

  // update all document in the collection
  if (update_all_documents_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for updating all documents in a collection","red");
  }
  else
  {
    color_print("PASSED! Test for updating all documents in a collection","green");
    count_test++;
  }

  // count how many documents have "message_settings":"XCASH_PROOF_OF_STAKE_TEST_DATA" in the collection
  int count1 = count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0);
  int count2 = count_all_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,0);
  if ((count1 == count2) && (count1 != -1 || count2 != -1))
  {
    color_print("PASSED! Test for counting documents in a collection that match a specific field name and field","green");
    color_print("PASSED! Test for counting all documents in a collection","green");
    count_test+= 2;   
  }
  else
  {
     color_print("FAILED! Test for counting documents in a collection that match a specific field name and field","red");
     color_print("FAILED! Test for counting all documents in a collection","red");
  }

  // delete a document from the collection
  if (delete_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0) == 0)
  {
    color_print("FAILED! Test for deleting a document from a collection","red");
  }
  else
  {
    color_print("PASSED! Test for deleting a document from a collection","green");
    count_test++;
  }

  // delete a collection from the database
  if (delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    color_print("FAILED! Test for deleting a collection from a database","red");
  }
  else
  {
    color_print("PASSED! Test for deleting a collection from a database","green");
    count_test++;
  }

  // write the end test message
  if (count_test == DATABASE_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mdatabase test - Passed test: %d, Failed test: 0\033[0m\n",DATABASE_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mdatabase test - Passed test: %d, Failed test: %d\033[0m\n",count_test,DATABASE_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }   
  return count_test;

  #undef DATABASE_TEST  
  #undef DATA_COUNT
  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef MESSAGE_SETTINGS
  #undef DATABASE_COLLECTION_STATISTICS_DATA
}