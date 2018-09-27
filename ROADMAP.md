# Roadmap

## `Milestone I`

- ### `basic math`
  - [x] `vec2, vec3, vec4`
  - [x] `mat2, mat3, mat4`
  - [x] `unit<T,Tag>`
  - [x] `basic math functions`
    ```
    clamp, min, max, approximately etc.
    ```
  - [x] `basic trigonometric functions and constants`
    ```
    pi, sin, cos, to_deg, to_rad etc.
    ```

- ### `basic utils`
  - [x] `path`
    ```
    functions to manipulating paths in a portable way
    ```
  - [x] `color, color32`
  - [x] `basic string functions`
    ```
    unicode convertions, wildcard patterns, type safe format
    ```
  - [x] `timer and time functions`
  - [x] `image, image loaders`
    ```
    basic true color formats
    internal: g8, ga8, rgb8, rgba8
    external: png, jpg, tga
    ```
  - [x] `streams and native file system`
  - [x] `thread pool and async tasks`

## `Milestone II`

- ### `basic core`
  - [x] `logger`
    ```
    levels, sinks
    ```
  - [x] `basic input system`
    ```
    events, listeners, polling
    devices: mouse, keyboard
    ```
  - [x] `basic window system`
    ```
    GLFW, Windows, Linux, Mac OS X
    windowed and fullscreen modes
    ```
  - [ ] `basic render system`
    ```
    OpenGL only
    index and vertex buffers, render state,
    textures, blends, pixel and vertex shaders
    ```
  - [x] `virtual file system`
    ```
    path aliases, archives, async loading
    ```
  - [ ] `basic resource manager`
    ```
    groups, dependencies, async loading
    ```

## `Milestone III`

```
Coming Soon...
```
