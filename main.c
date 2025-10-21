/**
 * @file      : main.c
 * @brief     : Implements the main user interface for the vector calculator program.
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/27/25
 * Course     : CPE 2600
 * Assignment : Lab 7
 * Section    : 112
 * 
 * Algorithm:
 * 1. Check for command-line arguments (-h for help).
 * 2. Initialize the vector store.
 * 3. Enter a continuous loop prompting the user for input.
 * 4. Use fgets() to read a full input line and trim whitespace.
 * 5. Execute recognized commands:
 *      - 'quit'  → Exit the program.
 *      - 'clear' → Remove all stored vectors.
 *      - 'list'  → Display all stored vectors.
 *      - 'save <file>'  → Save all stored vectors to a csv file.
 *      - 'load <file>'  → Load all vectors within csv file to be stored.
 * 6. If input contains '=' → process as a vector assignment.
 * 7. If input contains '+', '-', '*', or 'x' → process as an operation.
 * 8. Otherwise, treat input as a vector name and display its contents.
 * 9. Continue until the user types 'quit'.
 */

#include "vector.h"
#include "util.h"
#include "io.h"
#include <stdbool.h> // Needed to use the bool type, and true/false values
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* ===========================================================
 *                Local Constant Definitions
 * =========================================================== */
#define MAX_INPUT_LEN        100
#define MAX_TOKEN_LEN_LONG   64
#define MAX_TOKEN_LEN_MED    32
#define MAX_TOKEN_LEN_SHORT  10

/* ===========================================================
 *               Forward Function Declarations
 * =========================================================== */
void handle_assignment(VectorStore *store, char *input);
vector handle_operation(VectorStore *store, char *input);
void handle_display(VectorStore *store, char *input);

/* ===========================================================
 *                   Function Definitions
 * =========================================================== */

 /**
 * @brief Parses an input string to perform a vector operation.
 *
 * Handles vector addition (+), subtraction (-), dot product (*),
 * cross product (x), and scalar multiplication. The result can
 * be printed as 'ans' or assigned to a new vector variable
 * (e.g., "c = a + b").
 *
 * @param store Pointer to the VectorStore containing source vectors.
 * @param input The user-provided string (e.g., "a + b").
 * @return The resulting vector from the operation (add, sub, cross, scalar).
 * @return A null_vector if the operation was a dot product (which
 * prints to console) or if an error occurred.
 */
vector handle_operation(VectorStore *store, char *input)
{
    vector null_vector = {"", 0, 0, 0};
    vector result;
    vector *v1; 
    vector *v2;
    char left[MAX_TOKEN_LEN_SHORT];
    char right[MAX_TOKEN_LEN_SHORT];
    char result_name[MAX_TOKEN_LEN_SHORT];
    char op;
    float scalar;
    int assign = 0;

    if (sscanf(input, "%s = %s %c %s", result_name, left, &op, right) == 4) {
        assign = 1;
    } else if (sscanf(input, "%s %c %s", left, &op, right) != 3) {
        printf("Invalid operation format.\n");
        return null_vector;
    }

    if (isdigit(left[0]) || left[0] == '-' || left[0] == '+') {
        scalar = atof(left);
        v2 = find_vector(store, right);
        if (!v2) {
            printf("Vector '%s' not found.\n", right);
            return null_vector;
        }
        result.x = v2->x * scalar;
        result.y = v2->y * scalar;
        result.z = v2->z * scalar;
    }
    else if (isdigit(right[0]) || right[0] == '-' || right[0] == '+') {
        v1 = find_vector(store, left);
        if (!v1) {
            printf("Vector '%s' not found.\n", left);
            return null_vector;
        }
        scalar = atof(right);
        result.x = v1->x * scalar;
        result.y = v1->y * scalar;
        result.z = v1->z * scalar;
    }
    else if (op == '*') {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            printf("One or both vectors not found.\n");
            return null_vector;
        }
        float dot = dot_prod(*v1, *v2);
        printf("ans = %.2f\n", dot);
        return null_vector;
    }
    else if (op == 'x' || op == 'X') {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            printf("One or both vectors not found.\n");
            return null_vector;
        }
        result = cross_prod(*v1, *v2);
    }
    else {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            printf("One or both vectors not found.\n");
            return null_vector;
        }
        if (op == '+'){
            result = add(*v1, *v2);
        } else if (op == '-'){
            result = sub(*v1, *v2);
        } else {
            printf("Unsupported operator '%c'\n", op);
            return null_vector;
        }
    }

    if (assign) {
        strcpy(result.name, result_name);
        add_vector(store, result);
    } else {
        strcpy(result.name, "ans");
    }

    printf("%s = %.2f  %.2f  %.2f\n", result.name, result.x, result.y, result.z);
    return result;
}

/**
 * @brief Parses an assignment string to create or update a vector.
 *
 * This function handles two types of assignment:
 * 1. Direct assignment from values (e.g., "a = 1 2 3").
 * 2. Assignment from an operation (e.g., "c = a + b").
 *
 * @param store Pointer to the VectorStore to add the vector to.
 * @param input The user-provided assignment string (e.g., "a = 1 2 3").
 */
