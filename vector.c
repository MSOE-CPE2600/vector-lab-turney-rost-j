/**
 * @file      : vector.c
 * @brief     : Defines the mathematical operations and vector storage functions.
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/27/25
 * Course     : CPE 2600
 * Assignment : Lab 7
 * Section    : 112
 */

#include "vector.h"
#include "stdio.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include "util.h"

/**
 * @brief Initializes a vector store with an initial memory allocation.
 * @param store - Pointer to the VectorStore structure to initialize.
 */
void init_store(VectorStore *store) {
    store->vectors = malloc(INITIAL_CAPACITY * sizeof(vector));
    if (!store->vectors) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    store->count = 0;
    store->capacity = INITIAL_CAPACITY;
}

/**
 * @brief Frees all allocated memory associated with the vector store.
 * @param store - Pointer to the VectorStore to free.
 */
void free_store(VectorStore *store) {
    free(store->vectors);
    store->vectors = NULL;
    store->count = 0;
    store->capacity = 0;
}

/**
 * @brief Adds two vectors component-wise.
 * @param a - The first vector operand.
 * @param b - The second vector operand.
 * @return A new vector representing the sum of a and b.
 */
vector add(vector a, vector b) {
    vector result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

/**
 * @brief Subtracts one vector from another component-wise.
 * @param a - The vector to subtract from.
 * @param b - The vector to subtract.
 * @return A new vector representing the difference (a - b).
 */
vector sub(vector a, vector b) {
    vector result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

/**
 * @brief Multiplies two vectors component-wise.
 * @param a - The first vector operand.
 * @param b - The second vector operand.
 * @return A new vector containing the product of each component.
 */
vector mult(vector a, vector b) {
    vector result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

/**
 * @brief Computes the dot product of two vectors.
 * @param a - The first vector operand.
 * @param b - The second vector operand.
 * @return The scalar result of the dot product.
 */
float dot_prod(vector a, vector b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/**
 * @brief Computes the cross product of two vectors.
 * @param a - The first vector operand.
 * @param b - The second vector operand.
 * @return A new vector orthogonal to both a and b.
 */
vector cross_prod(vector a, vector b) {
    vector result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return result;
}

/**
 * @brief Adds or replaces a vector in the given vector store.
 * 
 * If a vector with the same name already exists, it is replaced. 
 * If there is available space, the vector is appended to the store.
 * If there is no available space, the array is doubled and the data is transferred over. 
 * 
 * @param store - Pointer to the VectorStore structure where vectors are stored.
 * @param v - The vector to add or replace.
 * @return 1 after successful
 */
int add_vector(VectorStore *store, vector v) {
    vector *existing = find_vector(store, v.name);
    if (existing != NULL) {
        *existing = v;
        printf("Vector '%s' replaced.\n", v.name);
        return 1;
    }

    // Expand if full
    if (store->count >= store->capacity) {
        // double the capacity
        int new_capacity = store->capacity * 2;
        // move the vectors into the new storage
        vector *temp = realloc(store->vectors, new_capacity * sizeof(vector));
        // check if realloc = NULL, this means that the realloc failed
        if (!temp) {
            // print error message
            fprintf(stderr, "Memory reallocation failed.\n");
            return 0;
        }
        // update stores pointer to the new block returned by realloc
        store->vectors = temp;
        // update the capacity field to that future resizes are correct
        store->capacity = new_capacity;
        printf("Vector storage expanded to %d.\n", new_capacity);
    }

    store->vectors[store->count++] = v;
    printf("Vector '%s' added.\n", v.name);
    return 1;
}

/**
 * @brief Searches for a vector by name within a store.
 * @param store - Pointer to the VectorStore containing the vectors.
 * @param name - The name of the vector to find.
 * @return Pointer to the vector if found, NULL otherwise.
 */
vector *find_vector(VectorStore *store, const char *name) {
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->vectors[i].name, name) == 0) {
            return &store->vectors[i];
        }
    }
    return NULL;
}

/**
 * @brief Removes all vectors from the given vector store.
 * @param store - Pointer to the VectorStore to clear.
 */
void clear_vectors(VectorStore *store) {
    store->count = 0;
    printf("All vectors cleared.\n");
}

/**
 * @brief Prints all stored vectors to the console.
 * @param store - Pointer to the VectorStore containing the vectors.
 */
void list_vectors(const VectorStore *store) {
    if (store->count == 0) {
        printf("No vectors stored.\n");
        return;
    }

    printf("Stored vectors:\n");
    for (int i = 0; i < store->count; i++) {
        printf("%s = %.2f  %.2f  %.2f\n",
               store->vectors[i].name,
               store->vectors[i].x,
               store->vectors[i].y,
               store->vectors[i].z);
    }
}

/**
 * @brief Evaluates a vector expression using the given store.
 * 
 * Supports operations including:
 * - Vector addition (+)
 * - Vector subtraction (-)
 * - Dot product (*)
 * - Cross product (x or X)
 * - Scalar multiplication (a * 2 or 2 * a)
 * 
 * @param store - Pointer to the VectorStore containing defined vectors.
 * @param expr - The string expression to evaluate.
 * @param result - Pointer to a vector where the computation result is stored.
 * @return 1 if the expression is valid and evaluated successfully, 0 otherwise.
 */
int evaluate_expression(VectorStore *store, const char *expr, vector *result) {
    char left[50], right[50];
    char op = 0;
    int i = 0;

    while (expr[i] && !strchr("+-*xX", expr[i])) i++;
    if (!expr[i]) {
        return 0;
    }
    op = expr[i];

    strncpy(left, expr, i);
    left[i] = '\0';
    strcpy(right, expr + i + 1);

    trim(left);
    trim(right);

    vector *v1, *v2;
    float scalar;
    result->x = result->y = result->z = 0;

    // scalar * vector
    if (isdigit(left[0]) || left[0] == '-' || left[0] == '+') {
        scalar = atof(left);
        v2 = find_vector(store, right);
        if (!v2) {
            return 0;
        }
        result->x = v2->x * scalar;
        result->y = v2->y * scalar;
        result->z = v2->z * scalar;
        return 1;
    }
    // vector * scalar
    else if (isdigit(right[0]) || right[0] == '-' || right[0] == '+') {
        v1 = find_vector(store, left);
        if (!v1) {
            return 0;
        }
        scalar = atof(right);
        result->x = v1->x * scalar;
        result->y = v1->y * scalar;
        result->z = v1->z * scalar;
        return 1;
    }
    // vector + vector
    else if (op == '+') {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            return 0;
        }
        *result = add(*v1, *v2);
        return 1;
    }
    // vector - vector
    else if (op == '-') {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            return 0;
        }
        *result = sub(*v1, *v2);
        return 1;
    }
    // vector * vector (dot product)
    else if (op == '*') {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            return 0;
        }
        float dot = dot_prod(*v1, *v2);
        result->x = dot;
        result->y = result->z = 0;
        return 1;
    }
    // vector x vector (cross product)
    else if (op == 'x' || op == 'X') {
        v1 = find_vector(store, left);
        v2 = find_vector(store, right);
        if (!v1 || !v2) {
            return 0;
        }
        *result = cross_prod(*v1, *v2);
        return 1;
    }

    return 0;
}
