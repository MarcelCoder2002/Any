/**
 * @file any.h
 * @brief Generic type container library for C
 * @author Marcel Raymond Goumou
 * @date 2025-09-25
 */

#ifndef ANY_H
#define ANY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

/**
 * @brief Get the stored value as a given type.
 *
 * Example:
 * @code
 * int x = Any_GetValue(myAny, int);
 * @endcode
 *
 * @param self Pointer to an Any object.
 * @param type The target type to cast to.
 */
#define Any_GetValue(self, type) (*(type *) Any_Get(self))

/**
 * @brief Get the stored array value as @c type* .
 *
 * Example:
 * @code
 * int *arr = Any_GetArrayValue(myAny, int);
 * @endcode
 *
 * @param self Pointer to an Any object.
 * @param type The type of the array elements.
 */
#define Any_GetArrayValue(self, type) ((type *) Any_Get(self))

/**
 * @brief Get the stored string value.
 *
 * Example:
 * @code
 * const char *s = Any_GetStringValue(myAny);
 * @endcode
 *
 * @param self Pointer to an Any object.
 */
#define Any_GetStringValue(self) Any_GetArrayValue(self, const char)

/**
 * @brief Get the stored wide string value.
 *
 * Example:
 * @code
 * const wchar_t *ws = Any_GetWStringValue(myAny);
 * @endcode
 *
 * @param self Pointer to an Any object.
 */
#define Any_GetWStringValue(self) Any_GetArrayValue(self, const wchar_t)

/**
 * @brief Set the value of an Any object using a typed value.
 *
 * Example:
 * @code
 * Any_SetValue(myAny, int, 42);
 * @endcode
 *
 * @param self  Pointer to an Any object.
 * @param type  The type of the value.
 * @param value The value to set.
 */
#define Any_SetValue(self, type, value) \
    Any_SetValueWithType(self, 0, type, value)

/**
 * @brief Set the value of an Any object from a C string.
 *
 * The string is copied including the terminating '\0'.
 *
 * Example:
 * @code
 * Any_SetStringValue(myAny, "Hello");
 * @endcode
 *
 * @param self  Pointer to an Any object.
 * @param value The value to set.
 */
#define Any_SetStringValue(self, value) \
    Any_SetStringValueWithType(self, 0, value)

/**
 * @brief Set the value of an Any object from a C string.
 *
 * The string is copied including the terminating '\0'.
 *
 * Example:
 * @code
 * Any_SetStringValueWithType(myAny, 0, "Hello");
 * @endcode
 *
 * @param self  Pointer to an Any object.
 * @param typeID Custom type identifier (optional, may be 0).
 * @param value The value to set.
 */
#define Any_SetStringValueWithType(self, typeID, value) \
    Any_SetArrayValueWithType(self, typeID, char, strlen(value) + 1, value)

/**
 * @brief Set the value of an Any object from a wide string.
 *
 * The string is copied including the terminating L'\0'.
 *
 * Example:
 * @code
 * Any_SetWStringValue(myAny, L"Hello");
 * @endcode
 *
 * @param self  Pointer to an Any object.
 * @param value The value to set.
 */
#define Any_SetWStringValue(self, value) \
    Any_SetWStringValueWithType(self, 0, value)

/**
 * @brief Set the value of an Any object from a wide string.
 *
 * The string is copied including the terminating L'\0'.
 *
 * Example:
 * @code
 * Any_SetWStringValueWithType(myAny, 0, L"Hello");
 * @endcode
 *
 * @param self  Pointer to an Any object.
 * @param typeID Custom type identifier (optional, may be 0).
 * @param value The value to set.
 */
#define Any_SetWStringValueWithType(self, typeID, value) \
    Any_SetArrayValueWithType(self, typeID, wchar_t, wcslen(value) + 1, value)

/**
 * @brief Set the value of an Any object from a literal array.
 *
 * Example:
 * @code
 * Any_SetLiteralArrayValue(myAny, int, {1,2,3});
 * @endcode
 *
 * @param self Pointer to an Any object.
 * @param type The array element type.
 * @param value Initializer list for the array.
 */
#define Any_SetLiteralArrayValue(self, type, value...) \
    Any_SetLiteralArrayValueWithType(self, 0, type, value)

/**
 * @brief Set the value of an Any object from a literal array.
 *
 * Example:
 * @code
 * Any_SetLiteralArrayValueWithType(myAny, 0, int, {1,2,3});
 * @endcode
 *
 * @param self Pointer to an Any object.
 * @param typeID Custom type identifier (optional, may be 0).
 * @param type The array element type.
 * @param value Initializer list for the array.
 */
#define Any_SetLiteralArrayValueWithType(self, typeID, type, value...) \
    Any_SetLiteralNestedArrayValueWithType(self, typeID, type, [], value)

/**
 * @brief Set the value of an Any object from an array pointer.
 *
 * Example:
 * @code
 * int arr[3] = {1,2,3};
 * Any_SetArrayValue(myAny, int, 3, arr);
 * @endcode
 *
 * @param self   Pointer to an Any object.
 * @param type   The array element type.
 * @param length Number of elements in the array.
 * @param value  Pointer to the array.
 */
