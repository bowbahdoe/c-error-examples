#include <stdio.h>
#include <stdlib.h>

/**
 * Parses a string containing the digits 0-9 into a number.
 * Does not support negative
 * @param s A string that may or may not be a number.
 * @return A heap allocated number on success, NULL on failure.
 */
int* parse_natural_base_10_number(const char* s) {
    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return NULL;
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    int* result = malloc(sizeof (int));
    *result = parsed;
    return result;
}

int main() {
    const char* examples[] = { "10", "foo", "42" };
    for (size_t i = 0; i < 3; i++) {
        const char* example = examples[i];
        int* parsed = parse_natural_base_10_number(example);
        if (parsed == NULL) {
            printf("failed: %s\n", example);
        }
        else {
            printf("worked: %d\n", *parsed);
        }

        free(parsed);
    }

    return 0;
}
