#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "heap.h"
#include "utils.h"

// #define DBP(fmt,arg...)  debug.printf(fmt,##arg)
// #define DBPLN(fmt,arg...)  debug.printf_line(fmt,##arg)

// #define DBP     log
// #define DBPLN   log_ln
#define DBP     
#define DBPLN   

#define	M_IDLE	0
#define M_BUSY	1

typedef struct Node {
    uint32_t type;
    uint32_t size;
    struct Node* prev;
    struct Node* next;
} Heap_node;

static uint32_t heap_max;
static Heap_node* head_node = NULL;

void alloc_test() {
    uint8_t* p[5];
    
    p[0] = (uint8_t*)heap_alloc(10);
    p[1] = (uint8_t*)heap_alloc(99);
    p[2] = (uint8_t*)heap_alloc(2000);
    p[4] = (uint8_t*)heap_alloc(200);
    
    heap_free(p[1]);
    heap_free(p[0]);
    p[0] = (uint8_t*)heap_alloc(104);
    /**/
    heap_free(p[2]);
    heap_free(p[0]);
    
    
    p[3] = (uint8_t*)heap_alloc(2104);
    heap_free(p[3]);
    heap_free(p[4]);
}


void heap_init(uint32_t* heap_buf, uint32_t len) {
    DBPLN("heap_init buf len %d", len);
    
    head_node = (Heap_node*)heap_buf;
    head_node->type = M_IDLE;
    head_node->size = len - sizeof(Heap_node);
    head_node->prev = NULL;
    head_node->next = NULL;
    
    heap_max = 0;
}

void heap_stat() {
    DBPLN("block_list:");
    for (Heap_node* mem_block = head_node; mem_block != NULL;) {	// 遍历内存块
        uint32_t addr = (uint32_t)mem_block - (uint32_t)head_node;
        DBPLN("% 5d : % 5d %s", addr, mem_block->size, mem_block->type == M_IDLE ? "idle" : "busy");
        
        if (mem_block->next == NULL) {
            DBPLN("heap tail %d max %d", addr, heap_max);
            break;
        } else {
            mem_block = mem_block->next;
        }
    }
    DBPLN("");
}

/*

*/
void* heap_alloc(uint32_t alloc_size) {
    if (head_node == NULL) {
        DBPLN("head_node NULL !!!!!!!!!!!!!!!!!!!!!!");
        return NULL;
    }
    
    Heap_node* mem_block = head_node;
    alloc_size = (alloc_size + 0x03) & 0xFFFFFFFC;	// 保证分配内存4字节对齐
    
    DBPLN("heap_alloc : %d", alloc_size);
    
    do {	// 遍历内存块
        // 容量够大 且为空闲
        if ((mem_block->size >= alloc_size) && (mem_block->type == M_IDLE)) {
            break;
        }
        
        mem_block = mem_block->next;
    } while (mem_block != NULL);
    
    // 貌似不用整理  使用前后空闲合并的free算法 可能不会产生两个相邻的空闲内存块 待证明
    if (mem_block == NULL) {	// 未找到足够大的空闲内存块 整理堆空间 合并连续空闲内存块
        mem_block = head_node;
        
        do {	// 遍历内存块
            if (mem_block->type == M_IDLE) {	// 当前空闲
                Heap_node* next_block = mem_block->next;
                
                while ((mem_block->next != NULL) && (next_block->type == M_IDLE)) {	// 下一内存块非空且空闲
                    next_block = mem_block->next;
                    mem_block->size += next_block->size + sizeof(Heap_node);
                    next_block->next->prev = mem_block;
                    mem_block->next = next_block->next;
                }
                
                if (mem_block->size >= alloc_size) {
                    break;
                }
            }
            
            mem_block = mem_block->next;	// 检查下一内存块
        } while (mem_block != NULL);
    }
    
    if (mem_block != NULL) {	// 找到足够大的空闲内存块 标记为已用
        mem_block->type = M_BUSY;
        
        // 原内存块还足够容纳另一内存块
        if (mem_block->size >= (alloc_size + (sizeof(Heap_node) + sizeof(uint32_t)))) {
            Heap_node* remain = (Heap_node*)((uint32_t)mem_block + sizeof(Heap_node) + alloc_size);
            remain->type = M_IDLE;
            remain->size = mem_block->size - alloc_size - sizeof(Heap_node);
            remain->prev = mem_block;
            remain->next = mem_block->next;
            
            if (remain->next != NULL) {
                remain->next->prev = remain;
            }
            
            mem_block->size = alloc_size;
            mem_block->next = remain;
            
        }
        
        uint32_t end_addr = (uint32_t)mem_block + sizeof(Heap_node) + mem_block->size - (uint32_t)head_node;
        if (end_addr > heap_max) {
            heap_max = end_addr;
        }
        
        DBPLN("new block addr %d len %d", (uint32_t)mem_block - (uint32_t)head_node, alloc_size);
        // heap_stat();
        return (void*)((uint32_t)mem_block + sizeof(Heap_node));
    } else {
        DBPLN("heap FULL !!!!!!!!!!!!!!!!!!!!!!");
    }
    
    return NULL;
}

void heap_free(void* mem) {
    if (mem == NULL) {
        return;
    }
    
    Heap_node* cur_block = (Heap_node*)((uint32_t)mem - sizeof(Heap_node));
    DBPLN("heap_free %d", (uint32_t)cur_block - (uint32_t)head_node);
    
    if (cur_block->type != M_BUSY) {
        DBPLN("free repeat !!!!!!!!!!!!!!!!!!!!!!");
        return;
    }
    
    cur_block->type = M_IDLE;
    
    // 下一内存块非空 空闲
    if (cur_block->next != NULL) {
        DBP("next ");
        Heap_node* next_block = cur_block->next;
        
        if (next_block->type == M_IDLE) {
            DBPLN("idle");
            // 二者合并
            cur_block->size += next_block->size + sizeof(Heap_node);
            cur_block->next = next_block->next;
            
            if (next_block->next != NULL) {
                next_block->next->prev = cur_block;
            }
        } else {
            DBPLN("busy");
        }
    }
    
    // 上一内存块非空 空闲
    if (cur_block->prev != NULL) {
        DBP("prev ");
        
        if (cur_block->prev->type == M_IDLE) {
            DBPLN("idle");
            // 二者合并
            cur_block->prev->size += cur_block->size + sizeof(Heap_node);
            cur_block->prev->next = cur_block->next;
            
            if (cur_block->next != NULL) {
                cur_block->next->prev = cur_block->prev;
            }
        } else {
            DBPLN("busy");
        }
    }
    
    // heap_stat();
}

