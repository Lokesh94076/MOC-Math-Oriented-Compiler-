#include <stdio.h>
#include <stdlib.h>
#include <listen.h>
#include <stdbool.h>
#include <parser.h>
#include <dlfcn.h>

int main() {
    // Create vars
    char *received_message = NULL; // Start with 0 bytes allocated
    char *generated_code = NULL;
    while (true){
        size_t message_size = 0;
        int bytes = start_server_listener(&received_message, &message_size);
        if (bytes >= 0 && received_message != NULL) {
            //printf("Message received: %s\n", received_message);

            
            // Main logic
            parse(message_size, received_message); //parse and generate code
            
            void *lib = dlopen("gen/generated.so", RTLD_LAZY);
            
            if (!lib)
            {
                printf("dlopen failed: %s\n", dlerror());
                return 1;
            }
            double (*square)(double);
            square = (double (*)(double)) dlsym(lib, "square");
            if (!square)
            {
                printf("dlsym failed: %s\n", dlerror());
                dlclose(lib);
                return 1;
            }

            double result = square(5);
            printf("Result: %f\n", result);
            dlclose(lib);

            
            free(received_message); // Free received message memory
            generated_code = NULL;
            received_message = NULL; // Reset for the next run
        } else {
            printf("Failed to receive data or connection closed.\n");
        }
    }
    return 0;
}
