valgrind -s  --read-var-info=yes --read-inline-info=yes  --track-origins=yes --leak-check=full --log-file="valgrind-report.txt" ./build/test-compiler
#--expensive-definedness-checks=yes