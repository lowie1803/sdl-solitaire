# sdl-solitaire

## Dependencies

- [Git][]
- C Compiler (gcc, ...)
- [CMake][]
- [SDL2][SDL] library
- [SDL2_gfx][] library

**On Debian/Ubuntu based distributions, use the following command:**

```sh
sudo apt install git build-essential pkg-config cmake cmake-data libsdl2-dev libsdl2-gfx-dev
```

**Optional packages:**

- [SDL2_image][] library
- [SDL2_ttf][] library
- [SDL2_net][] library
- [SDL2_mixer][] library

```sh
sudo apt install libsdl2-image-dev libsdl2-ttf-dev libsdl2-net-dev libsdl2-mixer-dev
```

## Build instructions

```sh
# Clone this repo
git clone https://gitlab.com/aminosbh/sdl-solitaire-2215.git
cd sdl-solitaire-2215

# Create a build folder
mkdir build
cd build

# Build
cmake ..
make

# Run
./sdl-solitaire-2215
```

***Note:*** To use SDL2_image, SDL2_ttf, SDL2_net or SDL2_mixer, you should
uncomment some instructions in the CMakeLists.txt file and re-execute the
`make` command.

### Open the project with an IDE under Linux

See [IDE_USAGE.md](IDE_USAGE.md) for details.

# MIDTERM DEMO: [](https://drive.google.com/file/d/1jtKv1SgVjiiGc9kZXDYYy0ZOPvjfIDzA/view?usp=sharing)
