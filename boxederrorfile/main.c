#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

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


struct FileOperationError {
    int error_number;
};

char* file_operation_error_describe(const void* self) {
    const struct FileOperationError* this = self;
    char* result;
    asprintf(&result, "%s", strerror(this->error_number));
    return result;
}

void file_operation_error_free(void* self) {
    free(self);
}

const struct ErrorOps file_operation_error_ops = {
        .describe = file_operation_error_describe,
        .free = file_operation_error_free
};

struct Error file_operation_error(int error_number) {
    struct FileOperationError* file_operation_error = malloc(sizeof (struct FileOperationError));
    file_operation_error->error_number = error_number;

    struct Error result = {
            .ops = file_operation_error_ops,
            .self = file_operation_error
    };
    return result;
}

struct ReadNumberFromFileResult {
    bool success;
    union {
        int success;
        struct Error error;
    } data;
};

struct ReadNumberFromFileResult read_number_from_file(const char* path) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        struct ReadNumberFromFileResult result = {
                .success = false,
                .data = {
                        .error = file_operation_error(errno)
                }
        };
        errno = 0;
        fclose(fp);
        return result;
    }

    // Max positive int is only 10 characters big in base 10
    char first_line[12];
    fgets(first_line, sizeof (first_line), fp);

    if (ferror(fp)) {
        struct ReadNumberFromFileResult result = {
                .success = false,
                .data = {
                        .error = file_operation_error(errno)
                }
        };
        errno = 0;
        fclose(fp);
        return result;
    }

    struct ParseNaturalNumberResult parse_result = parse_natural_base_10_number(first_line);
    if (!parse_result.success) {
        struct ReadNumberFromFileResult result = {
                .success = false,
                .data = {
                        .error = parse_result.data.error
                }
        };
        fclose(fp);
        return result;
    }

    struct ReadNumberFromFileResult result = {
            .success = true,
            .data = {
                    .success = parse_result.data.success
            }
    };

    fclose(fp);
    return result;
}

int main() {
    const char* examples[] = { "../ex1", "../ex2", "../ex3" };
    for (size_t i = 0; i < 3; i++) {
        const char* example_file = examples[i];

        struct ReadNumberFromFileResult result = read_number_from_file(example_file);
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
