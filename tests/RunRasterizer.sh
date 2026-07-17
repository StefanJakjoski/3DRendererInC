gcc ../src/main.c ../src/rasterizer.c ../src/LinearAlgebra.c ../src/image.c ../src/mesh.c -lm -o test
./test
rm test 