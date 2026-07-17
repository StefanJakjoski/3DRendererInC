gcc meshTests.c ../src/mesh.c ../src/LinearAlgebra.c -lm -o test
./test 2>/dev/null
rm test
