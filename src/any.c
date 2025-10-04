/**
 * @file any.c
 * @brief Implementation of the Any generic type container library
 * @author Marcel Raymond Goumou
 * @date 2025-09-25
 */

#include "any.h"

#include <stdlib.h>
#include <string.h>

struct Any {
    TypeID type;
    size_t size;
    void *value;
    bool has_value;
    bool is_user_defined;
};

static const Any Any_null_value = {0};
const Any *const Any_null = &Any_null_value;

#ifdef ANY_TRACK_ALLOCATION_COUNT

#include <stdio.h>

static size_t g_any_alloc_count = 0;

void* Any_malloc(const size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
        g_any_alloc_count++;
    }
    return ptr;
}

void* Any_calloc(const size_t num, const size_t size) {
    void* ptr = calloc(num, size);
    if (ptr) {
        g_any_alloc_count++;
    }
    return ptr;
}

void* Any_realloc(void* old_ptr, const size_t new_size) {
    void* ptr = realloc(old_ptr, new_size);
    if (ptr) {
        if (old_ptr == NULL)
            g_any_alloc_count++;
    }
    return ptr;
}

void Any_free(void* ptr) {
    if (ptr) {
        g_any_alloc_count--;
        free(ptr);
    }
}

void Any_MemoryReport(void) {
    if (g_any_alloc_count != 0) {
        printf("[Any] ⚠️ There are still %zu allocations left (memory leak)\n",
               g_any_alloc_count);
    } else {
        printf("[Any] ✅ Memory is clean (no memory leak) !\n");
    }
}

int Any_AllocationCount(void) {
    return (int)g_any_alloc_count;
}

#else

void Any_MemoryReport(void) {

}

int Any_AllocationCount(void) {
    return -1;
}

#endif //ANY_TRACK_ALLOCATION_COUNT

Any *Any_Init(Any *self, const TypeID type, const size_t size) {
    if (self != NULL) {
        self->type = type;
        self->size = size;
        self->is_user_defined = true;
        self->value = Any_calloc(1, size);
        if (!self->value) {
            Any_Destroy(self);
            return NULL;
        }
    }
    return self;
}

Any *Any_Create(const TypeID type, const size_t size) {
    Any *self = Any_calloc(1, sizeof(Any));
    if (self != NULL) {
        self->type = type;
        self->size = size;
        self->is_user_defined = false;
        self->value = Any_calloc(1, size);
        if (!self->value) {
            Any_Destroy(self);
            return NULL;
        }
    }
    return self;
}

void Any_Reset(Any *self) {
    if (Any_IsNull(self)) return;
    if (self->has_value) {
        Any_free(self->value);
        self->value = NULL;
        self->has_value = false;
        self->size = 0;
        self->type = 0;
    }
}

Any *Any_Set(Any *self, const TypeID type, const void *value, const size_t size) {
    if (!value || Any_IsNull(self)) return NULL;
    if (self->size != size) {
        void *tmp = Any_realloc(self->value, size);
        if (!tmp) return NULL;
        self->size = size;
        self->value = tmp;
    }

    memcpy(self->value, value, self->size);
    self->type = type;
    self->has_value = true;
    return self;
}

const void *Any_Get(const Any *self) {
    return Any_IsNull(self) ? NULL : self->value;
}

size_t Any_Size(const Any *self) {
    return Any_IsNull(self) ? 0 : self->size;
}

TypeID Any_Type(const Any *self) {
    return Any_IsNull(self) ? 0 : self->type;
}

bool Any_HasValue(const Any *self) {
    return Any_IsNull(self) ? false : self->has_value;
}

bool Any_IsEmpty(const Any *self) {
    return self && (!self->has_value || self->size == 0 || self == Any_null);
}

bool Any_IsUserDefined(const Any *self) {
    return Any_IsNull(self) ? false : self->is_user_defined;
}

bool Any_Equals(const Any *self, const Any *other) {
    if (!self || !other) return false;
    if (self == other) return true;
    return (
        self->has_value == other->has_value &&
        self->type == other->type &&
        self->size == other->size &&
        memcmp(self->value, other->value, self->size) == 0
    );
}

bool Any_IsNull(const Any *self) {
    return !self || self == Any_null;
}

Any *Any_Copy(const Any *self) {
    if (Any_IsNull(self) || !self->has_value) {
        return NULL;
    }

    Any *copy = Any_Create(self->type, self->size);
    if (!copy) {
        return NULL;
    }

    memcpy(copy->value, self->value, self->size);
    copy->has_value = true;
    return copy;
}

void Any_Move(Any *dest, Any *src) {
    if (Any_IsNull(dest) || Any_IsNull(src)) return;

    // Free the old value of dest if it exists
    if (dest->value) {
        Any_free(dest->value);
    }

    // Move data from src to dest
    dest->type = src->type;
    dest->size = src->size;
    dest->value = src->value;
    dest->has_value = src->has_value;

    // Clear src
    src->type = 0;
    src->size = 0;
    src->value = NULL;
    src->has_value = false;
}

void Any_Swap(Any *self, Any *other) {
    if (Any_IsNull(self) || Any_IsNull(other)) return;

    // Swap the fields
    const TypeID temp_type = self->type;
    const size_t temp_size = self->size;
    void *const temp_value = self->value;
    const bool temp_has_value = self->has_value;

    self->type = other->type;
    self->size = other->size;
    self->value = other->value;
    self->has_value = other->has_value;

    other->type = temp_type;
    other->size = temp_size;
    other->value = temp_value;
    other->has_value = temp_has_value;
}

void Any_Destroy(Any *self) {
    if (Any_IsNull(self)) return;
    if (self->value) Any_free(self->value);
    self->type = 0;
    self->size = 0;
    self->value = NULL;
    if (!self->is_user_defined) Any_free(self);
}
