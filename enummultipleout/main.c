#include <stdio.h>
#include <string.h>

enum ParseNaturalNumberResult {
    PARSE_NATURAL_SUCCESS,
    PARSE_NATURAL_EMPTY_STRING,
    PARSE_NATURAL_BAD_CHARACTER,
    PARSE_NUMBER_TOO_BIG
};

struct BadCharacterInfo {
    size_t index;
};

struct TooBigInfo {
    size_t remaining_characters;
};

enum ParseNaturalNumberResult parse_natural_base_10_number(
        const char* s,
        int* out_value,
        struct BadCharacterInfo* bad_character_info,
        struct TooBigInfo* too_big_info
) {
    if (s[0] == '\0') {
        return PARSE_NATURAL_EMPTY_STRING;
    }

    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            bad_character_info->index = i;
            return PARSE_NATURAL_BAD_CHARACTER;
        }
        else {
            int digit = s[i] - '0';
            int new_parsed = (parsed * 10) + digit;
            if ((new_parsed - digit) / 10 != parsed) {
                too_big_info->remaining_characters = strlen(s) - i;
                return PARSE_NUMBER_TOO_BIG;
            }
            else {
                parsed = new_parsed;
            }
        }
    }

    *out_value = parsed;
    return PARSE_NATURAL_SUCCESS;
}

int main() {
    const char* examples[] = { "10", "foo", "42", "", "99999999999999" };
    for (size_t i = 0; i < 5; i++) {
        const char* example = examples[i];
        int parsed;
        struct BadCharacterInfo bad_character_info;
        struct TooBigInfo too_big_info;

        switch (parse_natural_base_10_number(example, &parsed, &bad_character_info, &too_big_info)) {
            case PARSE_NATURAL_SUCCESS:
                printf("worked: %d\n", parsed);
                break;
            case PARSE_NATURAL_EMPTY_STRING:
                printf("failed because empty string\n");
                break;
            case PARSE_NATURAL_BAD_CHARACTER:
                printf("failed because bad char at index %zu: %s\n", bad_character_info.index, example);
                break;
            case PARSE_NUMBER_TOO_BIG:
                printf("number was too big. had %zu digits left: %s\n", too_big_info.remaining_characters, example);
                break;
        }
    }

    return 0;
}
