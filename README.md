# 3D-raytracer

A small raytracing project to create simple 3D graphics of spheres. I built this project puerly for learning purposes. I did not use any graphics API like Vulkan or OpenGL.

## 💻 Technologies

- `C`
- `Windows API`

## 📦 Features

This simple raytracer supports the following features:

- Rendering spheres
- Matte and shiny materials
- Reflective materials
- Transparent materials
- Refraction of light
- Responding to window resizes

## 🛠️ Making the raytracer

I wanted to learn how computer graphics work, so I got the idea to build my own simple renderer. I chose to start with raytracing because it can produce physics-accurate graphics.

I started with learning how the Windows API works so I could create a window and manipulate individual pixels on the window.

Quickly I realized I had to implement most of how to handle colors myself.

After this, I mostly followed the theory as outlined in Gabriel Gambetta's [Computer Graphics from Scratch](https://gabrielgambetta.com/computer-graphics-from-scratch/02-basic-raytracing.html).

From this book I learned how basic raytracing techniques work, like rendering, different types of lights, shadows, and reflections.

In order to create glass-like materials, I also added transparent materials that can refract light. I had to figure this part out myself, since the book mentioned above does not cover these topics. In order to increase the visual quality even further, I also added oversampling.

Since no extra graphics API was used, everything is ran on the CPU. This can make the window lag a bit, as raytracing is a computationally intensive task.

## ⚡ Running the project

This project is not cross-platform and Windows-only.

To run the raytracer, you can either downlaod and run the ex-file found [here](https://github.com/JarnoCor/3D-raytracer/releases), or follow these steps:

1. Install any C compiler (like [GCC](https://gcc.gnu.org))
2. Install [Make](https://www.gnu.org/software/make/)
3. Clone the repository to your local machine
4. Run `make` in the project directory
5. Run the generated exe-file

## 📸 Output

![Output of the raytracer, depcting spehes with different materials.](/media/screenshot.png "Output")
