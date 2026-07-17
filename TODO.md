```markdown
# Software Renderer To-Do List

A CPU-based 3D renderer written in C.

# Phase 0 — Project Setup

- ~~Create project directory structure~~
- ~~Create include/ directory for header files~~
- ~~Create src/ directory for implementation files~~
- Create assets/ directory for models and textures
- Create examples/ directory for test programs
- ~~Create tests/ directory for unit tests~`
- Create Makefile

## Suggested Modules

- Linear Algebra
    - Existing custom math library

- Image
    - Framebuffer storage
    - PPM export

- Mesh
    - Vertex storage
    - Triangle storage

- OBJ Loader
    - Parse OBJ files
    - Convert OBJ data into Mesh objects

- Camera
    - Position
    - Orientation
    - Projection

- Scene
    - Store objects
    - Store lights
    - Store camera

- Renderer
    - Coordinate transformations
    - Rendering pipeline

- Rasterizer
    - Triangle filling
    - Pixel generation

# Phase 1 — Image System

## Color (DONE)

- ~~Create Color structure~~
- ~~Store RGB values~~
- ~~Use 8-bit channels~~

## Image Structure (DONE)

- ~~Store width~~
- ~~Store height~~
- ~~Store pixel buffer~~

## Image Functions

- ~~Implement image_create()~~
- ~~Implement image_destroy()~~
- ~~Implement image_clear()~~
- Implement image_set_pixel()
- Implement image_get_pixel()

## PPM Export

- ~~Implement P3 ASCII PPM output~~
- Implement P6 binary PPM output
- ~~Test saving blank images~~
- ~~Test opening images externally~~

# Phase 2 — Linear Algebra Integration

- Verify vector operations
- Verify matrix operations
- Verify dot product
- Verify cross product
- Verify normalization
- Verify matrix multiplication
- Verify matrix inversion

Required types:

- vec2
- vec3
- vec4
- mat3
- mat4

# Phase 3 — Basic Drawing (DONE)

- Implement pixel drawing
- Implement line drawing
- Implement Bresenham line algorithm
- Draw coordinate axes
- Draw wireframe cube

# Phase 4 — Mesh System

## Vertex Data

Create support for:

- Vertex positions
- Vertex normals
- Texture coordinates

## Triangle Data

Each triangle stores:

- ~~Three vertex indices~~
- Three normal indices
- Three UV indices
- ~~Triangle normal~~
Example:

- vertex[0] corresponds with normal[0] and uv[0]
- vertex[1] corresponds with normal[1] and uv[1]
- vertex[2] corresponds with normal[2] and uv[2]

## Mesh Functions

- ~~Implement mesh_create()~~
- ~~Implement mesh_destroy()~~
- ~~Implement normal generation~~
- ~~Implement mesh transformations~~

# Phase 5 — OBJ Loader

## Support OBJ Commands

- Parse vertex positions (v)
- Parse texture coordinates (vt)
- Parse normals (vn)
- Parse faces (f)

## Support Face Formats

- Vertex only
- Vertex + UV
- Vertex + UV + Normal

## OBJ Loader API

- Implement obj_load()

## Testing

- Load cube OBJ
- Load Suzanne OBJ
- Load Stanford bunny OBJ

# Phase 6 — Camera System

## Camera Structure

Store:

- Position
- Forward vector
- Right vector
- Up vector
- Field of view
- Near plane
- Far plane

## Camera Orientation

Implement:

- Camera look-at function
- Forward vector calculation
- Right vector calculation
- Up vector correction

## View Matrix

Implement:

- Convert world coordinates into camera coordinates
- Build camera view matrix

# Phase 7 — Projection System

## Perspective Projection

Implement:

- Field of view handling
- Aspect ratio handling
- Near plane handling
- Far plane handling

## Projection Matrix

Implement:

- Perspective matrix creation
- Perspective divide
- Clip coordinate conversion

## Testing

- Render cube with perspective
- Test different FOV values
- Test near plane
- Test far plane

# Phase 8 — Scene System

## Object Structure

Store:

- Mesh pointer
- Transformation matrix

## Scene Structure

Store:

- Objects
- Object count
- Camera
- Lights

## Scene Functions

- Create scene
- Add objects
- Remove objects
- Destroy scene

# Phase 9 — Renderer Core

## Renderer Structure

Store:

- Framebuffer
- Depth buffer

## Renderer Functions

- renderer_create()
- renderer_render()
- renderer_destroy()

## Rendering Pipeline

Implement:

- Object transform
- World transform
- View transform
- Projection transform
- Perspective divide
- Viewport transformation
- Rasterization

# Phase 10 — Triangle Rasterizer

## Triangle Processing

Implement:

- Convert vertices to screen coordinates
- Calculate triangle bounding box
- Iterate over affected pixels
- Calculate barycentric coordinates
- Determine if pixel is inside triangle
- Interpolate attributes

## Interpolate

Support:

- Depth
- Color
- Normals
- UV coordinates

# Phase 11 — Depth Buffer

Implement:

- Depth buffer allocation
- Depth buffer clearing
- Depth testing
- Depth writing

Testing:

- Overlapping triangles
- Rotating cube
- Multiple objects

# Phase 12 — Basic Shading

## Flat Shading

Implement:

- Triangle normal calculation
- Single color per triangle

## Smooth Shading

Implement:

- Vertex normal interpolation
- Normal normalization
- Per-pixel normals

# Phase 13 — Lighting

## Directional Light

Implement:

- Light direction
- Light color
- Light intensity

## Lambert Lighting

Implement:

- Normal-light dot product
- Diffuse lighting

## Phong Lighting

Implement:

- Ambient lighting
- Diffuse lighting
- Specular highlights

# Phase 14 — Texture System

## Texture Structure

Store:

- Width
- Height
- Pixel data

## Texture Features

Implement:

- Texture loading
- UV sampling
- UV interpolation
- Texture mapping

# Phase 15 — Advanced Rasterization

Implement:

- Perspective-correct interpolation
- Back-face culling
- Triangle clipping
- Frustum clipping

# Phase 16 — Debug Features

Implement:

- Wireframe rendering
- Normal visualization
- Depth visualization
- UV visualization
- Bounding box visualization

# Phase 17 — Performance Improvements

Implement:

- Better memory layout
- Fewer allocations
- Object batching
- Spatial partitioning
- Multithreaded rendering

# Final Renderer Goals

The renderer should:

- Load OBJ models
- Render arbitrary meshes
- Support camera movement
- Support perspective projection
- Correctly rasterize triangles
- Perform depth testing
- Support lighting
- Support textures
- Export images as PPM files

Example final workflow:

- Load OBJ file
- Create scene
- Add camera
- Add lights
- Render scene
- Save framebuffer to PPM
```
