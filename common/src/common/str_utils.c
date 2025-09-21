#include "str_utils.h"
#include "common/log.h"
#include <stddef.h>
#include <string.h>

void str_remove_nl(char* target) {
    size_t nl_pos = strcspn(target, "\n");
    if (nl_pos == 0) { return; }
    target[nl_pos] = '\0';
}
