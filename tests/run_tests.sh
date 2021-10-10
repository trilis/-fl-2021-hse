rm -f tmp.out
cd ..
make ..
cd -
../a.out test01_a.in 1 > tmp.out
cmp tmp.out test01.out