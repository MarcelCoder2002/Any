/**
 * @file test_any.c
 * @brief Unit tests for the Any library
 * @author mac
 * @date 2025-09-25
 */

#include "any.h"
#include <stdio.h>
#include <string.h>

// Test macro
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("❌ FAILED: %s\n", message); \
            return 1; \
        } else { \
            printf("✅ SUCCESS: %s\n", message); \
        } \
    } while(0)

int test_basic_operations() {
    printf("\n=== Basic operations test ===\n");
    
    // Test création
    Any *a = Any_Create(0, sizeof(int));
    TEST_ASSERT(a != NULL, "Any creation");
    TEST_ASSERT(!Any_HasValue(a), "Empty Any has no value");
    
    // Test set/get
    const int value = 42;
    Any_Set(a, 0, &value, sizeof(int));
    TEST_ASSERT(Any_HasValue(a), "Any has value after Set");
    TEST_ASSERT(Any_GetValue(a, int) == 42, "GetValue returns correct value");
    
    Any_Destroy(a);
    return 0;
}

int test_copy() {
    printf("\n=== Any_Copy test ===\n");
    
    Any_DeclareAny(original, int, 123);
    Any *copy = Any_Copy(original);
    
    TEST_ASSERT(copy != NULL, "Any_Copy returns valid object");
    TEST_ASSERT(Any_GetValue(copy, int) == 123, "Copy has same value");
    TEST_ASSERT(Any_Equals(original, copy), "Original and copy are equal");
    
    // Modifier la copie ne doit pas affecter l'original
    Any_SetValue(copy, int, 456);
    TEST_ASSERT(Any_GetValue(original, int) == 123, "Copy modification does not affect original");
    TEST_ASSERT(Any_GetValue(copy, int) == 456, "Modified copy has new value");
    
    Any_Destroy(original);
    Any_Destroy(copy);
    return 0;
}

int test_move() {
    printf("\n=== Any_Move test ===\n");
    
    Any_DeclareAny(source, double, 3.14159);
    Any *dest = Any_Create(0, sizeof(double));

    const double original_value = Any_GetValue(source, double);
    Any_Move(dest, source);
    
    TEST_ASSERT(Any_GetValue(dest, double) == original_value, "Move preserves value");
    TEST_ASSERT(Any_IsNull(source), "Source is null after move");
    TEST_ASSERT(Any_HasValue(dest), "Destination has value after move");
    
    Any_Destroy(source);
    Any_Destroy(dest);
    return 0;
}

int test_swap() {
    printf("\n=== Any_Swap test ===\n");
    
    Any_DeclareAny(a, int, 100);
    Any_DeclareAny(b, int, 200);

    const int val_a_before = Any_GetValue(a, int);
    const int val_b_before = Any_GetValue(b, int);
    
    Any_Swap(a, b);
    
    TEST_ASSERT(Any_GetValue(a, int) == val_b_before, "A has B's value after swap");
    TEST_ASSERT(Any_GetValue(b, int) == val_a_before, "B has A's value after swap");
    
    Any_Destroy(a);
    Any_Destroy(b);
    return 0;
}

int test_string_operations() {
    printf("\n=== String operations test ===\n");
    
    Any *a = Any_Create(0, 0);
    Any_SetStringValue(a, "Test String");
    
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetStringValue");
    TEST_ASSERT(strcmp(Any_GetStringValue(a), "Test String") == 0, "Get returns correct string");
    
    // Test string copy
    Any *copy = Any_Copy(a);
    TEST_ASSERT(copy != NULL, "String copy successful");
    TEST_ASSERT(strcmp(Any_GetStringValue(copy), "Test String") == 0, "Copy has same string");
    
    Any_Destroy(a);
    Any_Destroy(copy);
    return 0;
}

int test_array_operations() {
    printf("\n=== Array operations test ===\n");
    
    // Test array with Any_SetArrayValue
    int arr[3] = {10, 20, 30};
    Any *a = Any_Create(0, 0);
    Any_SetArrayValue(a, int, 3, arr);
    
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetArrayValue");
    TEST_ASSERT(Any_GetSize(a) == sizeof(int) * 3, "Array size is correct");

    const int *retrieved_arr = Any_GetArrayValue(a, int);
    TEST_ASSERT(retrieved_arr != NULL, "GetArrayValue returns pointer");
    TEST_ASSERT(retrieved_arr[0] == 10, "First element is correct");
    TEST_ASSERT(retrieved_arr[1] == 20, "Second element is correct");
    TEST_ASSERT(retrieved_arr[2] == 30, "Third element is correct");
    
    Any_Destroy(a);
    return 0;
}