#define Any_SetArrayValue(self, type, length, value...) \
    Any_SetArrayValueWithType(self, 0, type, length, value)

/**
 * @brief Set the value of an Any object from an array pointer.
 *
 * Example:
 * @code
 * int arr[3] = {1,2,3};
 * Any_SetArrayValueWithType(myAny, 0, int, 3, arr);
 * @endcode
 *
 * @param self   Pointer to an Any object.
 * @param typeID Custom type identifier (optional, may be 0).
 * @param type   The array element type.
 * @param length Number of elements in the array.
 * @param value  Pointer to the array.
 */
#define Any_SetArrayValueWithType(self, typeID, type, length, value...) { \
    type *tmp = value; \
    Any_Set((self), (typeID), tmp, sizeof(type) * (length)); \
}

/**
 * @brief Set the value of an Any object from a nested array literal.
 *
 * Example:
 * @code
 * Any_SetLiteralNestedArrayValue(myAny, int, [2][2], {{1,2},{3,4}});
 * @endcode
 *
 * @param self    Pointer to an Any object.
 * @param type    The base type.
 * @param nesting Array nesting specifier (e.g. [2][2]).
 * @param value   Initializer list for the nested array.
 */
#define Any_SetLiteralNestedArrayValue(self, type, nesting, value...) \
    Any_SetLiteralNestedArrayValueWithType(self, 0, type, nesting, value)

/**
 * @brief Set the value of an Any object from a nested array literal.
 *
 * Example:
 * @code
 * Any_SetLiteralNestedArrayValueWithType(myAny, 0, int, [2][2], {{1,2},{3,4}});
 * @endcode
 *
 * @param self    Pointer to an Any object.
 * @param typeID  Custom type identifier (optional, may be 0).
 * @param type    The base type.
 * @param nesting Array nesting specifier (e.g. [2][2]).
 * @param value   Initializer list for the nested array.
 */
#define Any_SetLiteralNestedArrayValueWithType(self, typeID, type, nesting, value...) { \
    type tmp nesting = value; \
    Any_Set((self), typeID, tmp, sizeof(tmp)); \
}

/**
 * @brief Set the value of an Any object using a typed value and a custom typeID.
 *
 * Example:
 * @code
 * Any_SetValueWithType(myAny, 1, double, 3.14);
 * @endcode
 *
 * @param self   Pointer to an Any object.
 * @param typeID Custom type identifier (optional, may be 0).
 * @param type   The C type of the value.
 * @param value  The value to set.
 */
#define Any_SetValueWithType(self, typeID, type, value) { \
    type tmp = (value); \
    Any_Set((self), typeID, &tmp, sizeof(type)); \
}

/**
 * @brief Declare and initialize a new Any variable with type and value.
 *
 * Example:
 * @code
 * Any_Declare(myVar, 1, double, 3.14);
 * @endcode
 *
 * @param variable Variable name to declare.
 * @param typeID   Type identifier (optional, may be 0).
 * @param type     C type of the value.
 * @param value    Initial value.
 */
#define Any_Declare(variable, typeID, type, value) \
    Any *variable; \
    { \
        type tmp = (value); \
        variable = Any_Create((typeID), sizeof(type)); \
        Any_Set(variable, typeID, &tmp, sizeof(type)); \
    }

/**
 * @brief Declare and initialize a new Any variable with type and nested array literal value.
 *
 * Example:
 * @code
 * Any_DeclareLiteralNestedArrayWithType(myVar, 1, double, [][2], {{3.4, 5.6}, {-6.5, -4.3}});
 * @endcode
 *
 * @param variable Variable name to declare.
 * @param typeID   Type identifier (optional, may be 0).
 * @param type     C type of the value.
 * @param nesting  Array nesting specifier (e.g. [2][2]).
 * @param value    Initial value.
 */
#define Any_DeclareLiteralNestedArrayWithType(variable, typeID, type, nesting, value...) \
    Any *variable; \
    { \
        type tmp nesting = value; \
        variable = Any_Create((typeID), sizeof(tmp)); \
        Any_Set(variable, typeID, tmp, sizeof(tmp)); \
    }

/**
 * @brief Declare and initialize a new Any variable with nested array literal value.
 *
 * Example:
 * @code
 * Any_DeclareLiteralNestedArray(myVar, double, [][2], {{3.4, 5.6}, {-6.5, -4.3}});
 * @endcode
 *
 * @param variable Variable name to declare.
 * @param type     C type of the value.
 * @param nesting  Array nesting specifier (e.g. [2][2]).
 * @param value    Initial value.
 */
#define Any_DeclareLiteralNestedArray(variable, type, nesting, value...) \
    Any_DeclareLiteralNestedArrayWithType(variable, 0, type, nesting, value)

