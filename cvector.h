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

typedef unsigned long   (*fp_cv_rul)(cvector*);
typedef cv_byte         (*fp_cv_rbyte)(cvector*);
typedef cv_byte         (*fp_cv_rbyte_i)(cvector*, unsigned int);
typedef cv_byte         (*fp_cv_rbyte_uc)(cvector*, const cv_byte*);
typedef cv_byte         (*fp_cv_rbyte_ui_uc)(cvector*, unsigned int, const cv_byte*);
typedef cv_byte*        (*fp_cv_ruc_ui)(cvector*, unsigned int);
typedef void            (*fp_cv)(cvector*);

struct cvector_s
{
    // public:
    /// @brief Размер данных (всех элементов) в байтах
    fp_cv_rul           size;

    /// @brief Количество элементов в векторе
    fp_cv_rul           count;

    /// @brief Размер выделенной памяти под вектор в байтах
    /// @return 0 - память не выделена
    /// @return size - размер в байтах
    fp_cv_rul           capacity;

    /// @brief Проверка на наличие элементов в векторе
    fp_cv_rbyte         empty;

    /// @brief Добавление элемента в конец
    /// @return 0 - ошибка добавления
    /// @return 1 - успешное добавление
    fp_cv_rbyte_uc      push;

    /// @brief Удаление элемента из конца
    /// @return 0 - ошибка удаления
    /// @return 1 - успешное удаление
    fp_cv_rbyte         pop;

    /// @brief Замена элемента по индексу
    /// @return 0 - ошибка замены
    /// @return 1 - успешная замена
    fp_cv_rbyte_ui_uc   replace;

    /// @brief Удаление элемента по индексу O(log n)
    /// Используем указательную арифметику,
    /// т.к. она быстрее для большинства компиляторов
    fp_cv_rbyte_i       remove;

    /// @brief Чтение элемента по индексу O(1)
    /// @return ptr - возвращает указатель на начало элемента
    /// @return NULL - если элемент недоступен
    fp_cv_ruc_ui        at;

    /// @brief Удаление всех байтов и реалокация до одного байта
    /// После вызова этой функции capacity, size и count равны нулю
    fp_cv               clear;

    /// @brief Освобождение памяти (удаление вектора из памяти)
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