void handle_assignment(VectorStore *store, char *input)
{
    char left[MAX_TOKEN_LEN_MED];
    char right[MAX_TOKEN_LEN_LONG];
    float x;
    float y;
    float z;

    if (sscanf(input, "%31s = %63[^\n]", left, right) != 2) {
        printf("Invalid assignment format. Use: a = 1 2 3\n");
        return;
    }

    trim(left);
    trim(right);

    if (sscanf(right, "%f %f %f", &x, &y, &z) == 3) {
        vector v = {.x = x, .y = y, .z = z};
        strcpy(v.name, left);
        add_vector(store, v);
        printf("%s = %.2f  %.2f  %.2f\n", v.name, v.x, v.y, v.z);
        return;
    }

    vector result = handle_operation(store, right);
    strcpy(result.name, left);
    add_vector(store, result);
    printf("%s = %.2f  %.2f  %.2f\n", result.name, result.x, result.y, result.z);
}

/**
 * @brief Finds and displays a single vector from the store.
 *
 * Treats the input string as a vector name, searches for it in
 * the store, and prints its contents (e.g., "a = 1.00 2.00 3.00").
 * If the vector is not found, it prints an error message.
 *
 * @param store Pointer to the VectorStore to search.
 * @param input The name of the vector to find and display.
 */
void handle_display(VectorStore *store, char *input)
{
    trim(input);
    vector *v = find_vector(store, input);
    if (v == NULL) {
        printf("Vector '%s' not found.\n", input);
    } else {
        printf("%s = %.2f  %.2f  %.2f\n", v->name, v->x, v->y, v->z);
    }
}

/**
 * @brief Main entry point for the vector calculator.
 *
 * Initializes the vector store, checks for command-line arguments
 * (like -h for help), and then enters the main interactive
 * read-process-print loop until the user types 'quit'.
 *
 * @param argc The count of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return 0 on successful program termination (e.g., 'quit' or '-h').
 * @return 1 if an invalid command-line option is provided.
 */
int main(int argc, char *argv[])
{
    VectorStore store;
    init_store(&store);  

    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            printf("\n=== Vector Calculator Help ===\n");
            printf("Usage: ./vectorcalc [OPTION]\n\n");
            printf("Options:\n");
            printf("  -h           Display this help message\n\n");
            printf("Interactive Commands:\n");
            printf("  name = x y z         Create or replace a vector (e.g., a = 1 2 3)\n");
            printf("  list                 List all stored vectors\n");
            printf("  clear                Remove all stored vectors\n");
            printf("  save <file>          Ability to save to existing or new file\n");
            printf("  load <file>          Need to load from an existing file\n");
            printf("  name                 Display a single vector (e.g., a)\n");
            printf("  a + b, a - b         Vector addition and subtraction\n");
            printf("  a * b                Dot product (scalar result)\n");
            printf("  a x b                Cross product\n");
            printf("  2 * a or a * 2       Scalar multiplication\n");
            printf("  quit                 Exit the program\n");
            printf("\nExample Session:\n");
            printf("  vectorcalc> a = 1 2 3\n");
            printf("  vectorcalc> b = 4 5 6\n");
            printf("  vectorcalc> c = a x b\n");
            printf("  vectorcalc> list\n");
            printf("  vectorcalc> quit\n\n");
            free_store(&store);
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[1]);
            printf("Use './vectorcalc -h' for help.\n");
            free_store(&store);
            return 1;
        }
    }

    char input[MAX_INPUT_LEN];
    printf("vectorcalc> ");

    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';
        trim(input);

        if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "clear") == 0) {
            clear_vectors(&store);
        } else if (strcmp(input, "list") == 0) {
            list_vectors(&store);
        } else if (strcmp(input, "save") == 0) {
            // Catches the user typing just "save"
            printf("Error: Please provide a filename.\n");
            printf("Usage: save <filename.csv>\n");
        } else if (strncmp(input, "save ", 5) == 0) {
            char* filename = input + 5; 
            trim(filename);

            if (strlen(filename) == 0) {
                printf("Error: Please provide a filename.\n");
                printf("Usage: save <filename.csv>\n");
            } else {
                // save_vectors returns bool, but we can just call it
                if (save_vectors(&store, filename)) {
                    printf("Vectors have been saved to %s.\n", filename);
                } else {
                    // Error message was already printed inside save_vectors
                    printf("Failed to save vectors to %s.\n", filename);
                }
            }
        // --- LOAD BLOCK ---
        } else if (strcmp(input, "load") == 0) {
            // Catches the user typing just "load"
            printf("Error: Please provide a filename.\n");
            printf("Usage: load <filename.csv>\n");
        } else if (strncmp(input, "load ", 5) == 0) {
            char* filename = input + 5;
            trim(filename);

            if (strlen(filename) == 0) {
                printf("Error: Please provide a filename.\n");
                printf("Usage: load <filename.csv>\n");
            } else {
                // *** THIS IS THE KEY PART ***
                // Check the boolean return value from load_vectors
                if (load_vectors(&store, filename)) {
                    printf("Vectors have been loaded from %s.\n", filename);
                } else {
                    // Error message was already printed inside load_vectors
                    printf("Failed to load vectors from %s.\n", filename);
                }
            }
        } else if (strchr(input, '=') != NULL) {
            handle_assignment(&store, input);
        } else if (strchr(input, '+') || strchr(input, '-') ||
                   strchr(input, '*') || strchr(input, 'x') || strchr(input, 'X')) {
            handle_operation(&store, input);
        } else {
            handle_display(&store, input);
        }

        printf("vectorcalc> ");
    }

    printf("Goodbye!\n");
    free_store(&store);  
    return 0;
}
