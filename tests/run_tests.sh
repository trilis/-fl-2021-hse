cd .. && make && cd -
for num in {1..12}; do
    rm -f tmp.out
    echo run test "$num"
    ../a.out ./test"$num"_a.in ./test"$num"_s.in 1>./tmp.out 2>./tmp.out
    cmp tmp.out ./test"$num".out
done