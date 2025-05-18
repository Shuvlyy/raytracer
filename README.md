# OOP-400: `Raytracer`
###### An <img src="https://newsroom.ionis-group.com/wp-content/uploads/2023/09/epitech-2023-logo-m.png" alt="Epitech" height=18/> project

---

## Project

Ray tracing is a technique used to generate realistic digital images by
simulating the inverse path of light. Your goal is to create a program able to
generate an image from a file describing the scene.

## 📩 Installation

1. Clone the project
```shell
git clone git@github.com:EpitechPromo2028/B-OOP-400-REN-4-1-raytracer-pierre.montaret.git raytracer
cd raytracer/
```

2. Build the project
```sh
make -s
```

3. Launch the project using `./raytracer`
   If you need any help, `--help` flag is for you!

## Features

Here is the marking scheme of the project:

- Primitives
  - [x] Sphere
  - [x] Plane
  - [x] Cylinder
  - [x] Cone
  - [x] Limited cylinder (0.5)
  - [x] Limited cone (0.5)
  - [ ] Torus (1)
  - [ ] Tanglecube (1)
  - [x] Triangle (1)
  - [x] `.obj` file (1)
  - [ ] Fractal (2)
  - [ ] Möbius strip (2)
- Transformations
  - [x] Translation
  - [x] Rotation
  - [ ] Scale (0.5)
  - [ ] Shear (0.5)
  - [ ] Transformation matrix (2)
  - [ ] Scene graph (2)
- Lights
  - [x] Directional light
  - [x] Ambient light
  - [x] Drop shadows
  - [x] Multiple directional lights (0.5)
  - [x] Multiple point lights (1)
  - [x] Colored light (0.5)
  - [ ] Phong reflection model (2)
  - [ ] Ambient occlusion (2)
- Materials
  - [x] Flat color (lambertian)
  - [ ] Transparency (0.5)
  - [ ] Refraction (1)
  - [x] Reflection (0.5)
  - [ ] Texturing from file (1)
  - [ ] Texturing from procedural generation of chessboard (1)
  - [ ] Texturing from procedural generation of Perlin noise (1)
  - [ ] Normal mapping (2)
- Scene configuration
  - [x] Add primitives to scene
  - [x] Set up lighting
  - [x] Set up camera
  - [ ] Import a scene in a scene (2)
  - [x] Set up antialiasing through supersampling (0.5)
  - [ ] Set up antialiasing through adaptative supersampling (1)
- Interface
  - [x] No GUI, output to a PPM file
  - [x] Display the image during and after generation (1)
  - [x] Exit during or after generation (0.5)
  - [ ] Scene preview using a basic and fast renderer (2)
  - [ ] Automatic reload of the scene at file change (1)
- Optimizations
  - [ ] Space partitionning (2)
  - [x] Multithreading (1)
  - [x] Clustering (3)

## 👷🏻 Team

| [**Lysandre BOURSETTE**](https://github.com/shuvlyy)<br/><img src="https://avatars.githubusercontent.com/u/123988037?v=4" width=92> | [**Pierre MARGUERIE**](https://github.com/u/146085057)<br/><img src="https://avatars.githubusercontent.com/u/146085057?v=4" width=92> | [**Pierre MONTARET**](https://github.com/u/90644687)<br/><img src="https://avatars.githubusercontent.com/u/90644687?v=4" width=92> |
|-------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------|
| [lysandre.boursette@epitech.eu]("mailto:lysandre.boursette@epitech.eu")                                                             | [pierre.marguerie@epitech.eu]("mailto:pierre.marguerie@epitech.eu")                                                                   | [pierre.montaret@epitech.eu]("mailto:pierre.montaret@epitech.eu")                                                                  |

[![wakatime](https://wakatime.com/badge/user/2f50fe6c-0368-4bef-aa01-3a67193b63f8/project/2100da67-461a-4298-b599-18c8ec1fca67.svg)](https://wakatime.com/badge/user/2f50fe6c-0368-4bef-aa01-3a67193b63f8/project/2100da67-461a-4298-b599-18c8ec1fca67) + [![wakatime](https://wakatime.com/badge/user/2f50fe6c-0368-4bef-aa01-3a67193b63f8/project/3db8bf84-d727-477b-b028-57c54adf3ac3.svg)](https://wakatime.com/badge/user/2f50fe6c-0368-4bef-aa01-3a67193b63f8/project/3db8bf84-d727-477b-b028-57c54adf3ac3)
