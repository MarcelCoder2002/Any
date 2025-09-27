/**
 * @file stress_test.c
 * @brief Stress tests for the Any library
 * @author Marcel Raymond Goumou
 * @date 2025-09-25
 */

#include "any.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STRESS_ITERATIONS 10000
#define MAX_ARRAY_SIZE 1000

// Test macro
#define STRESS_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("❌ STRESS FAILED: %s\n", message); \
            return 1; \
        } \
    } while(0)

int stress_test_memory_management() {
    printf("\n=== Memory management stress test ===\n");
    
    for (int i = 0; i < STRESS_ITERATIONS; i++) {
        // Create and destroy many Any objects
        Any *a = Any_Create(i % 100, sizeof(int));
        STRESS_ASSERT(a != NULL, "Memory allocation should succeed");
        
        int value = i;
        Any_Set(a, i % 100, &value, sizeof(int));
        STRESS_ASSERT(Any_HasValue(a), "Any should have value");
        STRESS_ASSERT(Any_GetValue(a, int) == i, "Value should be preserved");
        
        Any_Destroy(a);
        
        if (i % 1000 == 0) {
            printf("   Processed %d objects...\n", i);
        }
    }
    
    printf("✅ Memory management stress test passed\n");
    return 0;
}

int stress_test_array_operations() {
    printf("\n=== Array operations stress test ===\n");
    
    for (int i = 0; i < 1000; i++) {
        const size_t array_size = (i % MAX_ARRAY_SIZE) + 1;
        int *test_array = malloc(array_size * sizeof(int));
        STRESS_ASSERT(test_array != NULL, "Test array allocation should succeed");
        
        // Fill array with test data
        for (size_t j = 0; j < array_size; j++) {
            test_array[j] = (int)(i * j);
        }
        
        // Store in Any
        Any *a = Any_Create(0, 0);
        Any_SetArrayValue(a, int, array_size, test_array);
        STRESS_ASSERT(Any_HasValue(a), "Any should have array value");
        STRESS_ASSERT(Any_GetSize(a) == array_size * sizeof(int), "Array size should be correct");
        
        // Verify data
        const int *retrieved = Any_GetArrayValue(a, int);
        for (size_t j = 0; j < array_size; j++) {
            STRESS_ASSERT(retrieved[j] == (int)(i * j), "Array data should be preserved");
        }
        
        // Test copy
        Any *copy = Any_Copy(a);
        STRESS_ASSERT(copy != NULL, "Array copy should succeed");
        STRESS_ASSERT(Any_Equals(a, copy), "Array copy should be equal");
        
        Any_Destroy(a);
        Any_Destroy(copy);
        free(test_array);
        
        if (i % 100 == 0) {
            printf("   Processed array %d (size %zu)...\n", i, array_size);
        }
    }
    
    printf("✅ Array operations stress test passed\n");
    return 0;
}

int stress_test_string_operations() {
    printf("\n=== String operations stress test ===\n");
    
    for (int i = 0; i < 1000; i++) {
        // Generate test string
        char test_string[256];
        snprintf(test_string, sizeof(test_string), "Test string number %d with some extra data", i);
        
        // Test regular string
        Any *a = Any_Create(0, 0);
        Any_SetStringValue(a, test_string);
        STRESS_ASSERT(Any_HasValue(a), "Any should have string value");
        STRESS_ASSERT(strcmp(Any_GetStringValue(a), test_string) == 0, "String should be preserved");
        
        // Test string with type
        Any *b = Any_Create(0, 0);
        Any_SetStringValueWithType(b, i % 1000, test_string);
        STRESS_ASSERT(Any_HasValue(b), "Any should have typed string value");
        STRESS_ASSERT(Any_GetType(b) == i % 1000, "Type ID should be preserved");
        STRESS_ASSERT(strcmp(Any_GetStringValue(b), test_string) == 0, "Typed string should be preserved");
        
        // Test wide string
        wchar_t wide_string[256];
        swprintf(wide_string, sizeof(wide_string)/sizeof(wchar_t), L"Wide test string %d", i);
        
        Any *c = Any_Create(0, 0);
        Any_SetWStringValue(c, wide_string);
        STRESS_ASSERT(Any_HasValue(c), "Any should have wide string value");
        STRESS_ASSERT(wcscmp(Any_GetWStringValue(c), wide_string) == 0, "Wide string should be preserved");
        
        Any_Destroy(a);
        Any_Destroy(b);
        Any_Destroy(c);
        
        if (i % 100 == 0) {
            printf("   Processed string %d...\n", i);
        }
    }
    
    printf("✅ String operations stress test passed\n");
    return 0;
}

