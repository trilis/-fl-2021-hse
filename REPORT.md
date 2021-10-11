
<b>
Наумцев Антон <br/>
Домашняя работа <br/>
Второй курс | Первый модуль <br/>
Формальные языки # 4 <br/>  
</b>
<br/>
<br/>

<h2>Отчёт о проделанной работе:</h2>

Реализовал парсер для языка описания конечных автоматов. Исходный код находится в папке **source**, тесты - в **tests**.

Немного изменил язык: убрал лишнее поле у описания ребра

1) **__begin: state_id** для того, чтобы ребро можно было использовать не только для описания одного перехода, а нескольких. Рёбра теперь описываются следующим образом: 

   ```cpp
    new Edge(to_n0):
        __end: n0
        __allowed_letters: {d0}
   ```

2) Убрал из описания создания ребра/состояния конкретизацию места, к которому относится **id**: 

    *new State(**States.empty**): -> new  State(**empty**):* 
    
    так как тип структуры можно вывести из контекста 


    
    ```cpp
    new State(empty):
        __comment: 'начальное состояние'
        __edges: {to_n0, f0_t1, f0_t2}
        __is_terminal: False
   ```

<br/>
<br/>

<h2>Описание грамматики: </h2>
[1] Перечисления задаются аналогично перечислениям в языке программирования C++: между элементами могут стоять любые виды пробельных символов: табы, пробелы, переносы строк, переменные и запятые должны чередоваться, последним в перечислении может быть как переменная, так и запятая.

Примеры:

```cpp
// example 1
{a, a3, a4, a5, a6, a6}

// example 2
{
    a, a3, a4, a5, 
a6,}

// example 3
{
    a, 
    a3, 
    a4, a5, 
a6,}

// example 4
{

}
```

[2] Определение рёбер, алфавита, и состояний представляют из себя **INIZIALIZATION_STRUCT_NAME + ENUMERATION_LIST**, **Q0** определяется так же, единственное различие - в перечислении **Q0** может быть только 1 переменная. Примеры:

```cpp
_States {
    empty,
    n0,
    ost1,
    ost2,
    ost3
}

Q0    {start}
```

[3] Описание рёбер. За отступами следить сложно, поэтому было принято решение их не отслеживать. Поля **__end** и **__allowed_letters** являются обязательными. Пример корректных описаний рёбер: 
```
new Edge(f0_t0):
    __end: ost0
    __allowed_letters: {d0}

new Edge      (   f0_t0    )        :
    __end: ost0
         __allowed_letters: {
             d0,
             }
```

[4]  Описание состояний. У описания состояний обязательным полем является только **__is_terminal**. Примеры корректных описаний состояний.

```cpp 

new State(empty):
    __comment: 'начальное состояние'
    __edges: {to_n0, f0_t1, f0_t2}
    __is_terminal: False

new State(empty):
    __is_terminal: False

new State   (empty ):
    __comment: 'пустой список рёбер'
    __edges: {
    }
    __is_terminal: True
```


<h2>Реализованные проверки:</h2>

Было решено поделить проверки на две части: Warnings и Errors.

**Warnings** - проверки, сообщающие о некритических ошибках, после которых работа не завершается

**Errors** - проверки, после которых работа завершается


Сообщения о непрохождении проверок первого и второго типа отображаются в отчётах


Были реализованы следующие проверки:

**Warnings**:

1] Состояния автомата уникальны

Сообщение: "All elements **__Alphabet**/**__States**/**__Edges** should be unique"

2] Если у вершины не задан какой-то переход, то автомат считается неполным и программа выдаст предупреждение с **id** состояния: 

Сообщение: "Automata is not full. State id **state_id**"


**Errors**:

1] Проверка задания начального состояния

Сообщение: "Start state is not set"

2] Проверка детерминированности конечный автомата

Сообщение: "One letter can't be used in two edges"

3] Проверка задания Q0 единственным состоянием:

Сообщение: "QO must be inizializate by one state"

4] Проверка задания обязательного поля **__allowed_letters** у ребра:

Сообщение: "Edge with id **edge_id** doesn't have field __allowed_letters"

5] Проверка задания обязательного поля **__end** у ребра:

Сообщение: "Edge with id **edge_id** doesn't have field __end"


6] Проверка задания обязательного поля **__is_terminal** у состояния:

Сообщение: "State with id **state_id** doesn't have field __is_terminal"

