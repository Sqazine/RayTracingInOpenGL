# RayTracingInOpenGL
The OpenGL implementation of Peter Shirley's ray tracing in one weekend series of books


<!-- PROJECT SHIELDS -->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![Apache-2.0 License][license-shield]][license-url]

<!-- PROJECT LOGO -->
<br />

# How to build
1. install C++ 17 compiler
2. install CMake(version >=3.10)
3. clone Microsoft's vcpkg C++ package manager(https://github.com/Microsoft/vcpkg)
4. create environment variables:VCPKG_ROOT=(vcpkg repo's root directory)
5. create environment variables:VCPKG_DEFAULT_TRIPLET=(current target machine's platform and instruction set architecture)   (See also:https://vcpkg.io/en/docs/users/triplets.html)
6. install SDL2:(vcpkg install sdl2)
7. install glad:(vcpkg install glad)
8. install imgui:(vcpkg install imgui[sdl2-binding,opengl3-binding])

# Screenshots
## 1.Creating an Image
![](/imgs/Snipaste_2022-03-28_03-51-57.png)
## 2.Add background
![](/imgs/Snipaste_2022-03-28_18-28-18.png)
## 3.Add a sphere
![](/imgs/Snipaste_2022-03-28_18-28-23.png)
## 4.Surface normal
![](/imgs/Snipaste_2022-03-28_22-18-33.png)
## 5.Multiple objects
![](/imgs/Snipaste_2022-04-07_03-55-52.png)
## 6.Antialiasing
![](/imgs/Snipaste_2022-04-07_04-07-29.png)

## 7.Diffuse material
Simple diffuse
![](/imgs/Snipaste_2022-04-07_20-57-03.png)
Gamma corrected diffuse
![](/imgs/Snipaste_2022-04-07_20-57-09.png)
Lambertian
![](/imgs/Snipaste_2022-04-07_20-57-23.png)
Half lambertian
![](/imgs/Snipaste_2022-04-07_20-57-29.png)

## 8.Metal material
Smooth metal
![](/imgs/Snipaste_2022-04-07_23-40-55.png)
Fuzzed metal
![](/imgs/Snipaste_2022-04-07_23-41-11.png)

## 9.Materials and post-effects
No post effect
![](/imgs/Snipaste_2022-03-28_03-53-03.png)
Sharpen
![](/imgs/Snipaste_2022-03-28_03-53-15.png)
Blur
![](/imgs/Snipaste_2022-03-28_03-53-22.png)
Edge detected
![](/imgs/Snipaste_2022-03-28_03-53-28.png)

<!-- links -->
[contributors-shield]: https://img.shields.io/github/contributors/Sqazine/RayTracingInOpenGL.svg?style=flat-square
[contributors-url]: https://github.com/Sqazine/RayTracingInOpenGL/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/Sqazine/RayTracingInOpenGL.svg?style=flat-square
[forks-url]: https://github.com/Sqazine/RayTracingInOpenGL/network/members
[stars-shield]: https://img.shields.io/github/stars/Sqazine/RayTracingInOpenGL.svg?style=flat-square
[stars-url]: https://github.com/Sqazine/RayTracingInOpenGL/stargazers
[issues-shield]: https://img.shields.io/github/issues/Sqazine/RayTracingInOpenGL.svg?style=flat-square
[issues-url]: https://img.shields.io/github/issues/Sqazine/RayTracingInOpenGL.svg
[license-shield]: https://img.shields.io/github/license/Sqazine/RayTracingInOpenGL.svg?style=flat-square
[license-url]: https://github.com/Sqazine/RayTracingInOpenGL/blob/master/LICENSE