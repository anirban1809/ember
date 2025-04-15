# Ember

Ember is a basic 3d Game Engine written purely in C++. The Graphics APIs currently supported are OpenGL and the OpenGL Shading Language (GLSL).

### Current Status
Ember currently is in very initial stages of development and is not in a state of distribution. The basic support for the following features is currently available.

 - Scene Viewport
 - Object Mesh Loading (.obj only)
 - Cubemaps
 - Perspective Camera
 - Point lighting
 - Entity Component System
 - Loading multiple static meshes
 - Multiple Scene lighting
 - Transformation for Static Meshes(Position, Rotation, Scale)

### In Development
The following features are currently under development

 - Material systems
 - Loading/Updating Static Meshes at Runtime.

### Building and running Ember
Build dependencies are only available for mac os as I am using a mac to develop this. Support for Visual Studio solutions and Windows builds will be added soon.

All the required dependencies are bundled in a vendor folder. So no additional setup for the dependencies would be necessary.

**Note:* The code uses some hardcoded file paths which have to be replaced before building.*

Once cloned, prepare the Makefiles using CMake:

    cmake -S . -B ./build

Then to build, run:

    cmake --build ./build

The executables will be created in the `build` directory.

<img width="1987" alt="image" src="https://github.com/user-attachments/assets/75d7bb2a-83c4-4b68-80b2-fc14f0071066" />
