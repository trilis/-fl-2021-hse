[Документация](https://github.com/antlr/antlr4/tree/master/doc/index.md)

[Грамматики для разных языков](https://github.com/antlr/grammars-v4)

[Как устанавливать ANTLR](https://github.com/antlr/antlr4/blob/master/doc/getting-started.md)

ANTLR по умолчанию генерирует файлы для Java, но умеет под много языков, включая Python и C++. Ссылки на гайды про конкретные языки [здесь](https://github.com/antlr/antlr4/blob/master/doc/targets.md)

Для примера из этого репозитория я генерил файлы с помощью

`antlr4 -Dlanguage=Python3 -visitor arithmetic.g4`

(`-visitor` чтобы использовать visitor, по умолчанию генерируется только listener)

После этого в питоновском файле можно заимпортить то что сгенерилось (в файле `visitor.py` показано, как) и просто запустить его: `python3 visitor.py`