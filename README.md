<h1 align="center">Hiruki</h1>
<p>
    Hiruki is a 3D software rasterizer, written in C++. The primary focus of this project is self-learning in the field of computer graphics and related algebra, along with creating and mantaining a comprehensive, useful library.
</p> 

<p>
    Hiruki uses software rendering, meaning it does not use any graphics engines nor make use of the GPU. It only uses SDL2 to manually render each pixel in the screen.
</p>

## Screenshots
<img src="https://github.com/user-attachments/assets/041b6ea5-2453-4939-bf14-b9004d514142" width="700" />

## Features
- Left-handed coordinate system.
- Perspective corrected and texture interpolation.
- Basic directional lighting (flat or Goraud).
- Basic camera system (via Up and LookAt).
- Z-buffer and backface culling.
- Full CPU rasterization that uses parallelization if wanted.
- Simple implementation, making the algorithms easy to read and understand.
- Built-in multi-textured and multi-meshed OBJ loading.

## Limitations
- Only compatible 3D models are OBJs.
- No alpha or transparency.

## Examples
There are three examples using the library available ![in the examples folder](./examples/).

## Build dependencies
- Cmake (3.20 or higher)
- SDL2 (2.30.3 or higher)
- OpenMP
- C++ 20

## License
Hiruki is open source under the terms of the ![MIT License](LICENSE).

## Credits & references
- https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/
- Used models' credits in their corresponding example `credits.txt` file.
- Used font, Alagard, made by `Hewett Tsoi`

<br>

Made by [Hugo Moreda](https://hmoreda.com)
