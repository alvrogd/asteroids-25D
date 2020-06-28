# Asteroids 2.5D

This project is a **game concept of a tribute to the classic Asteroids game** (Atari, 1979), **written in C++ and OpenGL**.

At first, it was intended to be a game where, just as in the original Asteroids, **you must destroy every asteroid that you see, as well as dodge them in order to avoid destroying your spaceship**. To that end, the spaceship can shoot energy blasts at your command, that are able to destroy the asteroids, but be cautious, as colliding with an asteroid will turn out to a game over! Be also aware that large asteroids will spawn smaller ones upon exploding. 

The project's background is also relevant to better understand why it was not completed. At the time, I was attending a subject where I was learning about computer graphics, and I was proposed to create a small application in which I could prove all the acquired knowledge. Therefore, making a small game would surely be a suitable final project. However, **time told me that I had made a choice that was too ambitious, as all I could do was just to build this prototype that I have now released on GitHub, just if anybody is interested in taking a look at it**.

Be aware that **I was learning both OpenGL and C++ while building it** (with prior knowledge of C), **so the code in this repository is definitely not intended to be a reliable reference on how to use any of those two languages**.

## Why is it called _2.5D_?

As it was intended to be a tribute to the original game, **I chose to make a 3D game, so that it was more suited to the type of games that we are used to nowadays, along with a greater player immersion**, instead of keeping the traditional top-down view.

However, I also chose to **preserve the limitation that the spaceship can only move on a single plane**. My motivation was that, if I allowed 3D movement, the player would need to constantly be watching out for asteroids that could approach from both above and below, which would considerably increase the game's difficulty. Another concern that would certainly arise is that, if asteroids could move in a 3D space, they could "block" the camera's view sometimes, therefore increasing even more the difficulty.

Hence, by limiting the spaceship's movement to a single plane, the player can be aware of its surroundings without too much of a hassle, as well as move at full speed without worrying too much about crashing into any asteroid.

## Getting started

### So, what does this prototype offer?

**Once you run the executable, the set up phase will probably take a few seconds**, as it needs to initialize multiple libraries and load certain resources from disk. **After that, you will be free to completely drive your spaceship!**.

#### Controls

The **controls** are quite easy:

* **W:** the spaceship speeds up in the direction that it is facing. Be aware that there is no way to slow down, so you could easily crash into an asteroid if you drive carelessly!
* **A:** the spaceship turns left.
* **D:** the spaceship turns right.
* **Space:** the spaceship shoots a burst of energy blasts that can destroy asteroids.
* **F1:** the camera is placed behind and next to the spaceship. This alternative leads to a greater difficulty, as you cannot be that much aware of your surroundings.
* **F2:** the camera is placed far behind the spaceship. This is the default mode.

#### What if I crash into an asteroid?

In such a case, **the spaceship will surely disintegrate**, meaning game over. However, you can always **reset the game's state by pressing the R key**; in fact, you can also use it at any time.

![Gameplay demo](asteroids-25D-gp.gif)

### So, what is this prototype missing compared with what you originally intended to build? 

On the one hand, **there are a few missing key features that surely make a difference between a game concept and an actual entertaining game**:

* It needs a **score counter**, built directly into the main window.
* Along with this counter, **the player would also have a limited quantity of lives** that would have to be depleted so that the destruction of the spaceship implies **a game over**.
* Thus, **the player would now have the feeling that a game over means that he loses a certain game progress**, in hopes that he would try to achieve the maximum possible score.

On the other hand, **some performance optimizations would surely come handy to increase the overall performance**:

* Surely there will be many places that seem as good candidates to be optimized, but a specific example would be to embrace an _instancing_ strategy when rendering the game's particles, as the current approach leads up to a quite noticeable bottleneck. Some resources on this matter are:
    * http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
    * https://learnopengl.com/Advanced-OpenGL/Instancing

### Getting the prototype

**This project has been built using Windows, and it also is the only platform where I have tested it**. There are two ways to get an executable in order to try it out.

#### Downloading the available binaries

1. You can **download the binaries of the latest release** right in the [project's releases page on GitHub](https://github.com/alvrogd/asteroids-25D/releases).
2. After unpacking them, you just need to **run the `asteroids-25D.exe` executable** and you are done!
3. Any required external files are already put in the right place.

#### Building it yourself

1. **Download or clone the repository**.

2. All the **C++ source files** that make up the project are available in the **`asteroids-25D/src` folder**.

3. **In order to generate the executable, you will need multiple third-party libraries**. Their corresponding files (just for Windows) are available in the `dependencies` folder.
    1. Each library contains an `include` folder which should be available as an additional include directory for the compiler. The GLM library is an exception, as its include folder is named `glm`.
    2. In order to link some libraries, their corresponding `.lib` files should be visible to the linker. In particular, they are from the Assimp, GLFW and irrKlang libraries.
    3. Last but not least, you will also need to explicitly compile the `dependencies/GLAD/src/glad.c` along with all the other source files of the project.

4. After successfully generating the executable, **you just need to make sure that certain contents are available exactly in the directory from where it will be run**:
    1. The `assimp-vc140-mt.dll` and `irrKlang.dll` files, from the Assimp and irrKlang libraries respectively.
    2. The `resources` folder, which contains all used graphical and sound resources.
    3. The `asteroids-25D/shaders` folder, which contains the source code for the OpenGL shader programs that will be used during the rendering process (read and compiled at runtime).

## Built with

* [C++](https://isocpp.org/)
* [OpenGL 3.3](https://www.opengl.org/)
* [GLSL 3.3](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)) - The language in which the shader programs are built.

### Third-party libraries

* [Assimp](https://github.com/assimp/assimp) - In order to load 3D models.
* [glad](https://github.com/Dav1dde/glad) - An OpenGL functions loader.
* [GLFW](https://github.com/glfw/glfw) - Which provides an API for creating windows and receiving inputs/events from the user.
* [GLM](https://github.com/g-truc/glm) - Which makes handy GLSL functions also available in C++.
* [irrKlang](https://www.ambiera.com/irrklang/) - In order to play any sounds.
* [stb](https://github.com/nothings/stb) - Which eases reading images from disk.

### Resources

* **Graphics:**
    * [Asteroids](https://www.cgtrader.com/free-3d-models/exterior/landmark/2600-lowpoly-rock).
    * [Skybox](https://spacedock.info/mod/926/Pood%27s%20Milky%20Way%20Skybox).
    * [Spaceship](https://www.turbosquid.com/FullPreview/Index.cfm/ID/465782).

* **Sounds:**
    * [Background music](https://nicoleproducer.itch.io/free-chiptune-song-galactik-funk).
    * [Object disintegration](https://www.youtube.com/watch?v=anBOuaxwDZI).
    * [Spaceship's shots](https://www.zapsplat.com/music/laser-rapid-fire-3/).

And **a huge thanks to one of the best learning resources that I have ever come across: [Learn OpenGL](https://learnopengl.com/)**.

## Authors

* **Álvaro Goldar Dieste** - [alvrogd](https://github.com/alvrogd)
