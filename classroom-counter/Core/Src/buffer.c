#include <stdint.h>
#include <string.h>

char *buffer_to_string(uint8_t *array, size_t size) {
    // char *string = malloc(size + 1);
    // if (string == NULL) {
    //     fprintf(stderr, "Memory allocation failed\n");
    //     exit(1);
    // }
    // memcpy(string, array, size);
    // string[size] = '\0';
    // return string;
    size_t result_length = (size * 3) + 1; //Add one for null charachter
    
    char* result = (char*)malloc(result_length * sizeof(char));
    result[0] = '\0';
    
    for (size_t i = 0; i < size; i++) {
        char temp[4]; // Buffer for a 3-digit number plus null terminator
        sprintf(temp, "%03u", array[i]); // Format as 3 digits with leading zeros
        strcat(result, temp);
    }
    
    return result;
}

void add_buffer(char ***array, size_t *capacity, uint8_t *buffer, size_t buffer_size) {
    char *new_scan = buffer_to_string(buffer, buffer_size);

    for (size_t i = 0; i < *capacity; i++) {
        if (strcmp((*array)[i], new_scan) == 0) {
            free(new_scan);
            return;
        }
    }

    char **temp = realloc(*array, sizeof(char *) * (*capacity + 1));
    if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_scan);
        exit(1);
    }
    *array = temp;

    (*array)[*capacity] = new_scan;
    (*capacity)++;
}