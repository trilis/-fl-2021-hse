from Derivatives import *


small_tests = [
    {
        'humanReadableRegExp': '0(0|1)*0',
        'regExp': Concat(
                    Char('0'),
                    Concat(
                           Star(
                               Alt(
                                    Char('0'),
                                    Char('1')
                               )
                            ),
                            Char('0')
                        )
                    ),
        'samples': [
            {
                'string': '0',
                'expected': False,
                'is_big': False,
            },
            {
                'string': '00',
                'expected': True,
                'is_big': False,
            },
            {
                'string': '10101010101010000',
                'expected': False,
                'is_big': True,
            },
            {
                'string': '0000000000',
                'expected': True,
                'is_big': False,
            },
            {
                'string': '101010101000',
                'expected': False,
                'is_big': True,
            },
        ]
    },
    {
        'humanReadableRegExp': '(111|000)*',
        'regExp': Star(
            Alt(
                Concat(
                    Char('1'),
                    Concat(
                           Char('1'),
                           Char('1')
                        )
                ),
                Concat(
                    Char('0'),
                    Concat(
                           Char('0'),
                           Char('0')
                        )
                )
            )
        ),
        'samples': [
            {
                'string': '11010111111000111',
                'expected': False,
                'is_big': True,
            },
            {
                'string': '000000000',
                'expected': True,
                'is_big': False,
            },
            {
                'string': '11100011100011101',
                'expected': False,
                'is_big': True,
            },
            {
                'string': '',
                'expected': True,
                'is_big': False,
            },
            {
                'string': '000111000',
                'expected': True,
                'is_big': False,
            },
        ]
    },
    {
        'humanReadableRegExp': '(11)*01(00)*',
        'regExp': Concat(
            Star(
                Concat(
                    Char('1'),
                    Char('1')
                )
            ),
            Concat(
                Concat(
                    Char('0'),
                    Char('1')
                ),
                Star(
                    Concat(
                        Char('0'),
                        Char('0')
                    )
                )
            )
        ),
        'samples': [
            {
                'string': '110100',
                'expected': True,
                'is_big': False,
            },
            {
                'string': '01',
                'expected': True,
                'is_big': False,
            },
            {
                'string': '1111010',
                'expected': False,
                'is_big': False,
            },
            {
                'string': '111010000',
                'expected': False,
                'is_big': False,
            },
            {
                'string': '11111111110100000000000000',
                'expected': True,
                'is_big': True,
            },
        ]
    },
    {
        'humanReadableRegExp': '101010',
        'regExp': Concat(
            Concat(
                Concat(
                    Char('1'),
                    Char('0')
                ),
                Char('1')
            ),
            Concat(
                Concat(
                    Char('0'),
                    Char('1')
                ),
                Char('0')
            )
        ),
        'samples': [
            {
                'string': '101010',
                'expected': True,
                'is_big': False,
            },
            {
                'string': '10101011123',
                'expected': False,
                'is_big': False,
            },
            {
                'string': '',
                'expected': False,
                'is_big': False,
            },
            {
                'string': '1010',
                'expected': False,
                'is_big': False,
            },
        ]
    }
]


large_tests = [
]


tests_blocks = [
    {
        'name': 'SMALL TESTS',
        'tests': small_tests
    },
    {
        'name': 'LARGE TESTS',
        'tests': large_tests
    },
]
