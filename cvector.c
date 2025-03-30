#include "cvector.h"
#include <string.h>
#include <stdio.h>

// приватные функции, но публично вызываемые по указатель из вне:
int         cv_size     (cvector* v);
int         cv_count    (cvector* v);
int         cv_capacity (cvector* v);
int         cv_isempty  (cvector* v);
int         cv_push     (cvector* v, const cv_byte* element);
int         cv_pop      (cvector* v);
int         cv_replace  (cvector* v, unsigned int index, const cv_byte* element);
int         cv_remove   (cvector* v, unsigned int index);
void        cv_clear    (cvector* v);
void        cv_free     (cvector* v);
cv_byte*    cv_at       (cvector* v, unsigned int index);

// приватные функции, доступные только в этом файле:
int p_cv_slow_realloc(cvector *v);

struct cprivate_s
{
    int p_max_count;    // максимальное количество элементов в массиве data
    int p_item_size;    // размер элемента в байтах
    int p_size_in;      // положение массива data (количество элементов в векторе)
    int p_alloc_size;   // реальный размер выделеной памяти в байтах
    cv_byte* p_data;    // указатель на массив с данными
};

/// @brief выделение памяти для вектора
cvector* cvector_new(int max_item, size_t item_size) {

    cvector* v = (cvector*)malloc(sizeof(cvector));

    if(v == NULL)
        return NULL;


    // create private:
    v->priv = (cprivate_t*)malloc(sizeof(cprivate_t));
    if(v->priv == NULL)
    {
        free(v);
        return NULL;
    }

    // private initial:
    v->priv->p_size_in = 0;
    v->priv->p_max_count = max_item;
    v->priv->p_item_size = item_size;
    v->priv->p_data = (cv_byte*)calloc(max_item, item_size);
    if(v->priv->p_data == NULL)
    {
        free(v->priv);
        free(v);
        return NULL;
    }

    // public initial:
    v->size     = cv_size;
    v->count    = cv_count;
    v->capacity = cv_capacity;
    v->empty    = cv_isempty;
    v->push     = cv_push;
    v->pop      = cv_pop;
    v->replace  = cv_replace;
    v->remove   = cv_remove;
    v->at       = cv_at;
    v->clear    = cv_clear;
    v->free     = cv_free;


    return v;
}

/// @brief истина - если вектор пуст
int cv_isempty(cvector *v)
{

    if(v != NULL)
    {
        if(v->priv->p_size_in > 0)
            return 0;
    }

    return 1;
}

/// @brief размер данных (всех элементов) в байтах
int cv_size(cvector *v)
{
    int ret = 0;
    if(v != NULL)
    {
        ret = &v->priv->p_data[v->priv->p_size_in*v->priv->p_item_size]
              - v->priv->p_data;
    }
    return ret;
}

/// @brief количество элементов в векторе
int cv_count(cvector *v)
{
    if(v != NULL)
        return v->priv->p_size_in;

    return 0;
}

/// @brief размер выделенной памяти под вектор в байтах
int cv_capacity(cvector *v)
{
    if(v != NULL)
        return v->priv->p_alloc_size;

    return 0;
}

/// @brief Добавление элемента в конец
int cv_push(cvector *v, const cv_byte* element)
{
    if(v != NULL)
    {
        // если динамический вектор
        if(v->priv->p_max_count == 0)
        {
            // требуемый размер
            unsigned int size = (v->priv->p_size_in+1) * v->priv->p_item_size;
            if(size > v->priv->p_alloc_size)
            {
                // увеличиваем размер массива в 2 раза для уменьшения реаллокаций
                v->priv->p_alloc_size = size*2;
                cv_byte* rv = (cv_byte*)realloc(v->priv->p_data, v->priv->p_alloc_size);
                if(rv == NULL) return 0;
                v->priv->p_data = rv;
            }
        }
        else
        {
            if(v->priv->p_size_in+1 > v->priv->p_max_count)
                return 0;
            v->priv->p_alloc_size = v->priv->p_max_count * v->priv->p_item_size;
        }

        memcpy(v->priv->p_data+(v->priv->p_size_in*v->priv->p_item_size),
               element, v->priv->p_item_size);

        v->priv->p_size_in += 1;
        return 1;
    }

    return 0;
}

