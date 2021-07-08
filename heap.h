#ifndef	HEAP_H
#define	HEAP_H

#include <stdint.h>

void heap_init(uint32_t* heap_buf, uint32_t len);
void alloc_test();

void* heap_alloc(uint32_t size);
void heap_free(void* p);

#endif