int test_literal_array_operations() {
    printf("\n=== Literal array operations test ===\n");
    
    // Test literal array
    Any *a = Any_Create(0, 0);
    Any_SetLiteralArrayValue(a, int, {1, 2, 3, 4, 5});
    
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetLiteralArrayValue");
    TEST_ASSERT(Any_GetSize(a) == sizeof(int) * 5, "Literal array size is correct");

    const int *arr = Any_GetArrayValue(a, int);
    TEST_ASSERT(arr[0] == 1, "Literal array element 0 is correct");
    TEST_ASSERT(arr[1] == 2, "Literal array element 1 is correct");
    TEST_ASSERT(arr[2] == 3, "Literal array element 2 is correct");
    TEST_ASSERT(arr[3] == 4, "Literal array element 3 is correct");
    TEST_ASSERT(arr[4] == 5, "Literal array element 4 is correct");
    
    Any_Destroy(a);
    return 0;
}

int test_nested_array_operations() {
    printf("\n=== Nested array operations test ===\n");
    
    // Test 2D array
    Any *a = Any_Create(0, 0);
    Any_SetLiteralNestedArrayValue(a, int, [2][3], {{1, 2, 3}, {4, 5, 6}});
    
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetLiteralNestedArrayValue");
    TEST_ASSERT(Any_GetSize(a) == sizeof(int) * 2 * 3, "Nested array size is correct");

    const int (*matrix)[3] = (int(*)[3])Any_GetArrayValue(a, int);
    TEST_ASSERT(matrix[0][0] == 1, "Matrix[0][0] is correct");
    TEST_ASSERT(matrix[0][1] == 2, "Matrix[0][1] is correct");
    TEST_ASSERT(matrix[0][2] == 3, "Matrix[0][2] is correct");
    TEST_ASSERT(matrix[1][0] == 4, "Matrix[1][0] is correct");
    TEST_ASSERT(matrix[1][1] == 5, "Matrix[1][1] is correct");
    TEST_ASSERT(matrix[1][2] == 6, "Matrix[1][2] is correct");
    
    Any_Destroy(a);
    return 0;
}

int test_declare_macros() {
    printf("\n=== Declare macros test ===\n");
    
    // Test Any_DeclareLiteralArray
    Any_DeclareLiteralArray(literal_array, double, {3.14, 2.71, 1.41});
    TEST_ASSERT(Any_HasValue(literal_array), "DeclareLiteralArray creates valid Any");
    TEST_ASSERT(Any_GetSize(literal_array) == sizeof(double) * 3, "DeclareLiteralArray size is correct");

    const double *darr = Any_GetArrayValue(literal_array, double);
    TEST_ASSERT(darr[0] == 3.14, "DeclareLiteralArray element 0 is correct");
    TEST_ASSERT(darr[1] == 2.71, "DeclareLiteralArray element 1 is correct");
    TEST_ASSERT(darr[2] == 1.41, "DeclareLiteralArray element 2 is correct");
    
    // Test Any_DeclareLiteralNestedArray
    Any_DeclareLiteralNestedArray(nested_array, float, [2][2], {{1.1f, 2.2f}, {3.3f, 4.4f}});
    TEST_ASSERT(Any_HasValue(nested_array), "DeclareLiteralNestedArray creates valid Any");
    TEST_ASSERT(Any_GetSize(nested_array) == sizeof(float) * 2 * 2, "DeclareLiteralNestedArray size is correct");

    const float (*fmatrix)[2] = (float(*)[2])Any_GetArrayValue(nested_array, float);
    TEST_ASSERT(fmatrix[0][0] == 1.1f, "DeclareLiteralNestedArray[0][0] is correct");
    TEST_ASSERT(fmatrix[0][1] == 2.2f, "DeclareLiteralNestedArray[0][1] is correct");
    TEST_ASSERT(fmatrix[1][0] == 3.3f, "DeclareLiteralNestedArray[1][0] is correct");
    TEST_ASSERT(fmatrix[1][1] == 4.4f, "DeclareLiteralNestedArray[1][1] is correct");
    
    Any_Destroy(literal_array);
    Any_Destroy(nested_array);
    return 0;
}

int test_string_with_type() {
    printf("\n=== String with type test ===\n");
    
    // Test Any_SetStringValueWithType
    Any *a = Any_Create(0, 0);
    Any_SetStringValueWithType(a, 42, "Hello World");
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetStringValueWithType");
    TEST_ASSERT(Any_GetType(a) == 42, "Type ID is preserved");
    TEST_ASSERT(strcmp(Any_GetStringValue(a), "Hello World") == 0, "String value is correct");
    
    Any_Destroy(a);
    return 0;
}

