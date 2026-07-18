gcc parseObject.c ../src/objReader.c ../src/rasterizer.c ../src/camera.c ../src/image.c ../src/mesh.c ../src/LinearAlgebra.c -lm -o test
./test $1 $2
rm test