/**
 * @brief Declare and initialize a new Any variable with type and array literal value.
 *
 * Example:
 * @code
 * Any_DeclareLiteralArrayWithType(myVar, 0, double, {3.4, 5.6});
 * @endcode
 *
 * @param variable Variable name to declare.
 * @param typeID   Type identifier (optional, may be 0).
 * @param type     C type of the value.
 * @param value    Initial value.
 */
#define Any_DeclareLiteralArrayWithType(variable, typeID, type, value...) \
    Any_DeclareLiteralNestedArrayWithType(variable, typeID, type, [], value)

/**
 * @brief Declare and initialize a new Any variable with array literal value.
 *
 * Example:
 * @code
 * Any_DeclareLiteralArray(myVar, double, {3.4, 5.6});
 * @endcode
 *
 * @param variable Variable name to declare.
 * @param type     C type of the value.
 * @param value    Initial value.
 */
#define Any_DeclareLiteralArray(variable, type, value...) \
    Any_DeclareLiteralArrayWithType(variable, 0, type, value)

/**
 * @brief Declare an Any variable without specifying a typeID.
 *
 * Example:
 * @code
 * Any_DeclareAny(myVar, int, 10);
 * @endcode
 *
 * @param variable Variable name to declare.
 * @param type     C type of the value.
 * @param value    Initial value.
 */
#define Any_DeclareAny(variable, type, value) Any_Declare(variable, 0, type, value)

/**
 * @brief Opaque identifier for user-defined types.
 *
 * TypeID is optional: set to 0 if unused.
 */
typedef uint64_t TypeID;

/** Forward declaration of the Any struct. */
typedef struct Any Any;

/**
 * @brief A constant null instance of Any.
 *
 * Represents an "empty" Any, used for comparisons or null checks.
 */
extern const Any *const Any_null;

/**
 * @brief Create a new Any object.
 *
 * @param type Type identifier (optional).
 * @param size Size of the stored type in bytes.
 * @return Pointer to the newly allocated Any, or NULL on failure.
 */
Any *Any_Create(TypeID type, size_t size);

/**
 * @brief Reset an Any object, clearing its value.
 *
 * Frees the stored data and resets type and size.
 *
 * @param self Pointer to the Any object.
 */
void Any_Reset(Any *self);

/**
 * @brief Set the value of an Any object.
 *
 * Copies the value into the Any. If the size differs, the storage is reallocated.
 *
 * @param self  Pointer to the Any object.
 * @param type  Type identifier (optional).
 * @param value Pointer to the data to copy.
 * @param size  Size of the data in bytes.
 * @return true on success and false otherwise
 */
bool Any_Set(Any *self, TypeID type, const void *value, size_t size);

/**
 * @brief Get a pointer to the stored value.
 *
 * The pointer is owned by the Any object and must not be freed.
 *
 * @param self Pointer to the Any object.
 * @return Pointer to the stored value, or NULL if empty.
 */
const void *Any_Get(const Any *self);

/**
 * @brief Get the size of the stored value.
 *
 * @param self Pointer to the Any object.
 * @return Size in bytes, or 0 if empty.
 */
size_t Any_GetSize(const Any *self);

/**
 * @brief Get the type identifier of the stored value.
 *
 * @param self Pointer to the Any object.
 * @return TypeID, or 0 if unused.
 */
TypeID Any_GetType(const Any *self);

/**
 * @brief Compare two Any objects for equality.
 *
 * Equality = same type, same size, and identical data.
 *
 * @param self  Pointer to the first Any.
 * @param other Pointer to the second Any.
 * @return true if equal, false otherwise.
 */
bool Any_Equals(const Any *self, const Any *other);

/**
 * @brief Check if an Any object is null (unusable).
 *
 * This means the pointer is NULL, or equals Any_null.
 *
 * @param self Pointer to the Any object.
 * @return true if null, false otherwise.
 */
bool Any_IsNull(const Any *self);

/**
 * @brief Check if an Any object currently has a stored value.
 *
 * @param self Pointer to the Any object.
 * @return true if a value is stored, false otherwise.
 */
bool Any_HasValue(const Any *self);

/**
 * @brief Create a deep copy of an Any object.
 *
 * @param self Pointer to the Any object.
 * @return A new Any object with copied data, or NULL on failure.
 */
Any *Any_Copy(const Any *self);

/**
 * @brief Move the contents of one Any into another.
 *
 * Transfers ownership: dest takes the data, src becomes empty.
 *
 * @param dest Pointer to destination Any.
 * @param src  Pointer to source Any.
 */
void Any_Move(Any *dest, Any *src);

/**
 * @brief Swap the contents of two Any objects.
 *
 * Shallow swap of type, size, pointer, and state.
 *
 * @param self  Pointer to the first Any.
 * @param other Pointer to the second Any.
 */
void Any_Swap(Any *self, Any *other);

/**
 * @brief Destroy an Any object and free its memory.
 *
 * After this, the pointer must not be used again.
 *
 * @param self Pointer to the Any object.
 */
void Any_Destroy(Any *self);

#endif // ANY_H
