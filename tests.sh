#!/usr/bin/env bash
./build.sh

./run tests/test1.aut tests/test1.wrd 1>tests/cur_ans
if cmp -s -- tests/test1.ans tests/cur_ans; then
  echo "test1 is ok"
else
  echo "test1 is failed"
fi

./run tests/test2.aut tests/test2.wrd 1>tests/cur_ans
if cmp -s -- tests/test2.ans tests/cur_ans; then
  echo "test2 is ok"
else
  echo "test2 is failed"
fi

./run tests/test3.aut tests/test3.wrd 1>tests/cur_ans
if cmp -s -- tests/test3.ans tests/cur_ans; then
  echo "test3 is ok"
else
  echo "test3 is failed"
fi

./run tests/test4.aut tests/test3.wrd 1>/dev/null 2>tests/cur_ans
if cmp -s -- tests/test4.ans tests/cur_ans; then
  echo "test4 is ok"
else
  echo "test4 is failed"
fi

./run tests/test5.aut tests/test3.wrd 1>/dev/null 2>tests/cur_ans
if cmp -s -- tests/test5.ans tests/cur_ans; then
  echo "test5 is ok"
else
  echo "test5 is failed"
fi

./run tests/test6.aut tests/test3.wrd 1>/dev/null 2>tests/cur_ans
if cmp -s -- tests/test6.ans tests/cur_ans; then
  echo "test6 is ok"
else
  echo "test6 is failed"
fi

rm tests/cur_ans