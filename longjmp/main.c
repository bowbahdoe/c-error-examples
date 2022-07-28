#include <stdio.h>
#include <setjmp.h>

int parse_natural_base_10_number(const char* s, jmp_buf env) {
    int parsed = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            longjmp(env, 0);
        }
        else {
            parsed *= 10;
            parsed += s[i] - '0';
        }
    }

    return parsed;
}

int main() {
    const char* examples[] = { "10", "42", "foo", "32", "1f"};

    for (size_t i = 0; i < 6; i++) {
        const char* example = examples[i];

        jmp_buf env;
        if (setjmp(env) > 0) {
            printf("failed: %s\n", example);
        }
        else {
            int parsed = parse_natural_base_10_number(example, env);
            printf("parsed: %d\n", parsed);
        }

    }

    return 0;
}
