#include <stdio.h>
#include <stdbool.h>

bool parse_natural_base_10_number(const char* s, int* out_value, size_t* out_bad_index) {
    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            *out_bad_index = i;
            return false;
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    *out_value = parsed;
    return true;
}

int main() {
    const char* examples[] = { "10", "foo", "42", "12a34" };
    for (size_t i = 0; i < 4; i++) {
        const char* example = examples[i];

        int parsed;
        size_t bad_index;
        bool success = parse_natural_base_10_number(example, &parsed, &bad_index);
        if (!success) {
            printf("failed: %s\n        ", example);
            for (size_t j = 0; j < bad_index; j++) {
                printf(" ");
            }
            printf("^☹️\n");
        }
        else {
            printf("worked: %d\n", parsed);
        }
    }

    return 0;
}
