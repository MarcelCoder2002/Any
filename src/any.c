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
};

static const Any Any_null_value = {0};
const Any *const Any_null = &Any_null_value;

Any *Any_Create(const TypeID type, const size_t size) {
    Any *self = malloc(sizeof(Any));
    if (self != NULL) {
        self->type = type;
        self->size = size;
        self->value = calloc(1, size);
        self->has_value = false;
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
        free(self->value);
        self->value = NULL;
        self->has_value = false;
        self->size = 0;
        self->type = 0;
    }
}

bool Any_Set(Any *self, const TypeID type, const void *value, const size_t size) {
    if (!value || Any_IsNull(self)) return false;
    if (self->size != size) {
        void *tmp = realloc(self->value, size);
        if (!tmp) return false;
        self->size = size;
        self->value = tmp;
    }

    memcpy(self->value, value, self->size);
    self->type = type;
    self->has_value = true;
    return true;
}

const void *Any_Get(const Any *self) {
    return Any_IsNull(self) ? NULL : self->value;
}

size_t Any_GetSize(const Any *self) {
    return Any_IsNull(self) ? 0 : self->size;
}

TypeID Any_GetType(const Any *self) {
    return Any_IsNull(self) ? 0 : self->type;
}

bool Any_HasValue(const Any *self) {
    return Any_IsNull(self) ? false : self->has_value;
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
    return !self || self == Any_null || (self->type == 0 && self->size == 0 && self->value == NULL);
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
    if (!dest || Any_IsNull(src)) return;

    // Free the old value of dest if it exists
    if (dest->value) {
        free(dest->value);
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
    if (!self || !other) return;

    // Échanger les champs
    const TypeID temp_type = self->type;
    const size_t temp_size = self->size;
    void *temp_value = self->value;
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
    if (self->value) free(self->value);
    self->type = 0;
    self->size = 0;
    self->value = NULL;
    free(self);
}
