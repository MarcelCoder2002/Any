# Any Library

A C library that implements a generic `Any` type similar to the `std::any` concept from C++.

## Features

### Basic Features
- **Generic storage**: Can store any type of data
- **Automatic memory management**: Automatic allocation and deallocation
- **Simple API**: Convenient macros to ease usage
- **Type safety**: Nullity and validity checks

## Installation

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

### Basic Example

```c
#include "any.h"

int main() {
    // Creation and initialization
    Any_DeclareAny(myVar, int, 42);
    
    // Value retrieval
    int value = Any_GetValue(myVar, int);
    printf("Value: %d\n", value);
    
    // Modification
    Any_SetValue(myVar, int, 100);
    
    // Cleanup
    Any_Destroy(myVar);
    return 0;
}
```

### New Features

#### Deep Copying
```c
Any_DeclareAny(original, double, 3.14159);
Any *copy = Any_Copy(original);
// copy is an independent copy of original
Any_Destroy(original);
Any_Destroy(copy);
```

#### Move Semantics
```c
Any_DeclareAny(source, int, 42);
Any *dest = Any_Create(0, sizeof(int));
Any_Move(dest, source);
// source is now empty, dest contains the value
```

#### Swapping
```c
Any_DeclareAny(a, int, 10);
Any_DeclareAny(b, int, 20);
Any_Swap(a, b);
// a now contains 20, b contains 10
```

#### String Conversion
```c
Any_DeclareAny(myVar, int, 42);
char *str = Any_ToString(myVar);
printf("Representation: %s\n", str);
free(str); // Don't forget to free memory
```

#### Value Verification
```c
Any *myVar = Any_Create(0, sizeof(int));
if (Any_HasValue(myVar)) {
    printf("Object has a value\n");
} else {
    printf("Object is empty\n");
}
```

### Supported Types

- Primitive types: `int`, `float`, `double`, `char`, `long`, `long long`
- Character strings: `char*`, `wchar_t*`
- Arrays and custom structures
- Arbitrary binary data

### Useful Macros

- `Any_Declare(variable, typeID, type, value)`: Declaration and initialization
- `Any_DeclareAny(variable, type, value)`: Declaration without typeID
- `Any_GetValue(self, type)`: Typed retrieval
- `Any_GetStringValue(self, type)`: String (char *) retrieval
- `Any_GetWStringValue(self, type)`: WString (wchar_t *) retrieval
- `Any_SetValue(self, type, value)`: Typed storage
- `Any_SetStringValue(self, value)`: String storage
- `Any_SetArrayValue(self, type, length, value)`: Array storage

## Testing

### Quick Test
Run all tests with a single command:

```bash
./run_tests.sh
```

### Manual Testing

#### Unit Tests
Run comprehensive unit tests:

```bash
make
./tests/test_any
```

#### Stress Tests
Run stress tests to validate performance and memory management:

```bash
make
./tests/stress_test
```

#### Example
Run the example program:

```bash
make
./examples/example
```

### Test Coverage

The test suite covers:

- **Basic Operations**: Creation, destruction, value setting/getting
- **Memory Management**: Allocation, deallocation, reallocation
- **Array Operations**: 1D and 2D arrays, literal arrays, typed arrays
- **String Operations**: Regular strings, wide strings, typed strings
- **Move/Swap Operations**: Efficient data transfer between objects
- **Error Handling**: NULL pointer handling, invalid operations
- **Stress Testing**: 10,000+ iterations for memory management validation
- **Edge Cases**: Empty objects, type mismatches, boundary conditions

## Complete Example

See `examples/main.c` for a complete example demonstrating all features.

## Limitations

- No robust error handling (functions return NULL on failure)
- Strings are copied, not referenced

## API Reference

### Main Functions

```c
Any *Any_Create(TypeID type, size_t size);
void Any_Destroy(Any *self);
bool Any_Set(Any *self, TypeID type, const void *value, size_t size);
const void *Any_Get(const Any *self);
bool Any_Equals(const Any *self, const Any *other);
bool Any_IsNull(const Any *self);
bool Any_HasValue(const Any *self);
size_t Any_GetSize(const Any *self);
TypeID Any_GetType(const Any *self);
Any *Any_Copy(const Any *self);
void Any_Move(Any *dest, Any *src);
void Any_Swap(Any *self, Any *other);
```

## License

This project is licensed under the MIT License.