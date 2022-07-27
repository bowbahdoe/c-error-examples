#include <stdio.h>

enum ParseNaturalNumberResult {
    PARSE_NATURAL_SUCCESS,
    PARSE_NATURAL_EMPTY_STRING,
    PARSE_NATURAL_BAD_CHARACTER
};

enum ParseNaturalNumberResult parse_natural_base_10_number(const char* s, int* out) {
    if (s[0] == '\0') {
        return PARSE_NATURAL_EMPTY_STRING;
    }

    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return PARSE_NATURAL_BAD_CHARACTER;
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    *out = parsed;
    return PARSE_NATURAL_SUCCESS;
}

int main() {
    const char* examples[] = { "10", "foo", "42", "" };
    for (size_t i = 0; i < 4; i++) {
        const char* example = examples[i];
        int parsed;
        switch (parse_natural_base_10_number(example, &parsed)) {
            case PARSE_NATURAL_SUCCESS:
                printf("worked: %d\n", parsed);
                break;
            case PARSE_NATURAL_EMPTY_STRING:
                printf("failed because empty string\n");
                break;
            case PARSE_NATURAL_BAD_CHARACTER:
                printf("failed because bad char: %s\n", example);
                break;
        }
    }

    return 0;
}
