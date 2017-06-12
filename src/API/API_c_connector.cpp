//
// Created by Haoxiangpeng on 6/11/2017.
//

#include "API_c_connector.h"
#include "API.h"

#ifdef __cplusplus
extern "C" {
#endif

static API *api_instance = NULL;

void lazy_api() {
    if (api_instance == NULL) {
        api_instance = new API();
    }
}

void create_index(char *a, char *b, char *c) {
    lazy_api();
    api_instance->create_index(a, b, c);
}

#ifdef __cplusplus
}
#endif
