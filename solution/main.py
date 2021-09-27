import time
from Derivatives import match
from tests import tests_blocks


if __name__ == '__main__':
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
                start_time = time.time()
                res = match(r, input_str)
                end_time = time.time()
                status = 'OK' if res == expected else 'Wrong Answer'
                print(f'{input_str} | Status: {status} | Time: {end_time - start_time: 0.6f}s')
        print('\n')