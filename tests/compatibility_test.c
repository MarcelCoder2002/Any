/**
 * @file compatibility_test.c
 * @brief Compatibility tests for various data types
 * @author Marcel Raymond Goumou
 * @date 2025-09-25
 */

#include "any.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Test macro
#define COMPAT_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("❌ COMPAT FAILED: %s\n", message); \
            return 1; \
        } else { \
            printf("✅ SUCCESS: %s\n", message); \
        } \
    } while(0)

int test_basic_types() {
    printf("\n=== Basic types compatibility test ===\n");
    
    // Test char
    Any_DeclareAny(char_var, char, 'A');
    COMPAT_ASSERT(Any_GetValue(char_var, char) == 'A', "char type works");
    Any_Destroy(char_var);
    
    // Test unsigned char
    Any_DeclareAny(uchar_var, unsigned char, 255);
    COMPAT_ASSERT(Any_GetValue(uchar_var, unsigned char) == 255, "unsigned char type works");
    Any_Destroy(uchar_var);
    
    // Test short
    Any_DeclareAny(short_var, short, -32768);
    COMPAT_ASSERT(Any_GetValue(short_var, short) == -32768, "short type works");
    Any_Destroy(short_var);
    
    // Test unsigned short
    Any_DeclareAny(ushort_var, unsigned short, 65535);
    COMPAT_ASSERT(Any_GetValue(ushort_var, unsigned short) == 65535, "unsigned short type works");
    Any_Destroy(ushort_var);
    
    // Test int
    Any_DeclareAny(int_var, int, -2147483648);
    COMPAT_ASSERT(Any_GetValue(int_var, int) == -2147483648, "int type works");
    Any_Destroy(int_var);
    
    // Test unsigned int
    Any_DeclareAny(uint_var, unsigned int, 4294967295U);
    COMPAT_ASSERT(Any_GetValue(uint_var, unsigned int) == 4294967295U, "unsigned int type works");
    Any_Destroy(uint_var);
    
    // Test long
    Any_DeclareAny(long_var, long, -9223372036854775807L);
    COMPAT_ASSERT(Any_GetValue(long_var, long) == -9223372036854775807L, "long type works");
    Any_Destroy(long_var);
    
    // Test unsigned long
    Any_DeclareAny(ulong_var, unsigned long, 18446744073709551615UL);
    COMPAT_ASSERT(Any_GetValue(ulong_var, unsigned long) == 18446744073709551615UL, "unsigned long type works");
    Any_Destroy(ulong_var);
    
    // Test long long
    Any_DeclareAny(llong_var, long long, -9223372036854775807LL);
    COMPAT_ASSERT(Any_GetValue(llong_var, long long) == -9223372036854775807LL, "long long type works");
    Any_Destroy(llong_var);
    
    // Test unsigned long long
    Any_DeclareAny(ullong_var, unsigned long long, 18446744073709551615ULL);
    COMPAT_ASSERT(Any_GetValue(ullong_var, unsigned long long) == 18446744073709551615ULL, "unsigned long long type works");
    Any_Destroy(ullong_var);
    
    // Test float
    Any_DeclareAny(float_var, float, 3.14159f);
    COMPAT_ASSERT(Any_GetValue(float_var, float) == 3.14159f, "float type works");
    Any_Destroy(float_var);
    
    // Test double
    Any_DeclareAny(double_var, double, 2.718281828459045);
    COMPAT_ASSERT(Any_GetValue(double_var, double) == 2.718281828459045, "double type works");
    Any_Destroy(double_var);
    
    // Test long double
    Any_DeclareAny(ldouble_var, long double, 1.41421356237309504880L);
    COMPAT_ASSERT(Any_GetValue(ldouble_var, long double) == 1.41421356237309504880L, "long double type works");
    Any_Destroy(ldouble_var);
    
    return 0;
}

int test_pointer_types() {
    printf("\n=== Pointer types compatibility test ===\n");
    
    // Test void pointer
    int value = 42;
    void *ptr = &value;
    Any_DeclareAny(void_ptr_var, void*, ptr);
    COMPAT_ASSERT(Any_GetValue(void_ptr_var, void*) == ptr, "void* type works");
    Any_Destroy(void_ptr_var);
    
    // Test function pointer (using manual approach due to macro limitations)
    void (*func_ptr)(void) = (void(*)(void))0x12345678;
    Any *func_ptr_var = Any_Create(0, sizeof(void(*)(void)));
    Any_Set(func_ptr_var, 0, &func_ptr, sizeof(void(*)(void)));
    void (**retrieved_func_ptr)(void) = (void(**)(void))Any_Get(func_ptr_var);
    COMPAT_ASSERT(*retrieved_func_ptr == func_ptr, "function pointer type works");
    Any_Destroy(func_ptr_var);
    
    return 0;
}