/// @brief Удаление элемента из конца
int cv_pop(cvector *v)
{
    if(v != NULL && v->priv->p_data != NULL && v->priv->p_size_in > 0)
    {
        v->priv->p_size_in -= 1;

        memset(v->priv->p_data + (v->priv->p_size_in * v->priv->p_item_size),
               0, v->priv->p_item_size);

        // если динамический вектор
        if(v->priv->p_max_count == 0)
        {
            // освобождать память (realloc) когда накопился большой хвост,
            p_cv_slow_realloc(v);
        }
        else
            v->priv->p_alloc_size = v->priv->p_max_count * v->priv->p_item_size;

        return 1;
    }

    return 0;
}

/// @brief Замена элемента по индексу
int cv_replace(cvector *v, unsigned int index, const cv_byte* element)
{
    if(v != NULL)
    {
        if(index < v->priv->p_size_in)
        {
            memcpy(v->priv->p_data + (index * v->priv->p_item_size),
                   element, v->priv->p_item_size);
            return 1;
        }
    }
    return 0;
}

/// @brief Удаление элемента по индексу O(log n)
/// Используем указательную арифметику, т.к. она быстрее для большинства компиляторов
int cv_remove(cvector *v, unsigned int index)
{
    if(v == NULL || v->priv->p_data == NULL || index > v->priv->p_size_in-1 || v->priv->p_size_in < 1)
        return 0;

    int i = 0;

    // free
    memset(v->priv->p_data + (index * v->priv->p_item_size),
           0,v->priv->p_item_size);

    // сдвигаем влево элементы, которые были справа от удаленного элемента,
    // закрываем дыру
    for(i=index; i<v->priv->p_size_in-1; i++)
        memcpy(v->priv->p_data + (i * v->priv->p_item_size),
               v->priv->p_data + ((i+1) * v->priv->p_item_size),
               v->priv->p_item_size);

    // resize
    v->priv->p_size_in -= 1;

    // если динамический вектор
    if(v->priv->p_max_count == 0)
    {
        // освобождать память (realloc) когда накопился большой хвост,
        p_cv_slow_realloc(v);
    }
    else
        v->priv->p_alloc_size = v->priv->p_max_count * v->priv->p_item_size;

    return 1;
}

/// @brief "медленное" перевыделение памяти,
/// используется в функциях pop и remove
int p_cv_slow_realloc(cvector *v)
{
    // Хвост освобождаем (realloc) если текущий размер резерва
    // превысил требуемый на 50%, например:
    // Пусть размер одного элемента - 1 байт,
    // после ряда удалений требуется 25 байт для данных,
    // всего выделено 62 байта (данные+резерв),
    // для 25 байт данных достаточно 25 байт резерва,
    // но реалокацию выполним размер хвоста будет больше,
    // чем 25+(25/2) байт.
    // Т.е. хвост = 62(всего выделено) - 25(данные),
    // если хвост >= 25(требуемый резерв)+(25/2), то реаллоцируем.
    unsigned int size = v->priv->p_size_in * v->priv->p_item_size; // требуемый размер

    // (size>0) - при удалении последнего элемента память не перевыделяем,
    // остается висеть количество байт для одного элемента + резерв.
    if( size>0 && v->priv->p_alloc_size - size >= size+(size/2) )
    {
        v->priv->p_alloc_size = size*2;
        cv_byte* rv = (cv_byte*)realloc(v->priv->p_data, v->priv->p_alloc_size);
        if(rv == NULL) return 0;
        v->priv->p_data = rv;
    }

    return 1;
}

/// @brief чтение элемента по индексу O(1)
/// @return возвращает указатель на начало элемента
cv_byte* cv_at(cvector* v, unsigned int index)
{
    if(v == NULL || index > v->priv->p_size_in-1 || v->priv->p_size_in <= 0)
        return NULL;

    return &v->priv->p_data[index * v->priv->p_item_size];
}

void cv_clear(cvector* v)
{
    if(v != NULL)
    {
        memset(v->priv->p_data, 0, v->priv->p_size_in * v->priv->p_item_size);

        // освобождение памяти, если динамический вектор
        // оставляем в памяти 1 байт, чтобы обращение к массиву было валидным
        if(v->priv->p_max_count == 0)
            v->priv->p_data = (cv_byte*)realloc(v->priv->p_data, 1);

        v->priv->p_alloc_size = 0;
        v->priv->p_size_in = 0;
    }
}

void cv_free(cvector* v)
{
    if(v != NULL)
    {
        if(v->priv->p_data != NULL)
        {
            free(v->priv->p_data);
            v->priv->p_data =NULL;
        }
        v->priv->p_max_count = 0;
        v->priv->p_size_in = 0;
        free(v);
    }
}