int test_wide_string_operations() {
    printf("\n=== Wide string operations test ===\n");
    
    // Test wide string
    Any *a = Any_Create(0, 0);
    Any_SetWStringValue(a, L"Wide String Test");
    
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetWStringValue");
    TEST_ASSERT(wcscmp(Any_GetWStringValue(a), L"Wide String Test") == 0, "Wide string value is correct");
    
    // Test wide string with type
    Any *b = Any_Create(0, 0);
    Any_SetWStringValueWithType(b, 123, L"Typed Wide String");
    TEST_ASSERT(Any_HasValue(b), "Any has value after SetWStringValueWithType");
    TEST_ASSERT(Any_GetType(b) == 123, "Wide string type ID is preserved");
    TEST_ASSERT(wcscmp(Any_GetWStringValue(b), L"Typed Wide String") == 0, "Typed wide string value is correct");
    
    Any_Destroy(a);
    Any_Destroy(b);
    return 0;
}

int test_any_set_return_value() {
    printf("\n=== Any_Set return value test ===\n");
    
    Any *a = Any_Create(0, sizeof(int));
    const int value = 42;
    
    // Test successful set
    bool result = Any_Set(a, 0, &value, sizeof(int));
    TEST_ASSERT(result, "Any_Set returns true on success");
    TEST_ASSERT(Any_HasValue(a), "Any has value after successful Set");
    
    // Test failure with NULL value
    result = Any_Set(a, 0, NULL, sizeof(int));
    TEST_ASSERT(!result, "Any_Set returns false with NULL value");
    
    // Test failure with NULL Any
    result = Any_Set(NULL, 0, &value, sizeof(int));
    TEST_ASSERT(!result, "Any_Set returns false with NULL Any");
    
    Any_Destroy(a);
    return 0;
}

int test_array_with_type() {
    printf("\n=== Array with type test ===\n");
    
    int arr[4] = {100, 200, 300, 400};
    Any *a = Any_Create(0, 0);
    
    // Test Any_SetArrayValueWithType
    Any_SetArrayValueWithType(a, 999, int, 4, arr);
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetArrayValueWithType");
    TEST_ASSERT(Any_GetType(a) == 999, "Array type ID is preserved");
    TEST_ASSERT(Any_GetSize(a) == sizeof(int) * 4, "Array size is correct");

    const int *retrieved = Any_GetArrayValue(a, int);
    TEST_ASSERT(retrieved[0] == 100, "Array element 0 is correct");
    TEST_ASSERT(retrieved[1] == 200, "Array element 1 is correct");
    TEST_ASSERT(retrieved[2] == 300, "Array element 2 is correct");
    TEST_ASSERT(retrieved[3] == 400, "Array element 3 is correct");
    
    Any_Destroy(a);
    return 0;
}

int test_literal_array_with_type() {
    printf("\n=== Literal array with type test ===\n");
    
    // Test Any_SetLiteralArrayValueWithType
    Any *a = Any_Create(0, 0);
    Any_SetLiteralArrayValueWithType(a, 777, char, {'A', 'B', 'C', 'D'});
    
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetLiteralArrayValueWithType");
    TEST_ASSERT(Any_GetType(a) == 777, "Literal array type ID is preserved");
    TEST_ASSERT(Any_GetSize(a) == sizeof(char) * 4, "Literal array size is correct");

    const char *chars = Any_GetArrayValue(a, char);
    TEST_ASSERT(chars[0] == 'A', "Literal array char 0 is correct");
    TEST_ASSERT(chars[1] == 'B', "Literal array char 1 is correct");
    TEST_ASSERT(chars[2] == 'C', "Literal array char 2 is correct");
    TEST_ASSERT(chars[3] == 'D', "Literal array char 3 is correct");
    
    Any_Destroy(a);
    return 0;
}

int test_nested_array_with_type() {
    printf("\n=== Nested array with type test ===\n");
    
    // Test Any_SetLiteralNestedArrayValueWithType
    Any *a = Any_Create(0, 0);
    Any_SetLiteralNestedArrayValueWithType(a, 555, short, [3][2], {{1, 2}, {3, 4}, {5, 6}});
    
    TEST_ASSERT(Any_HasValue(a), "Any has value after SetLiteralNestedArrayValueWithType");
    TEST_ASSERT(Any_GetType(a) == 555, "Nested array type ID is preserved");
    TEST_ASSERT(Any_GetSize(a) == sizeof(short) * 3 * 2, "Nested array size is correct");

    const short (*matrix)[2] = (short(*)[2])Any_GetArrayValue(a, short);
    TEST_ASSERT(matrix[0][0] == 1, "Nested array[0][0] is correct");
    TEST_ASSERT(matrix[0][1] == 2, "Nested array[0][1] is correct");
    TEST_ASSERT(matrix[1][0] == 3, "Nested array[1][0] is correct");
    TEST_ASSERT(matrix[1][1] == 4, "Nested array[1][1] is correct");
    TEST_ASSERT(matrix[2][0] == 5, "Nested array[2][0] is correct");
    TEST_ASSERT(matrix[2][1] == 6, "Nested array[2][1] is correct");
    
    Any_Destroy(a);
    return 0;
}

