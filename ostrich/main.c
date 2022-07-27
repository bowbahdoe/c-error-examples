#include <stdio.h>

int parse_natural_base_10_number(const char* s) {
    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        parsed *= 10;
        parsed += s[i] - '0';
    }

    return parsed;
}

int main() {
    printf("Expecting garbage or crash on bad values\n");
    const char* examples[] = { "10", "foo", "42", "" };
    for (size_t i = 0; i < 4; i++) {
        const char* example = examples[i];
        int parsed = parse_natural_base_10_number(example);
        printf("parsed: %d\n", parsed);
    }

    return 0;
}
