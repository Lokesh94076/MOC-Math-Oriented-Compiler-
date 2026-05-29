#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fuc {
    char function_name[128];
    char argument[128];
    char expression[512];
};

int generate(char *func_name, char *argu, char *exp){
    
    printf("function_name: %s\n", func_name);
    printf("argument_name: %s\n", argu);
    printf("expression_name: %s\n\n", exp);
    
    FILE *fptr;
    
    fptr = fopen("gen/gen.c", "a");
    if (fptr == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    fprintf(fptr, "double %s(%s){return %s;}\n", func_name, argu, exp);
    fclose(fptr);
    
    return 0;
}

int parse(int message_size, char *recieved_msg) {
    // clear the file from last attempt
    FILE *fptr;
    fptr = fopen("gen/gen.c", "w");
    fprintf(fptr, "");
    fclose(fptr);
    
    struct fuc functions = {0};
    const char *command = "gcc gen/gen.c -o gen/output --shared";
    bool passed_argument = false;
    bool passed_equal = false;
    int function_name_counter = 0;
    int arguement_counter = 0;
    int expression_counter = 0;
    
    for (int i = 0; i < message_size; i++) {
        // 1. Check for message boundaries or delimiters
        if (recieved_msg[i] == '\n' || recieved_msg[i] == '\0') {
            functions.function_name[function_name_counter] = '\0';
            functions.argument[arguement_counter] = '\0'; // Added this missing termination
            functions.expression[expression_counter] = '\0';

            generate(functions.function_name, functions.argument, functions.expression);
            // CRITICAL: Reset the struct AND all state tracking variables
            memset(&functions, 0, sizeof(functions));
            function_name_counter = 0;
            arguement_counter = 0;
            expression_counter = 0;
            passed_argument = false;
            passed_equal = false;
    
            continue; // Move smoothly to the next character
        }
    
        // 2. State transition checks
        if (recieved_msg[i] == '(') {
            passed_argument = true;
            continue;
        }
        if (recieved_msg[i] == ')') {
            passed_argument = false;
            continue;
        }
        if (recieved_msg[i] == '=') {
            passed_equal = true;
            continue; 
        }
    
        // 3. Populate fields based on current state flags
        if (!passed_equal) {
            if (!passed_argument) {
                if (function_name_counter < 127) {
                    functions.function_name[function_name_counter++] = recieved_msg[i];
                } else {
                    printf("Ran out of function name memory.\n");
                }
            } else {
                if (arguement_counter < 127) {
                    functions.argument[arguement_counter++] = recieved_msg[i];
                } else {
                    printf("Ran out of argument memory.\n");
                }
            }
        } else {
            // Fixed: Use expression_counter check here instead of arguement_counter
            if (expression_counter < 127) {
                functions.expression[expression_counter++] = recieved_msg[i];
            } else {
                printf("Ran out of expression memory.\n");
            }
        }
        if (i == message_size - 1){
            functions.function_name[function_name_counter] = '\0';
            functions.argument[arguement_counter] = '\0'; // Added this missing termination
            functions.expression[expression_counter] = '\0';

            generate(functions.function_name, functions.argument, functions.expression);
        }
        
    }
    system(command);
    return 0;
}
