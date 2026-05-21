#include <stdio.h>
#include <stdlib.h>
#include <listen.h>
#include <stdbool.h>

int main() {
    // Create vars
    char *received_message = NULL; // Start with 0 bytes allocated
    while (true){

        size_t message_size = 0;
        int bytes = start_server_listener(&received_message, &message_size);
        if (bytes >= 0 && received_message != NULL) {
            printf("Message received: %s\n", received_message);

            free(received_message); // Free recueved message
            received_message = NULL; // Reset for the next run
        } else {
            printf("Failed to receive data or connection closed.\n");
        }
    }
    return 0;
}
