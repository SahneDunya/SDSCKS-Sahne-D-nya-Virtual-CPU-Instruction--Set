#include "target_info.h"
#include <string.h>

const char *get_target_triple() {
    return SDSCKS_TARGET_TRIPLE;
}

const char *get_data_layout() {
    return SDSCKS_DATA_LAYOUT;
}