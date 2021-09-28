# Задание 2

## Время работы 

При помощи скриптов, принимающих
единственный параметр - строку, можно запустить [медленную](/run_slow) версию [быструю](/run_fast) и оценить разницу во времени работы для произвольной строки. Время работы замедляется при помощи утилиты `time`.

Для тестов, представленных в [tests](\tests):

```txt
Test #1
slow version:
    Кончилась оперативная память 
    на втором регулярном выражении
    real    1m53,198s
    user    1m26,617s
    sys     0m7,845s
fast version:
    real    0m0,008s
    user    0m0,000s
    sys     0m0,008s
```

```txt
Test #2
slow version:
    Кончилась оперативная память 
    на втором регулярном выражении
    real    2m7,916s
    user    1m41,193s
    sys     0m9,338s
fast version:
    real    0m0,017s
    user    0m0,012s
    sys     0m0,000s
```

```txt
Test #3
slow version:
    real    0m0,008s
    user    0m0,008s
    sys     0m0,000s
fast version:
    real    0m0,008s
    user    0m0,008s
    sys     0m0,000s
```

```txt
Test #4
slow version:
    real    0m0,006s
    user    0m0,000s
    sys     0m0,006s
fast version:
    real    0m0,006s
    user    0m0,006s
    sys     0m0,000s
```

```txt
Test #5
slow version:
    Кончилась оперативная память 
    на втором регулярном выражении
    real    2m25,773s
    user    1m27,057s
    sys     0m8,241s
fast version:
    real    0m0,015s
    user    0m0,011s
    sys     0m0,000s
```

```txt
Test #6
slow version:
    real    0m1,211s
    user    0m1,114s
    sys     0m0,096s
fast version:
    real    0m0,008s
    user    0m0,004s
    sys     0m0,004s
```

Как можно заметить, стало значительно лучше, потому что стало отрабатывать
на всех регулярных выражениях, как минимум.

# Длинный тест

Внутри [main.cpp](main.cpp) есть регулярное выражение `r_slow`, которое принимает любые строки
из латинских букв и цикфр от 0 до 9.
На тесте из 400 символов отрабатывает уже примерно две секунды. Объяснение тому- Concat и Star
довольного сильно раскрываются, из-за чего регулярное выражение растет.

```
./run_fast sefysebfuyabv897908asf12dsfahuefyvb7y634grfwuevb76rgwfvbujh23vb7i623hv1y3vsdf67uyeqvfvweqf76wefge66sefysebfuyabv897908asf12dsfahuefyvb7y634grfwuevb76rgwfvbujh23vb7i623hv1y3vsdf67uyeqvfvweqf76wefge66sefysebfuyabv897908asf12dsfahuefyvb7y634grfwuevb76rgwfvbujh23vb7i623hv1y3vsdf67uyeqvfvweqf76wefge66sefysebfuyabv897908asf12dsfahuefyvb7y634grfwuevb76rgwfvbujh23vb7i623hv1y3vsdf67uyeqvfvweqf76wefge66

real    0m1,916s
user    0m1,858s
sys     0m0,056s
```