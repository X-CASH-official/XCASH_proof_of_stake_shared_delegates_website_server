CPP=gcc
CFLAGS= -Ifunctions/database_functions/ -Ifunctions/global_data/ -Ifunctions/network_daemon_functions/ -Ifunctions/network_functions/ -Ifunctions/network_wallet_functions/ -Ifunctions/server_functions/ -Ifunctions/string_functions/ -Ifunctions/thread_functions/ -Iglobal_data -Itests -Itests/analysing_code_test/ -Itests/database_functions_test/ -Itests/global_data/ -Itests/network_functions_test/ -Itests/reset_variables_allocated_on_the_heap_test/ -Itests/string_functions_test/ -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0

LDFLAGS= -lmongoc-1.0 -lbson-1.0 -lmicrohttpd -lpthread

all: xcash_proof_of_stake_website_server

xcash_proof_of_stake_website_server:
	gcc -o xcash_proof_of_stake_website_server xcash_proof_of_stake_website_server.c functions/database_functions/database_functions.c functions/network_daemon_functions/network_daemon_functions.c functions/network_functions/network_functions.c functions/network_wallet_functions/network_wallet_functions.c functions/server_functions/server_functions.c functions/string_functions/string_functions.c functions/thread_functions/thread_functions.c tests/analysing_code_test/analysing_code_test.c tests/database_functions_test/database_functions_test.c tests/network_functions_test/network_functions_test.c tests/reset_variables_allocated_on_the_heap_test/reset_variables_allocated_on_the_heap_test.c tests/string_functions_test/string_functions_test.c tests/xcash_proof_of_stake_website_server_test.c $(CFLAGS) $(LDFLAGS)

clean:
	rm xcash_proof_of_stake_website_server