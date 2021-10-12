### _1. Проверьте следующие языки на регулярность, за каждое 1 балл. Либо докажите их нерегулярность, либо постройте для них автомат или регулярку._


* Язык не регулярен, докажем через отрицание леммы о накачке: возьмем слово <img src="https://i.upmath.me/svg/a%5Enabb%5En" alt="a^nabb^n" />. <img src="https://i.upmath.me/svg/u%20%3D%20a%5En%2C%20v%20%3D%20b%5En%2C%20%7Cu%7C%20%3D%20%7Cb%7C%20%3D%20n" alt="u = a^n, v = b^n, |u| = |b| = n" />. Выберем <img src="https://i.upmath.me/svg/%20k%20%3D%200" alt=" k = 0" />. Тогда <img src="https://i.upmath.me/svg/%7Cu_%7Bnew%7D%7C%20%3C%20n%20%3D%7Cv%7C" alt="|u_{new}| &lt; n =|v|" />, следовательно слово не принадлежит языку

* Язык не регулярен: берем слово <img src="https://i.upmath.me/svg/a%5En%20c%5Em%20e%5En%2C%20m%20%3D%20n%20%2B%20n%20%2B%201" alt="a^n c^m e^n, m = n + n + 1" />. Возьмем <img src="https://i.upmath.me/svg/k%20%3D%200" alt="k = 0" />, тогда степень при <img src="https://i.upmath.me/svg/a" alt="a" /> будет меньше исходной и <img src="https://i.upmath.me/svg/m%20%3C%20n%20%2B%20n%20%2B%201" alt="m &lt; n + n + 1" />. Тогда степень <img src="https://i.upmath.me/svg/n%20%2B%20n%20%2B%201" alt="n + n + 1" /> при <img src="https://i.upmath.me/svg/c" alt="c" /> не будет совпадать со значением <img src="https://i.upmath.me/svg/m" alt="m" />, следовательно, слово не принадлежит языку.

* Мы не знаем, бесконечно ли количество чисел-близнецов. Рассмотрим два случая:
    - Их конечное число, и наибольшее из левых близнецов это <img src="https://i.upmath.me/svg/k" alt="k" />. Тогда все слова языка имеют длину меньше <img src="https://i.upmath.me/svg/k" alt="k" />, их можно задать регулярным выражением <img src="https://i.upmath.me/svg/a%5C%7B0%2C%20k%5C%7D" alt="a\{0, k\}" />
    - Их бесконечно, отлично, тогда нам подходят вообще любые слова состоящие только из <img src="https://i.upmath.me/svg/a" alt="a" />. Это задается регулярным выражением <img src="https://i.upmath.me/svg/a*" alt="a*" />

### _2. Парсинг с помощью производных_


Применила все оптимизации. Некоторые интересные результаты:

Без оптимизаций:
9) 4.929538s|aaaaaaaaaa: False
10) 30.234448s|abababababa: True
С оптимизацией:
9) 0.000033s|aaaaaaaaaa: False
10) 0.000147s|abababababa: True

test3. Dividing by 3
3) 12.021193s|10000001: True
--
3) 0.000237s|10000001: True

test5. (a | b | c | d)*
3) 2.511186s|dabcddacbd: True
--
3) 0.000253s|dabcddacbd: True

test2: ((a|b)(a|b))*
3) 7.054936s|aaaabababa: True
--
3) 0.000183s|aaaabababa: True


Был еще тест, на котором с оптимизацией отработало меньше секунды, а без оптимизации больше 160 секунд, но мой компьютер умер и я решила не рисковать с такими тестами(


Тест, на котором оптимизация работает долго: `(aba|bab|cdc)* ((a|b)f)*`, со строкой `abacdcbabafafbfabacdcbabafafbfabacdcbabafafbf` работает за '13.309543s'
Без оптимизации компьютер зависает

```cpp
test_hard = Star(Concat(
    Star(Alt(Alt(Concat(Concat(Char('a'), Char('b')), Char('a')),
                 Concat(Concat(Char('b'), Char('a')), Char('b'))),
             Concat(Concat(Char('c'), Char('d')), Char('c')))),
    Star(Concat(Alt(Char('a'), Char('b')), Char('f')))))
```





