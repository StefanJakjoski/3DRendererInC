gcc parseTest.c ../src/objReader.c ../src/rasterizer.c ../src/camera.c ../src/image.c ../src/mesh.c ../src/LinearAlgebra.c -lm -o test
./test
rm test