7] Если lexer при разбиении на токены встретит неизвестный ему символ, то он выдаст ошибку:

Сообщение: "invalid character: **symbol** line: **line_number**"


<h2> Тесты: </h2>
Все тесты находятся в папке tests, каждый тест находится в отдельной папке и описывается двумя файлами: automata.txt и tests.txt, результат каждого теста находится в файле output.txt
<br/>
<br/>
**ERROR-ТЕСТЫ** прекращают работу на этапе разбора конечного автомата
<br/>
<br/>
**WARNING-ТЕСТЫ** - тесты, проверяющие корректно составленые автоматы, но выдающие варнинги

<br/>
<br/>
<h3>ERROR-TEST | test1:</h3>
Проверка инициализации начального состояния
<br/>
<br/>
<h3>WARNING-TEST | test2:</h3>
Автомат проверяющий корректность ввода одного числа
+
выдачи предупреждений о неполности автомата 
<br/>
<br/>
<h3>WARNING-TEST | test3:</h3>
Автомат проверяющий последовательность, состоящую из 0 и 1, на чередование символов
+ варнинги на определение неуникальными значениями состояний
<br/>
<br/>
<h3>ERROR-TEST | test4:</h3>
Проверка детерминированности автомата: из одной вершины не должно выходить два ребра по одинаковым буквам
<br/>
<br/>
<h3>ERROR-TEST | test5:</h3>
Проверка инициализации начального состояния ровно одним состоянием
<br/>
<br/>
<h3>WARING-TEST | test6:</h3>
Корректный конечный автомат, проверяющий, что в последовательности цифр чётность цифр чередуется.
<br/>
<br/
<br/>
<h3>WARNING-TEST | test7:</h3>
Автомат, считывающий число в троичной записи и проверящий, что сумма цифр в нём делится на 3
+ варнинги на неполноту автомата

<br/>
<br/>


<h2>Пример одного из тестов:</h2>


automata.txt:
```python
_Alphabet {
    d0, d1, d2, d3, d4, d5, d6, d7, d8, d9
}

_States {
    empty,
    ost1,
    ost2
}

_Q0{empty}


new State(empty):
    __edges: {to_0, to_1}
    __comment: 'начальное состояние'
    __is_terminal: False

new State(ost1):
    __edges: {to_0}
    __comment: 'Текущая цифра чётная'
    __is_terminal: True

new State(ost2):
    __comment: 'Текущая цифра нечётная'
    __edges: {to_1}
    __is_terminal: True


new Edge(to_0):
    __allowed_letters: {d0, d2, d4, d6, d8} 
    __end: ost2 

new Edge(to_1):
    __allowed_letters: {d1, d3, d5, d7, d9}
    __end: ost1
```

tests.txt
```python
d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 
d0
d1
d1 d2 d3 d5
d0 d0 d0 d2 d2 d2 
d1 d2 d5 d0 d3 d8 d4 
d5 d0 d5 d0 d1 d2 d9 d1
d0 d1 d8 d7 d8
d7 d4 d1 d2 d5
d0 d1 d0 d1 d0 d3 d2 d3 d2 d3 d2 d5 d8 d5 d8 d5 d8 d9 d8
```
output.txt:
```python
-----------------------Creating automata------------------------
[Warning] Automata is not full. State id 'ost1'
[Warning] Automata is not full. State id 'ost2'
----------------Automata is created successfully----------------


Start testing file: './test6/tests.txt'

TEST #1: d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 
True | Final state: ost1 | 'Текущая цифра чётная'

TEST #2: d0
True | Final state: ost2 | 'Текущая цифра нечётная'

TEST #3: d1
True | Final state: ost1 | 'Текущая цифра чётная'

TEST #4: d1 d2 d3 d5
False | Final state: None

TEST #5: d0 d0 d0 d2 d2 d2 
False | Final state: None

TEST #6: d1 d2 d5 d0 d3 d8 d4 
False | Final state: None

TEST #7: d5 d0 d5 d0 d1 d2 d9 d1
False | Final state: None

TEST #8: d0 d1 d8 d7 d8
True | Final state: ost2 | 'Текущая цифра нечётная'

TEST #9: d7 d4 d1 d2 d5
True | Final state: ost1 | 'Текущая цифра чётная'

TEST #10: d0 d1 d0 d1 d0 d3 d2 d3 d2 d3 d2 d5 d8 d5 d8 d5 d8 d9 d8
True | Final state: ost2 | 'Текущая цифра нечётная'
```