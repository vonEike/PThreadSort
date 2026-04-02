# Руководство программиста
````markdown
## Модули

### Состав файлов и модульные границы

В корне репозитория присутствуют исходники и заголовки, а также примеры входа/выхода и артефакты сборки:

- `heap.c/.h`
- `parallel.c/.h`
- `io.c/.h`
- `main.c`
- `point.h`
- примеры `input.txt`, `output.txt`
- объектные файлы `*.o`
- бинарный файл `heapsort`

### Назначение модулей

- **point.h** — структура данных `Point3D`
- **heap.c/.h** — локальная сортировка (heapsort), вспомогательные функции
- **parallel.c/.h** — параллельная сортировка и merge
- **io.c/.h** — ввод/вывод
- **main.c** — точка входа

---

## Представление данных

Тип элемента:

```c
typedef struct {
    double x;
    double y;
    double z;
} Point3D;
````

Сортировка:

1. по `x`
2. при равенстве — по `y`
3. затем по `z`

---

## Структуры данных

### Point3D

* координаты: `x`, `y`, `z`
* тип: `double`

---

### ThreadData (internal)

Используется в `parallel.c`

```c
typedef struct {
    Point3D* array;
    int left;
    int right;
} ThreadData;
```

**Семантика:**

* `array` — общий массив
* `[left, right)` — диапазон

**Инвариант:**

```
0 ≤ left ≤ right ≤ size
```

---

## Справочник функций

### Таблица функций

| Функция           | Файл       | Назначение              |
| ----------------- | ---------- | ----------------------- |
| main              | main.c     | управление программой   |
| readArrayFromFile | io.c       | чтение файла            |
| writeArrayToFile  | io.c       | запись файла            |
| heapSort          | heap.c     | сортировка              |
| comparePoints     | heap.c     | сравнение               |
| swap              | heap.c     | обмен                   |
| heapify           | heap.c     | куча                    |
| parallelHeapSort  | parallel.c | параллельная сортировка |
| threadFunction    | parallel.c | поток                   |
| merge             | parallel.c | слияние                 |

---

## Описание функций

### main

```c
int main(int argc, char** argv);
```

Точка входа программы.

**Параметры:**

* `argc` — количество аргументов
* `argv` — аргументы

**Возвращает:**

* `0` — успех
* `1` — ошибка

---

### readArrayFromFile

```c
int readArrayFromFile(const char* filename, Point3D** out_array, int* out_count);
```

Чтение массива из файла.

---

### writeArrayToFile

```c
int writeArrayToFile(const char* filename, Point3D* array, int count);
```

Запись массива в файл.

---

### heapSort

```c
void heapSort(Point3D arr[], int n);
```

Сортировка методом heapsort.

---

### parallelHeapSort

```c
void parallelHeapSort(Point3D* array, int size, int threads_count);
```

Параллельная сортировка.

---

## Алгоритм

Алгоритм состоит из двух фаз:

### 1. Параллельная сортировка

Каждый поток:

```c
heapSort(segment)
```

---

### 2. Слияние

Главный поток:

```c
merge(...)
```

---

## Модель потоков

Создание:

```c
pthread_create(...)
```

Ожидание:

```c
pthread_join(...)
```

---

## Последовательность выполнения

1. Чтение данных
2. Деление на сегменты
3. Создание потоков
4. Сортировка
5. merge
6. запись результата

---

## Жизненный цикл

* запуск `main`
* чтение данных
* параллельная сортировка
* запись
* освобождение памяти

---

## Сборка и запуск

```bash
gcc main.c io.c parallel.c heap.c -o heapsort -pthread
```

---

## Проверки ошибок

Отсутствуют проверки:

* `malloc`
* `pthread_create`
* `pthread_join`

---

# Руководство пользователя

## Назначение

Сортировка массива точек из файла.

---

## Формат входных данных

Пример:

```
102,000
101,000
100,000
...
```

---

## Параметры запуска

```bash
./heapsort input.txt output.txt
```

| Аргумент | Назначение    |
| -------- | ------------- |
| argv[1]  | входной файл  |
| argv[2]  | выходной файл |

---

## Формат выходных данных

```
count
x y z
x y z
...
```

---

## Пример

```
5
0.000000 9.000000 9.000000
1.000000 2.000000 2.000000
...
```
