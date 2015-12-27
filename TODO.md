# Todo list

## Engine - Arya

- **Line strokes** for fancy looking primitives
- **Sound** Use SDL
- **Shaders**
    - Better shader system. Adding a *custom* part to a shader should not require copying/rewriting the entire shader.
- **Particle effects** If possible, support for the standard particle effect format

### Low priority

- **CommandHandler**
    - Set commands with argument and argument types
    - Argument parsing so callback gets proper arguments instead of `line`
    - Tab-completion
- **Shadows**
    - Improve speed after completing shader system
- **Skybox**
- **Text rendering** Benchmark these options
    - Per label, save a list of quads (`Arya::Geometry`) and render these every frame
    - Per label, prerender text to a texture by first generating quad geometry with `stb_truetype` and then using OpenGL Render-To-Texture
    - Per label, prerender text to a texture on CPU by using `stb_truetype` methods

## Networking

Search for C++11 networking libraries

## Game - Prismer

- **Networking integration**
    - Create event-based system where user input as well as network input goes through the same event handlers
    - Event system handles `GameEvent`s
    - Input classes like `GridInput` that handle selecting/hovering tiles (`ClientSideEvents` ?) are *not* handled in this way.
    - Instead after a tile is selected and a game event is performed then a `GameEvent` is sent to the event system.
- **Client-Server modularity**
    - Classes should be designed such that the server can compile the game without linking to the graphics engine
    - Information like `hovered` should preferably be in a client-only *component* of `Tile` or Client subclass of `Tile`.