int test_stdint_types() {
    printf("\n=== stdint.h types compatibility test ===\n");
    
    // Test int8_t
    Any_DeclareAny(int8_var, int8_t, -128);
    COMPAT_ASSERT(Any_GetValue(int8_var, int8_t) == -128, "int8_t type works");
    Any_Destroy(int8_var);
    
    // Test uint8_t
    Any_DeclareAny(uint8_var, uint8_t, 255);
    COMPAT_ASSERT(Any_GetValue(uint8_var, uint8_t) == 255, "uint8_t type works");
    Any_Destroy(uint8_var);
    
    // Test int16_t
    Any_DeclareAny(int16_var, int16_t, -32768);
    COMPAT_ASSERT(Any_GetValue(int16_var, int16_t) == -32768, "int16_t type works");
    Any_Destroy(int16_var);
    
    // Test uint16_t
    Any_DeclareAny(uint16_var, uint16_t, 65535);
    COMPAT_ASSERT(Any_GetValue(uint16_var, uint16_t) == 65535, "uint16_t type works");
    Any_Destroy(uint16_var);
    
    // Test int32_t
    Any_DeclareAny(int32_var, int32_t, -2147483648);
    COMPAT_ASSERT(Any_GetValue(int32_var, int32_t) == -2147483648, "int32_t type works");
    Any_Destroy(int32_var);
    
    // Test uint32_t
    Any_DeclareAny(uint32_var, uint32_t, 4294967295U);
    COMPAT_ASSERT(Any_GetValue(uint32_var, uint32_t) == 4294967295U, "uint32_t type works");
    Any_Destroy(uint32_var);
    
    // Test int64_t
    Any_DeclareAny(int64_var, int64_t, -9223372036854775807LL);
    COMPAT_ASSERT(Any_GetValue(int64_var, int64_t) == -9223372036854775807LL, "int64_t type works");
    Any_Destroy(int64_var);
    
    // Test uint64_t
    Any_DeclareAny(uint64_var, uint64_t, 18446744073709551615ULL);
    COMPAT_ASSERT(Any_GetValue(uint64_var, uint64_t) == 18446744073709551615ULL, "uint64_t type works");
    Any_Destroy(uint64_var);
    
    // Test size_t
    Any_DeclareAny(size_var, size_t, SIZE_MAX);
    COMPAT_ASSERT(Any_GetValue(size_var, size_t) == SIZE_MAX, "size_t type works");
    Any_Destroy(size_var);
    
    // Test ptrdiff_t
    Any_DeclareAny(ptrdiff_var, ptrdiff_t, PTRDIFF_MAX);
    COMPAT_ASSERT(Any_GetValue(ptrdiff_var, ptrdiff_t) == PTRDIFF_MAX, "ptrdiff_t type works");
    Any_Destroy(ptrdiff_var);
    
    return 0;
}

int test_custom_structures() {
    printf("\n=== Custom structures compatibility test ===\n");
    
    // Test simple structure
    typedef struct {
        int x;
        int y;
    } Point;

    const Point p = {10, 20};
    Any_DeclareAny(point_var, Point, p);
    const Point retrieved_point = Any_GetValue(point_var, Point);
    COMPAT_ASSERT(retrieved_point.x == 10 && retrieved_point.y == 20, "simple structure works");
    Any_Destroy(point_var);
    
    // Test structure with different types
    typedef struct {
        char name[32];
        int age;
        double salary;
    } Employee;

    const Employee emp = {"John Doe", 30, 50000.0};
    Any_DeclareAny(emp_var, Employee, emp);
    const Employee retrieved_emp = Any_GetValue(emp_var, Employee);
    COMPAT_ASSERT(strcmp(retrieved_emp.name, "John Doe") == 0 && 
                  retrieved_emp.age == 30 && 
                  retrieved_emp.salary == 50000.0, "complex structure works");
    Any_Destroy(emp_var);
    
    // Test union
    typedef union {
        int i;
        float f;
        char c;
    } TestUnion;

    const TestUnion u = {.i = 42};
    Any_DeclareAny(union_var, TestUnion, u);
    const TestUnion retrieved_union = Any_GetValue(union_var, TestUnion);
    COMPAT_ASSERT(retrieved_union.i == 42, "union type works");
    Any_Destroy(union_var);
    
    return 0;
}

int test_enum_types() {
    printf("\n=== Enum types compatibility test ===\n");
    
    // Test enum
    typedef enum {
        RED = 0,
        GREEN = 1,
        BLUE = 2
    } Color;
    
    Any_DeclareAny(color_var, Color, GREEN);
    COMPAT_ASSERT(Any_GetValue(color_var, Color) == GREEN, "enum type works");
    Any_Destroy(color_var);
    
    return 0;
}

int main() {
    printf("🔧 Compatibility tests for Any library\n");
    
    int result = 0;
    result += test_basic_types();
    result += test_pointer_types();
    result += test_stdint_types();
    result += test_custom_structures();
    result += test_enum_types();
    
    printf("\n=== Compatibility test summary ===\n");
    if (result == 0) {
        printf("🎉 All compatibility tests passed!\n");
        printf("✅ Any library is compatible with all tested data types\n");
    } else {
        printf("❌ %d compatibility test(s) failed.\n", result);
    }
    
    return result;
}
