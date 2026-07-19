# 3DRendererInC
The above is an attempt at a rudimentary, CPU-powered 3D renderer written in C using custom modules. The end goal is to have a fully capable 3D renderer that can parse arbitrary object files and color maps, transform the underlying mesh, utilize various shading and light simulation techniques, and export a high definition render.

## CLI Guide:
A CLI tool is provided for easier testing and access. To begin, navigate to the /tests directory. On Linux, a bash file that handles compiling is already provided. Simply run: 
```bash
./RunCLI.sh
``` 

For Windows, no file is provided but simply run the windows equivalent:
```PowerShell
gcc 3DRendererCLI.c ..\src\objReader.c ..\src\rasterizer.c ..\src\image.c ..\src\mesh.c ..\src\LinearAlgebra.c -lm -o test.exe  
test.exe
del test.exe
```

Afterwards, use the following lines in the command line as specified.

1. `parse <location of .obj file>` to parse the object file.
2. `push <x, y, z>` to push object into view.
3. `scale <scale coefficient>` to scale the object up/down.
4. `rotate <x, y, z>` to rotate that many degrees across the specified axis.
5. `light <x, y, z>` to define direction of light source (interpreted as the direction in which light "falls down", not the direction towards the light).
6. `image <width, height>` to redefine the image dimensions (default at <1000, 1000>).
7. `color <r, g, b>` to specify the default color for 'monochrome' rendering.
8. `render <type, name>` to specify the type of render ('monochrome' or 'random') and the name (or relative location) where the render will be exported.
9. `camera <x, y, z>` to specify the coordinates of the camera position
10. `camera_rotate <x, y, z>` to rotate the camera by the specified values on the given axes.
11. `exit` to safely shut down CLI. 

## TODO:
### High Priority:
* ~~Implement camera functionality (positioning, perspective view, near/far thresholds) (currently hardcoded at position 500,500,0 with flat projection)~~
* Implement .png (image) parsing
* Implement image wrapping over object

### Medium Priority:
* Implement other shading options (Phong, diffusion, etc...)
* Update CLI to match any changes

### Low Priority:
* Implement custom backgrounds (thin grid, white or other colors, etc...)
* Implement shadows onto background plane 

## Assets
DISCLAIMER: None of the assets are mine. I got all of them off of various online platforms or wrote them up myself for the purposes of testing.
