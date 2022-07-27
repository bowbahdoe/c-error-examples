#include <stdio.h>

enum ParseNaturalNumberResultKind {
    PARSE_NATURAL_SUCCESS,
    PARSE_NATURAL_EMPTY_STRING,
    PARSE_NATURAL_BAD_CHARACTER
};

struct BadCharacter {
    size_t index;
    char c;
};

struct ParseNaturalNumberResult {
    enum ParseNaturalNumberResultKind kind;
    union {
        int success;
        struct BadCharacter bad_character;
    } data;
};

struct ParseNaturalNumberResult parse_natural_base_10_number(const char* s) {
    if (s[0] == '\0') {
        struct ParseNaturalNumberResult result = {
                .kind = PARSE_NATURAL_EMPTY_STRING
        };
        return result;
    }

    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            struct ParseNaturalNumberResult result = {
                    .kind = PARSE_NATURAL_BAD_CHARACTER,
                    .data = {
                            .bad_character = {
                                    .index = i,
                                    .c = s[i]
                            }
                    }
            };
            return result;
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    struct ParseNaturalNumberResult result = {
            .kind = PARSE_NATURAL_SUCCESS,
            .data = {
                    .success = parsed
            }
    };

    return result;
}

int main() {
    const char* examples[] = { "10", "foo", "42", "12a34" };
    for (size_t i = 0; i < 4; i++) {
        const char* example = examples[i];

        struct ParseNaturalNumberResult result = parse_natural_base_10_number(example);
        switch (result.kind) {
            case PARSE_NATURAL_SUCCESS:
                printf("worked: %d\n", result.data.success);
                break;
            case PARSE_NATURAL_EMPTY_STRING:
                printf("got empty string");
                break;
            case PARSE_NATURAL_BAD_CHARACTER:
                printf("failed: %s\n        ", example);
                for (size_t j = 0; j < result.data.bad_character.index; j++) {
                    printf(" ");
                }
                printf("^☹️ '%c' is not good\n", result.data.bad_character.c);
                break;
        }

    }

    return 0;
}
