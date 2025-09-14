#pragma once
#include "pb.h"
#include <stdint.h>
#include "common/wrapper/client_info.h"
#include "common/list.h"

list client_list_init(void);
void client_list_strcpy(common_client* dst, list* client_list);
