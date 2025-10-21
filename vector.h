/**
 * @file      : vector.h
 * @brief     : Declares vector structures and mathematical/vector store functions.
 * 
 * Name       : rostj@msoe.edu <Jesse Rost>
 * Date       : 10/27/25
 * Course     : CPE 2600
 * Assignment : Lab 7
 * Section    : 112
 */

#ifndef VECTOR_H
#define VECTOR_H
#define INITIAL_CAPACITY 5

/**
 * @brief Represents a named 3D vector with x, y, and z components.
 * 
 * This structure is used to store both the vector’s name
 * and its numerical components for mathematical operations.
 */
typedef struct
{
    char name[10];  /**< The vector’s name identifier (max 9 chars). */
    float x;        /**< The x-component of the vector. */
    float y;        /**< The y-component of the vector. */
    float z;        /**< The z-component of the vector. */
} vector;

/**
 * @brief Represents a collection of stored vectors.
 * 
 * This structure maintains a dynamically allocated array
 * of named vectors and a count of how many are currently stored.
 * It replaces the need for global variables by grouping all
 * storage and management into one unit.
 */
typedef struct {
    vector *vectors;   /**< Dynamically allocated array of vectors. */
    int count;         /**< Number of vectors currently stored. */
    int capacity;      /**< Total allocated slots. */
} VectorStore;

/* ==================== Initialization and Cleanup ==================== */

/**
 * @brief Initializes a vector store with an initial capacity.
 * @param store Pointer to the VectorStore to initialize.
 */
void init_vector_store(VectorStore *store);

/**
 * @brief Frees dynamically allocated memory for a vector store.
 * @param store Pointer to the VectorStore to free.
 */
void free_vector_store(VectorStore *store);

/* ==================== Math Operations ==================== */

/** 
 * @brief Adds two vectors component-wise. 
 * @param a The first vector operand.
 * @param b The second vector operand.
 * @return A new vector representing the sum of a and b.
 */
vector add(vector a, vector b);

/** 
 * @brief Subtracts the second vector from the first vector component-wise. 
 * @param a The vector to be subtracted from (minuend). 
 * @param b The vector to subtract (subtrahend). 
 * @return A new vector representing the difference (a - b). 
 */
vector sub(vector a, vector b);

/** 
 * @brief Multiplies two vectors component-wise (Hadamard product). 
 * @param a The first vector operand. 
 * @param b The second vector operand. 
 * @return A new vector where each component is the product of the 
 * corresponding components of a and b.
 */
vector mult(vector a, vector b);

/** 
 * @brief Calculates the dot product (scalar product) of two vectors. 
 * @param a The first vector operand. 
 * @param b The second vector operand. 
 * @return A float value representing the dot product. 
 */
float dot_prod(vector a, vector b);

/** 
 * @brief Calculates the cross product (vector product) of two vectors. 
 * @param a The first vector operand. 
 * @param b The second vector operand. 
 * @return A new vector that is orthogonal to both a and b. 
 */
vector cross_prod(vector a, vector b);

/* ==================== Store Management ==================== */

/** 
 * @brief Adds or replaces a vector in the vector store. 
 * If a vector with the same name already exists, it is replaced. 
 * If there is capacity remaining, the vector is appended. 
 * @param store Pointer to the VectorStore where the vector is stored. 
 * @param v The vector to add or replace. 
 * @return 1 if successful, 0 if the store is full. 
 */
int add_vector(VectorStore *store, vector v);

/** 
 * @brief Searches the vector store for a vector by its name. 
 * @param store Pointer to the VectorStore to search. 
 * @param name The name of the vector to find. 
 * @return A pointer to the found vector, or NULL if not found. 
 */
vector *find_vector(VectorStore *store, const char *name);

/** 
 * @brief Removes all vectors from the vector store. 
 * @param store Pointer to the VectorStore to clear. 
 */
void clear_vectors(VectorStore *store);

/**
 * @brief Displays all currently stored vectors and their components. 
 * @param store Pointer to the VectorStore containing vectors to list. 
 */
void list_vectors(const VectorStore *store);

/* ==================== Expression Evaluation ==================== */

/** 
 * @brief Evaluates a vector expression such as "a + b", "a x b", or "2 * a". 
 * Supports addition, subtraction, dot and cross products, 
 * and scalar multiplication between defined vectors. 
 * @param store Pointer to the VectorStore containing available vectors. 
 * @param expr The expression string to evaluate. 
 * @param result Pointer to a vector where the result is stored. 
 * @return 1 if successful, 0 if an error occurs. 
 */
int evaluate_expression(VectorStore *store, const char *expr, vector *result);

/**
 * @brief Initializes a VectorStore structure.
 *
 * Allocates dynamic memory for the vector array, sets the initial capacity,
 * and initializes the count to zero. This must be called before any vectors
 * are added to the store to ensure valid memory allocation.
 *
 * @param store Pointer to the VectorStore to initialize.
 *
 * @note If memory allocation fails, the program will terminate with an error message.
 */

void init_store(VectorStore *store);

/**
 * @brief Frees all dynamically allocated memory used by a VectorStore.
 *
 * Releases the heap memory previously allocated for the vector array.
 * After freeing, the function resets the pointer to NULL and count/capacity
 * to zero to prevent dangling pointers and accidental reuse.
 *
 * @param store Pointer to the VectorStore to free.
 */

void free_store(VectorStore *store);

#endif // VECTOR_H
