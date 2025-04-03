# cvector

![Alt Text](https://github.com/ivanMoskalevDev/cvector/blob/main/target/resources/logo.png)

#
# RU:
Простая векторная библиотека C подобная объекту vector 
из стандартной библиотеки C++,с удобным интерфейсом с поддержкой всех 
других типов и пользовательских структур,
использующая универсальную структуру,
в которой тип передается как cv_byte* и хранится в массиве байт.\
Вектор можно создать как динамически расширяемый, так и фиксированного 
количества элементов для уменшения реаллокаций и увеличения производительности.\
По аналогии с std::vector в динамической версии вектора при добавлении
нового элемента реаллокация памяти происходит с резервом для уменьшения 
общего количества реаллокаций по формуле: 
[текущее колличество элементов + (текущее колличество элементов / 2)].\
При удалении элементов, когда размер резерва становится большим, 
также происходит реаллокация (освобождение) памяти.\
Размер зарезервированной памяти можно узнать с помощью функции capacity().

### Для выделения в памяти вектора необходимо:
cvector* vector = NULL;  // создать указатель на структуру  \
vector = cvector_new (0, sizeof(int));  // аллоцировать динамический вектор в памяти \
В функцию cvector_new:
 - Первым параметром (max_item) передается признак динамического
вектора: max_item = 0 - вектор динамического размера, 
max_item > 0 -  максимально хранящееся количество элементов.
 - Вторым параметром (item_size) передается размер элемента в байтах.
 
### Пример обращения к функциям:
int a = 8;\
vector->push(vector, (cv_byte*)&a);\
vector->pop(vector);

### Библиотека имеет следующие функции:
 - size - Размер данных (всех элементов) в байтах;
 - count - Количество элементов в векторе;
 - capacity - Размер выделенной памяти под вектор в байтах;
 - empty - Проверка на наличие элементов в векторе;
 - push - Добавление элемента в конец O(1);
 - pop - Удаление элемента из конца O(1);
 - replace - Замена элемента по индексу O(1);
 - remove - Удаление элемента по индексу O(log n);
 - at - Чтение элемента по индексу O(1);
 - clear - Удаление всех байтов и реалокация до одного байта;
 - free - Освобождение памяти (удаление вектора из памяти).


#
# EN:
A simple C vector library similar to the vector object
from the C++ standard library, with a user-friendly interface that supports all 
other types and user-defined structures
that use a universal structure
in which the type is passed as cv_byte* and stored in a byte array.\
A vector can be created either dynamically expandable or
with a fixed number of elements to reduce realizations and increase productivity.\
By analogy with std::vector in the dynamic version of a vector, when
a new element is added, memory is reallocated with a reserve to reduce 
the total number of realocations according to the formula: 
[current number of elements + (current number of elements / 2)].\
When deleting items, when the reserve size becomes large,
the memory is also deallocated. \
The size of the reserved memory can be found using the capacity() function.

### To allocate a vector in memory, you must:
cvector* vector = NULL; // create a pointer to the structure \
vector = cvector_new (0, sizeof(int)); // alocate the vector in memory \
To the cvector_new function:
 - The first parameter (max_item) indicates the dynamic
vector: max_item = 0 is a vector of dynamic size,
max_item > 0 is the maximum number of elements stored.
 - The second parameter (item_size) specifies the size of the item in bytes.
 
### Example of accessing functions:
int a = 8;\
vector->push(vector, (cv_byte*)&a);\
vector->pop(vector);

### The library has the following functions:
- size - The size of the data (of all elements) in bytes;
- count - The number of elements in the vector;
- capacity - The size of the allocated memory for the vector in bytes;
- empty - Check for the presence of elements in the vector;
- push - Adding an element to the end of O(1);
- pop - Removing an element from the end of O(1);
- replace - Replacing an element by the O(1) index;
- remove - Deleting an element by index O(log n);
- at - Reading an element by index O(1);
- clear - Delete all bytes and realocate to one byte;
- free - Freeing memory (deleting a vector from memory).










