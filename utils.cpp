#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <ctime>

#include "utils.h"



time_t str_to_time(const char* str) {
    tm tim;
    sscanf(str, "%d-%d-%d %d:%d:%d", &tim.tm_year, &tim.tm_mon, &tim.tm_mday, &tim.tm_hour, &tim.tm_min, &tim.tm_sec);
    tim.tm_year -= 1900;
    tim.tm_mon--;
    tim.tm_isdst = 0;
    return mktime(&tim);
}

int time_to_str(time_t time, char* buf, int size) {
    struct tm* result = localtime((time_t*)&time);
    
    result->tm_year = result->tm_year + 1900;
    result->tm_mon = result->tm_mon + 1;
    
    return snprintf(buf, size, "%04d-%02d-%02d %02d:%02d:%02d",
                    result->tm_year,
                    result->tm_mon,
                    result->tm_mday,
                    result->tm_hour,
                    result->tm_min,
                    result->tm_sec);
}

static char time_str_buf[32];
char* time_to_str(time_t time) {
    time_to_str(time, time_str_buf, sizeof(time_str_buf));
    return time_str_buf;
}

int now_to_str(char* buf, int size) {
    return time_to_str(time(0), buf, size);
}

char* now_to_str() {
    now_to_str(time_str_buf, sizeof(time_str_buf));
    return time_str_buf;
}


static FILE* log_file = NULL;
void open_log_file(const char* file_path) {
    log_file = fopen(file_path, "w");
    if (log_file == NULL) {
        printf("open %s fail\n", file_path);
    }
    log_ln("log start @ %s", now_to_str());
}

void close_log_file() {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_str(const char* str) {
    printf("%s", str);
    if (log_file != NULL) {
        fprintf(log_file, "%s", str);
    }
}

static char printf_buf[1024];
void log(const char* pcString, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, pcString);
    
    vsnprintf(printf_buf, sizeof(printf_buf), pcString, arg_ptr);
    
    log_str(printf_buf);
    
    va_end(arg_ptr);
}

void log_ln(const char* pcString, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, pcString);
    
    vsnprintf(printf_buf, sizeof(printf_buf), pcString, arg_ptr);
    
    log_str(printf_buf);
    log_str("\n");
    
    va_end(arg_ptr);
}


void log_hex(void* data, int len) {
    unsigned char* byte_data = (unsigned char*)data;
    for (int i = 0; i < len; i++) {
        log("%02X ", byte_data[i]);
        if ((i + 1) % 8 == 0) {
            log_str(" ");
        }
        
        if ((i + 1) % 16 == 0) {
            log_str("\n");
        }
    }
    log_str("\n");
}


