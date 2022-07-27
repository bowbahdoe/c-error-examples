#include <stdio.h>

int parse_natural_base_10_number(const char* s) {
    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return -1;
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    return parsed;
}

int main() {
    const char* examples[] = { "10", "foo", "42" };
    for (size_t i = 0; i < 3; i++) {
        const char* example = examples[i];
        int parsed = parse_natural_base_10_number(example);
        if (parsed < 0) {
            printf("failed: %s\n", example);
        }
        else {
            printf("worked: %d\n", parsed);
        }
    }

    return 0;
}
