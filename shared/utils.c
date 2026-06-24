#include "utils.h"

bool is_same_content_memory(void* a, void* b, uint32_t n_bytes) {
    uint8_t* aa = (uint8_t*)a;
    uint8_t* bb = (uint8_t*)b;
    for (uint32_t i=0; i<n_bytes; i++) {
        if (aa[i] != bb[i]) {
            return false;
        }
    }
    
    return true;
}