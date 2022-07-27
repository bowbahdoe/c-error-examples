#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct ErrorOps {
    char* (*describe)(const void*);
    void (*free)(void*);
};

struct Error {
    struct ErrorOps ops;
    void* self;
};

char* error_describe(struct Error error) {
    return error.ops.describe(error.self);
}

void error_free(struct Error error) {
    if (error.ops.free != NULL) {
        error.ops.free(error.self);
    }
}

char* empty_string_describe(const void* self) {
    char* result;
    asprintf(&result, "Empty string is not good");
    return result;
}

const struct ErrorOps empty_string_error_ops = {
        .describe = empty_string_describe,
        .free = NULL
};

struct Error empty_string_error() {
    struct Error result = {
            .ops = empty_string_error_ops,
            .self = NULL
    };
    return result;
}

struct BadCharacterError {
    char* source;
    size_t index;
};

char* bad_character_describe(const void* self) {
    const struct BadCharacterError* this = self;
    char* result;
    asprintf(&result, "Bad character in %s at index %zu: '%c'", this->source, this->index, this->source[this->index]);
    return result;
}

void bad_character_free(void* self) {
    struct BadCharacterError* this = self;
    free(this->source);
    free(this);
}

const struct ErrorOps bad_character_error_ops = {
        .describe = bad_character_describe,
        .free = bad_character_free
};

struct Error bad_character_error(const char* source, size_t index) {
    struct BadCharacterError* error = malloc(sizeof (struct BadCharacterError));

    char* source_clone = calloc(strlen(source) + 1, sizeof (char));
    strcpy(source_clone, source);
    error->source = source_clone;

    error->index = index;

    struct Error result = {
            .ops = bad_character_error_ops,
            .self = error
    };
    return result;
}

struct ParseNaturalNumberResult {
    bool success;
    union {
        int success;
        struct Error error;
    } data;
};

struct ParseNaturalNumberResult parse_natural_base_10_number(const char* s) {
    if (s[0] == '\0') {
        struct ParseNaturalNumberResult result = {
                .success = false,
                .data = {
                        .error = empty_string_error()
                }
        };
        return result;
    }

    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            struct ParseNaturalNumberResult result = {
                    .success = false,
                    .data = {
                            .error = bad_character_error(s, i)
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
            .success = true,
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
        if (!result.success) {
            char* description = error_describe(result.data.error);
            printf("error: %s\n", description);
            free(description);
            error_free(result.data.error);
        }
        else {
            printf("success: %d\n", result.data.success);
        }
    }

    return 0;
}
