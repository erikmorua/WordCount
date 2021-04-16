#pragma once
#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>
#include <ctype.h>
#include <wctype.h>
void* progress_monitor(void* data);
long int wordcount(const char* fname);