int test_declare_with_type() {
    printf("\n=== Declare with type test ===\n");
    
    // Test Any_DeclareLiteralArrayWithType
    Any_DeclareLiteralArrayWithType(literal_with_type, 333, long, {1000L, 2000L, 3000L});
    TEST_ASSERT(Any_HasValue(literal_with_type), "DeclareLiteralArrayWithType creates valid Any");
    TEST_ASSERT(Any_GetType(literal_with_type) == 333, "DeclareLiteralArrayWithType type ID is preserved");
    TEST_ASSERT(Any_GetSize(literal_with_type) == sizeof(long) * 3, "DeclareLiteralArrayWithType size is correct");

    const long *longs = Any_GetArrayValue(literal_with_type, long);
    TEST_ASSERT(longs[0] == 1000L, "DeclareLiteralArrayWithType element 0 is correct");
    TEST_ASSERT(longs[1] == 2000L, "DeclareLiteralArrayWithType element 1 is correct");
    TEST_ASSERT(longs[2] == 3000L, "DeclareLiteralArrayWithType element 2 is correct");
    
    // Test Any_DeclareLiteralNestedArrayWithType
    Any_DeclareLiteralNestedArrayWithType(nested_with_type, 111, unsigned char, [2][4], {{1, 2, 3, 4}, {5, 6, 7, 8}});
    TEST_ASSERT(Any_HasValue(nested_with_type), "DeclareLiteralNestedArrayWithType creates valid Any");
    TEST_ASSERT(Any_GetType(nested_with_type) == 111, "DeclareLiteralNestedArrayWithType type ID is preserved");
    TEST_ASSERT(Any_GetSize(nested_with_type) == sizeof(unsigned char) * 2 * 4, "DeclareLiteralNestedArrayWithType size is correct");

    const unsigned char (*ubmatrix)[4] = (unsigned char(*)[4])Any_GetArrayValue(nested_with_type, unsigned char);
    TEST_ASSERT(ubmatrix[0][0] == 1, "DeclareLiteralNestedArrayWithType[0][0] is correct");
    TEST_ASSERT(ubmatrix[0][1] == 2, "DeclareLiteralNestedArrayWithType[0][1] is correct");
    TEST_ASSERT(ubmatrix[0][2] == 3, "DeclareLiteralNestedArrayWithType[0][2] is correct");
    TEST_ASSERT(ubmatrix[0][3] == 4, "DeclareLiteralNestedArrayWithType[0][3] is correct");
    TEST_ASSERT(ubmatrix[1][0] == 5, "DeclareLiteralNestedArrayWithType[1][0] is correct");
    TEST_ASSERT(ubmatrix[1][1] == 6, "DeclareLiteralNestedArrayWithType[1][1] is correct");
    TEST_ASSERT(ubmatrix[1][2] == 7, "DeclareLiteralNestedArrayWithType[1][2] is correct");
    TEST_ASSERT(ubmatrix[1][3] == 8, "DeclareLiteralNestedArrayWithType[1][3] is correct");
    
    Any_Destroy(literal_with_type);
    Any_Destroy(nested_with_type);
    return 0;
}

int main() {
    printf("🧪 Unit tests for Any library\n");
    
    int result = 0;
    result += test_basic_operations();
    result += test_copy();
    result += test_move();
    result += test_swap();
    result += test_string_operations();
    result += test_array_operations();
    result += test_literal_array_operations();
    result += test_nested_array_operations();
    result += test_declare_macros();
    result += test_string_with_type();
    result += test_wide_string_operations();
    result += test_any_set_return_value();
    result += test_array_with_type();
    result += test_literal_array_with_type();
    result += test_nested_array_with_type();
    result += test_declare_with_type();
    
    printf("\n=== Test summary ===\n");
    if (result == 0) {
        printf("🎉 All tests passed successfully!\n");
    } else {
        printf("❌ %d test(s) failed.\n", result);
    }
    
    return result;
}
