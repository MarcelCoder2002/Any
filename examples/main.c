/**
 * @file main.c
 * @brief Example usage of the Any library
 * @author mac
 * @date 2025-09-25
 */

#include "any.h"

#include <stdio.h>


int main(int argc, char *argv[]) {
    printf("=== Any library features test ===\n\n");
    
    // Test 1: Basic creation and usage
    printf("1. Basic test:\n");
    Any_DeclareAny(a, int, 10);
    printf("   Initial value: %d\n", Any_GetValue(a, int));
    
    Any_SetValue(a, int, 78);
    printf("   After modification: %d\n", Any_GetValue(a, int));
    
    // Test 3: Any_Copy
    printf("\n3. Any_Copy test:\n");
    Any *copy = Any_Copy(a);
    if (copy) {
        printf("   Copy created: %d\n", Any_GetValue(copy, int));
        Any_Destroy(copy);
    }
    
    // Test 4: Any_Move
    printf("\n4. Any_Move test:\n");
    Any *dest = Any_Create(0, sizeof(int));
    Any_Move(dest, a);
    printf("   After move - dest: %d\n", Any_GetValue(dest, int));
    printf("   After move - src (a): %s\n", Any_IsNull(a) ? "null" : "non-null");
    
    // Test 5: Any_Swap
    printf("\n5. Any_Swap test:\n");
    Any_DeclareAny(b, double, 3.14159);
    printf("   Before swap - dest: %d, b: %.5f\n", Any_GetValue(dest, int), Any_GetValue(b, double));
    Any_Swap(dest, b);
    printf("   After swap - dest: %.5f, b: %d\n", Any_GetValue(dest, double), Any_GetValue(b, int));
    
    // Test 6: Different types with Any_ToString
    printf("\n6. Any_ToString test with different types:\n");
    
    // Character string
    Any_SetStringValue(b, "Hello World");
    printf("   String: %s\n", Any_GetStringValue(b));

    // Float
    Any_SetValue(b, float, 2.71828f);
    printf("   Float: %f\n", Any_GetValue(b, float));

    // Test with double to see the difference
    Any_SetValue(b, double, 2.718281828459045);
    printf("   Double: %f\n", Any_GetValue(b, double));

    // Test 7: Any_HasValue
    printf("\n7. Any_HasValue test:\n");
    printf("   b has value: %s\n", Any_HasValue(b) ? "yes" : "no");
    printf("   dest has value: %s\n", Any_HasValue(dest) ? "yes" : "no");
    printf("   Any_null has value: %s\n", Any_HasValue(Any_null) ? "yes" : "no");
    
    // Cleanup
    Any_Destroy(a);
    Any_Destroy(b);
    Any_Destroy(dest);
    
    printf("\n=== Tests completed ===\n");
    Any_MemoryReport();
    return 0;
}
