import time
from DerivativesOptimized import match
from tests import tests_blocks


if __name__ == '__main__':
    with_big_tests = True
    for block in tests_blocks:
        block_name = block['name']
        print(f'------------- {block_name} -------------')
        for (index, test) in enumerate(block['tests'], start=1):
            regExp = test['humanReadableRegExp']
            print(f'TEST #{index} | Expression: {regExp}')
            r = test['regExp']
            for sample in test['samples']:
                input_str = sample['string']
                expected = sample['expected']
                is_big = sample['is_big']
                if not with_big_tests and is_big:
                    start_time = 0
                    end_time = 0
                    status = 'Skipped'
                else:
                    start_time = time.time()
                    res = match(r, input_str)
                    end_time = time.time()
                    status = 'OK' if res == expected else 'Wrong Answer'
                print(f'{input_str} | Status: {status} | Time: {end_time - start_time: 0.6f}s')
        print('\n')