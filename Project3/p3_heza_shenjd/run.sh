make clean
make calc

for i in {1..5}
do
    ./calc < testcase
    # ./calc < naivetest
done

echo "Origin: 58s."