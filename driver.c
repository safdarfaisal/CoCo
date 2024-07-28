#include ""
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *source_code = read_file(argv[1]);
    Lexer lexer = init_lexer(source_code);