# Лабораторная работа: "Использование SIMD инструкций процессора для ускорения работы программы."

## Задача

### 1) Написать три варианта программы, рисующей фрактал Мандельброта:
- **1 вариант:** обычная программа без каких-либо улучшений
- **2 вариант:** при вычислении точек для построения фрактала использовать массивы из 8 элементов, чтобы дать оптимизатору возможность использовать SIMD инструкции
- **3 вариант:** самостоятельно использовать SIMD инструкции, поддерживаемые процессором.

### 2) Сравнить скорости работы трех вариантов программы.

### 3) Ответить на вопрос: удалось ли обогнать оптимизатор?

## Используемое оборудование:

### Программы запускаются и тестируются на удаленном сервере:
**Данные о СPU:**

<img
  src="Images/cpu.png"
  alt="Alt text"
  title="Optional title"
  style="display: inline-block; margin: 0 auto; max-width: 600px">

**Операционная система:** Linux Ubuntu 22.04

## Ход работы

### Точки для построения фрактала Мандельборта находятся по следующей формуле.


- x<sub>n</sub> = x<sub>n-1</sub><sup>2</sup> - y<sub>n-1</sub><sup>2</sup> + x<sub>0</sub>
- y<sub>n</sub> = 2 * x<sub>n-1</sub>y<sub>n-1</sub> + y<sub>0</sub>

### Полученная картинка

<img
  src="Images/Mandelbrot.png"
  alt="Alt text"
  title="Optional title"
  style="display: inline-block; margin: 0 auto; max-width: 600px">

 1) В первой версии программы не применялись никакие оптимизации. Ниже показана разница в скорости исполнения программы, скомпилированной сначала с высоким уровнем оптимизации, а потом с низким.

|Оптимизация|Кол-во тактов  |
|:---------:|:-------------:|
|    -O0    |   490860293   |
|    -O3    |  210271065    |

**Прирост скорости:** примерно в 2.3 раза

 2) Во второй программе в вычислениях участвуют восьми элементные массивы. Это позволяет оптимизатору самостоятельно вставлять SIMD инструкции. Видно, что
 разница между различными уровнями оптимизации выше, чем в других версиях программы.

|Оптимизация|Кол-во тактов  |
|:---------:|:-------------:|
|    -O0    | 30746534403   |
|    -O3    |  4612440772   |

**Прирост скорости:** примерно в 6.7 раза

 3) В третьей версии напрямую использованы SIMD инструкции: AVX, AVX2. При низком уровне оптимизации, 100 screens

|Оптимизация|Кол-во тактов  |
|:---------:|:-------------:|
|    -O0    | 191093953     |
|    -O3    |  9504686      |

**Прирост скорости:** примерно в 3.2 раза


 ### Все результаты представлены в таблице ниже, все программы скомпилированы с флагом оптимизации -O3. В третьем столбце представлено отношение скорости работы программ по отношению к первому варианту программы.

|Программа  |Кол-во тактов  | Отношение скорости         |
|:---------:|:-------------:|:---------:|
|       1   |   210271065   |  1.0      |
|       2   |  4612440772   |  0.046    |
|       3   |    60371086   |  3.48     |



## Вывод:

Использование для работы удаленного сервера оказывает сильное влияние на скорость работы, поэтому полученные результаты нельзя считать абсолютно точными.
Тем не менее видно, что использование SIMD инструкций на порядок ускоряет работу программы. В результате удалось обогнать оптимизатор примерно в 3.48 раза.