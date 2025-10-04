#include "any.h"
#include <stdio.h>

int main(void) {
    printf("\n=== Leak demonstration (tracking-aware) ===\n");

    // Intentionally leak an Any allocation when tracking is enabled
    Any *leaky = Any_Create(0, sizeof(int));
    if (leaky) {
        int v = 123;
        Any_Set(leaky, 0, &v, sizeof(v));
        // Deliberately do NOT destroy: Any_Destroy(leaky);
    }

    Any_MemoryReport();
#ifdef ANY_TRACK_ALLOCATION_COUNT
    const int leaks = Any_AllocationCount();
    if (leaks <= 0) {
        printf("Expected a positive leak count in demo, got %d.\n", leaks);
        return 1; // fail if not detected under tracking
    }
    printf("Leak demo: detected %d outstanding allocation(s) as expected.\n", leaks);
    return 0; // success: detection works
#else
    printf("Allocation tracking disabled; leak demo is a no-op.\n");
    return 0;
#endif
}