int stress_test_move_swap_operations() {
    printf("\n=== Move/Swap operations stress test ===\n");
    
    for (int i = 0; i < 1000; i++) {
        // Create test objects
        Any_DeclareAny(a, int, i);
        Any_DeclareAny(b, double, i * 3.14159);
        
        // Test swap
        const int val_a_before = Any_GetValue(a, int);
        const double val_b_before = Any_GetValue(b, double);
        
        Any_Swap(a, b);
        
        STRESS_ASSERT(Any_GetValue(a, double) == val_b_before, "Swap should work correctly");
        STRESS_ASSERT(Any_GetValue(b, int) == val_a_before, "Swap should work correctly");
        
        // Test move
        Any *dest = Any_Create(0, sizeof(double));
        Any_Move(dest, a);
        
        STRESS_ASSERT(Any_GetValue(dest, double) == val_b_before, "Move should preserve value");
        STRESS_ASSERT(Any_IsNull(a), "Source should be null after move");
        
        Any_Destroy(a);
        Any_Destroy(b);
        Any_Destroy(dest);
        
        if (i % 100 == 0) {
            printf("   Processed move/swap %d...\n", i);
        }
    }
    
    printf("✅ Move/Swap operations stress test passed\n");
    return 0;
}

int stress_test_nested_arrays() {
    printf("\n=== Nested arrays stress test ===\n");
    
    for (int i = 0; i < 100; i++) {
        const int rows = (i % 10) + 1;
        const int cols = (i % 10) + 1;
        
        // Create nested array with Any_DeclareLiteralNestedArray
        if (rows == 2 && cols == 2) {
            Any_DeclareLiteralNestedArray(matrix, int, [2][2], {{i, i+1}, {i+2, i+3}});
            STRESS_ASSERT(Any_HasValue(matrix), "Nested array should have value");
            STRESS_ASSERT(Any_GetSize(matrix) == sizeof(int) * 2 * 2, "Nested array size should be correct");

            const int (*arr)[2] = (int(*)[2])Any_GetArrayValue(matrix, int);
            STRESS_ASSERT(arr[0][0] == i, "Nested array[0][0] should be correct");
            STRESS_ASSERT(arr[0][1] == i+1, "Nested array[0][1] should be correct");
            STRESS_ASSERT(arr[1][0] == i+2, "Nested array[1][0] should be correct");
            STRESS_ASSERT(arr[1][1] == i+3, "Nested array[1][1] should be correct");
            
            Any_Destroy(matrix);
        }
        
        if (i % 10 == 0) {
            printf("   Processed nested array %d...\n", i);
        }
    }
    
    printf("✅ Nested arrays stress test passed\n");
    return 0;
}

int stress_test_error_conditions() {
    printf("\n=== Error conditions stress test ===\n");
    
    for (int i = 0; i < 1000; i++) {
        // Test NULL Any
        bool result = Any_Set(NULL, 0, &i, sizeof(int));
        STRESS_ASSERT(!result, "Any_Set should return false with NULL Any");
        
        // Test NULL value
        Any *a = Any_Create(0, sizeof(int));
        result = Any_Set(a, 0, NULL, sizeof(int));
        STRESS_ASSERT(!result, "Any_Set should return false with NULL value");
        
        // Test NULL copy
        const Any *copy = Any_Copy(NULL);
        STRESS_ASSERT(copy == NULL, "Any_Copy should return NULL with NULL input");
        
        // Test NULL move
        Any_Move(NULL, a);
        Any_Move(a, NULL);
        
        // Test NULL swap
        Any_Swap(NULL, a);
        Any_Swap(a, NULL);
        
        Any_Destroy(a);
        
        if (i % 100 == 0) {
            printf("   Processed error test %d...\n", i);
        }
    }
    
    printf("✅ Error conditions stress test passed\n");
    return 0;
}

int main() {
    printf("🔥 Stress tests for Any library\n");
    printf("Running %d iterations for memory management test\n", STRESS_ITERATIONS);

    const clock_t start_time = clock();
    
    int result = 0;
    result += stress_test_memory_management();
    result += stress_test_array_operations();
    result += stress_test_string_operations();
    result += stress_test_move_swap_operations();
    result += stress_test_nested_arrays();
    result += stress_test_error_conditions();

    const clock_t end_time = clock();
    const double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n=== Stress test summary ===\n");
    if (result == 0) {
        printf("🎉 All stress tests passed successfully!\n");
        printf("⏱️  Total execution time: %.2f seconds\n", cpu_time);
    } else {
        printf("❌ %d stress test(s) failed.\n", result);
    }
    
    return result;
}
