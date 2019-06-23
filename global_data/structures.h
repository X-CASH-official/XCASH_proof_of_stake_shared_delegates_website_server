#ifndef STRUCTURES_H_   /* Include guard */
#define STRUCTURES_H_

/*
-----------------------------------------------------------------------------------------------------------
Global Structures
-----------------------------------------------------------------------------------------------------------
*/

// server
struct MHD_Daemon* server; 

// post request data
struct connection_info_struct {
    int connection_type;
    size_t count;
    char* item[100];
    char* value[100];
    struct MHD_PostProcessor *post_result_settings;
};

// network_data_nodes_list
  struct network_data_nodes_list {
    char* network_data_nodes_public_address[NETWORK_DATA_NODES_AMOUNT]; // The block verifiers public address
    char* network_data_nodes_IP_address[NETWORK_DATA_NODES_AMOUNT]; // The block verifiers IP address
};

// database struct
struct database_document_fields {
    size_t count;
    char* item[DATABASE_ARRAY_COUNT];
    char* value[DATABASE_ARRAY_COUNT];
};

struct database_multiple_documents_fields {
    size_t document_count;
    size_t database_fields_count;
    char* item[DATABASE_ARRAY_COUNT][DATABASE_ARRAY_COUNT];
    char* value[DATABASE_ARRAY_COUNT][DATABASE_ARRAY_COUNT];
};
#endif