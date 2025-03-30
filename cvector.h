/*
*	С++ - подобная библиотека вектора
* 1. Написать тесты для библиотеки
*/

#ifndef _CVECTOR_H_
#define _CVECTOR_H_

#include <stdlib.h>

typedef struct cprivate_s cprivate_t;
typedef struct cvector_s cvector;
typedef unsigned char cv_byte;

typedef int (*fp_cv_rint)(cvector*);
typedef int (*fp_cv_rint_i)(cvector*, unsigned int);
typedef int (*fp_cv_rint_uc)(cvector*, const cv_byte*);
typedef int (*fp_cv_rint_ui_uc)(cvector*, unsigned int, const cv_byte*);
typedef cv_byte* (*fp_cv_ruc_ui)(cvector*, unsigned int);
typedef void (*fp_cv)(cvector*);

struct cvector_s
{
    // public:
    fp_cv_rint          size;
    fp_cv_rint          count;
    fp_cv_rint          capacity;
    fp_cv_rint          empty;
    fp_cv_rint_uc       push;
    fp_cv_rint          pop;
    fp_cv_rint_ui_uc    replace;
    fp_cv_rint_i        remove;
    fp_cv_ruc_ui        at;
    fp_cv               clear;
    fp_cv               free;

    // private:
    cprivate_t*         priv;
};



/// @brief выделение памяти под массив данных размером max_item*item_size
/// @param max_item -  максимально хранящееся количество элементов,
/// если max_item = 0, то динамический размер.
/// @param item_size - размер элемента в байтах
cvector* cvector_new(int max_item, size_t item_size);

#endif
