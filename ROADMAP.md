# Roadmap

## `Milestone I (basic math and utils)`

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

## `Milestone II (basic core)`

- ### `basic core`

  - [x] `engine`
    ```
    game loop
    frame timers
    core initialization
    ```

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

  - [x] `basic render system`
    ```
    OpenGL only
    index and vertex buffers, render state,
    textures, blends, pixel and vertex shaders
    ```

  - [x] `virtual file system`
    ```
    path aliases, archives, async loading
    ```

## `Milestone III (basic high)`

- ### `basic high`

  - [ ] `library`
    ```
    resource management, abstract loaders
    groups, dependencies, async loading
    ```

  - [ ] `resources`
    ```
    meshes, shaders, textures, materials
    ```

  - [ ] `scene graph`
    ```
    nodes, scenes
    parent child relationship, bounding boxes, event bubbling
    ```

## `Milestone IV (advanced high)`

- ### `advanced high`

  - [ ] `sprites`
    ```
    static and frame-by-frame animations
    ```

  - [ ] `ECS`
    ```
    basic entity component system
    component processing and joins
    ```

  - [ ] `animations`
    ```
    tweeners and spine runtime
    ```

  - [ ] `labels`
    ```
    ttf, bmfont
    ```

  - [ ] `scripting`
    ```
    lua runtime and bindings
    ```
