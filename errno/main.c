#include <stdio.h>
#include <stdbool.h>

_Thread_local static bool parse_number_error = false;

int parse_natural_base_10_number(const char* s) {
    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            parse_number_error = true;
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    return parsed;
}

int main() {
    const char* examples[] = { "10", "42", "foo" };
    for (size_t i = 0; i < 3; i++) {
        const char* example = examples[i];
        int parsed = parse_natural_base_10_number(example);
        if (parse_number_error) {
            parse_number_error = false;
            printf("error: %s\n", example);
        }
        else {
            printf("parsed: %d\n", parsed);
        }
    }

    return 0;
}
