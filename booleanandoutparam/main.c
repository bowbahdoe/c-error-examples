#include <stdio.h>
#include <stdbool.h>

/**
 * Parses a string containing the digits 0-9 into a number.
 * Does not support negative
 * @param s A string that may or may not be a number.
 * @return A natural int, and a negative int if it failed failure.
 */
bool parse_natural_base_10_number(const char* s, int* out) {
    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    *out = parsed;
    return true;
}

int main() {
    const char* examples[] = { "10", "foo", "42" };
    for (size_t i = 0; i < 3; i++) {
        const char* example = examples[i];
        int parsed;
        bool success = parse_natural_base_10_number(example, &parsed);
        if (!success) {
            printf("failed: %s\n", example);
        }
        else {
            printf("worked: %d\n", parsed);
        }
    }

    return 0;
}
