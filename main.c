/**
 * @file      : main.c
 * @brief     : Implements the main user interface for the vector calculator program.
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/10/25
 * Course     : CPE 2600
 * Assignment : Lab 5
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
 * 6. If input contains '=' → process as a vector assignment.
 * 7. If input contains '+', '-', '*', or 'x' → process as an operation.
 * 8. Otherwise, treat input as a vector name and display its contents.
 * 9. Continue until the user types 'quit'.
 */

#include "vector.h"
#include "util.h"
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
 * @brief Parses and executes a mathematical vector operation from an input string.
 * 
 * This function supports scalar multiplication, vector addition, subtraction,
 * dot products, and cross products. It also supports assignment operations such
 * as `c = a x b`. The function retrieves existing vectors from the store and 
 * performs the requested operation, printing results to the console.
 * 
 * @param store Pointer to the vector storage structure.
 * @param input The user-entered string containing the operation (e.g., "a + b").
 * @return The resulting vector, or an empty vector if an error occurs.
 */
vector handle_operation(VectorStore *store, char *input)
{
    vector null_vector = {"", 0, 0, 0};
    vector result;
    vector *v1, *v2;
    char left[MAX_TOKEN_LEN_SHORT], right[MAX_TOKEN_LEN_SHORT], result_name[MAX_TOKEN_LEN_SHORT];
    char op;
    float scalar;
    int assign = 0;

    // Detect assignment (e.g., "c = a + b")
    if (sscanf(input, "%s = %s %c %s", result_name, left, &op, right) == 4)
        assign = 1;
    else if (sscanf(input, "%s %c %s", left, &op, right) != 3) {
        printf("Invalid operation format.\n");
        return null_vector;
    }

    // Handle scalar * vector
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
    // Handle vector * scalar
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
    // Handle dot product
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
    // Handle cross product
    else if (op == 'x' || op == 'X') {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            printf("One or both vectors not found.\n");
            return null_vector;
        }
        result = cross_prod(*v1, *v2);
    }
    // Handle addition and subtraction
    else {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            printf("One or both vectors not found.\n");
            return null_vector;
        }
        if (op == '+') {
            result = add(*v1, *v2);
        }
        else if (op == '-') {
            result = sub(*v1, *v2);
        }
        else {
            printf("Unsupported operator '%c'\n", op);
            return null_vector;
        }
    }

    // Handle assignment result
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
 * @brief Handles vector creation or reassignment from an input string.
 * 
 * This function interprets commands of the form `a = 1 2 3` to define a vector,
 * or `a = b + c` to perform an operation and store the result under a name.
 * 
 * @param store Pointer to the vector storage structure.
 * @param input The user-provided input line to parse and execute.
 */
void handle_assignment(VectorStore *store, char *input)
{
    char left[MAX_TOKEN_LEN_MED], right[MAX_TOKEN_LEN_LONG];
    float x, y, z;

    if (sscanf(input, "%31s = %63[^\n]", left, right) != 2) {
        printf("Invalid assignment format. Use: a = 1 2 3\n");
        return;
    }

    trim(left);
    trim(right);

    // Direct numeric vector assignment
    if (sscanf(right, "%f %f %f", &x, &y, &z) == 3) {
        vector v = {.x = x, .y = y, .z = z};
        strcpy(v.name, left);
        add_vector(store, v);
        printf("%s = %.2f  %.2f  %.2f\n", v.name, v.x, v.y, v.z);
        return;
    }

    // Expression-based assignment (e.g., a = b + c)
    vector result = handle_operation(store, right);
    strcpy(result.name, left);
    add_vector(store, result);
    printf("%s = %.2f  %.2f  %.2f\n", result.name, result.x, result.y, result.z);
}

/**
 * @brief Displays a stored vector’s values by name.
 * 
 * This function trims and interprets user input as a vector name, then retrieves
 * the corresponding vector from storage and prints its components.
 * 
 * @param store Pointer to the vector storage structure.
 * @param input The vector name entered by the user.
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
 * @brief Main entry point for the vector calculator application.
 * 
 * This function manages user interaction, command parsing, and overall control
 * flow. It provides an interactive REPL-like environment for creating and 
 * manipulating vectors. The program continues until the user types `quit`.
 * 
 * @param argc Command-line argument count.
 * @param argv Command-line argument vector.
 * @return 0 on normal exit, non-zero on error.
 */
int main(int argc, char *argv[])
{
    VectorStore store = { .count = 0 };  // Initialize empty vector store

    // Handle command-line arguments
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
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[1]);
            printf("Use './vectorcalc -h' for help.\n");
            return 1;
        }
    }

    // Interactive command loop
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
    return 0;
}
