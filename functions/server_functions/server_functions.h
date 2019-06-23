#ifndef SERVER_FUNCTIONS_H_   /* Include guard */
#define SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

ssize_t read_file_server(void *cls, size_t position , char* data, size_t file_size);
void callback_reset(void *cls);
int post_result(void *connection_info_cls, enum MHD_ValueKind post_type, const char* item,const char* file_name, const char* content_type,const char* transfer_encoding, const char* data, uint64_t position, size_t size);
void post_request_completed(void* cls, struct MHD_Connection* connection,void** settings, enum MHD_RequestTerminationCode request);
int get_statistics(char* result);
int get_blocks_found(char* result);
int get_public_address_information(struct MHD_Connection* connection, char* result);
int get_voters_list(char* result);
int get_public_address_payment_information(struct MHD_Connection* connection, char* result);
int create_server(void* cls,struct MHD_Connection* connection,const char* url,const char* method,const char* version,const char* upload_data,size_t* upload_data_size,void** settings);
#endif