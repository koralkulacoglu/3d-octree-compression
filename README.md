# 3D Octree Compression

A C++ implementation of 3D image compression using octree data structures, with OpenGL visualization.

## Dependencies

- C++17 compiler (`g++` or `clang++`)
- CMake 3.10+
- OpenGL 4.6
- GLFW3
- GLM
- GLAD (OpenGL loader)

### Install on Ubuntu/WSL

```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libglfw3-dev libglm-dev
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
```

## Setup

### Clone the repository

```bash
git clone <your-repo-url>
cd 3d-octtree-compression
```

### Generate GLAD files

```bash
python3 -m venv venv
source venv/bin/activate
pip install glad
python -m glad --profile=core --api=gl=4.6 --generator=c --out-path=.
```

### Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Run

```bash
./build/OctreeViewer
```

## Controls

- `WASD` — Move camera
- Mouse — Look around
- `M` — Toggle mouse capture
- `ESC` — Exit

## WSL Display Setup

If using WSL, ensure you have an X server running (e.g., VcXsrv or WSLg), then set the display:

```bash
export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0.0
```

## Rebuild

```bash
cd build
rm -rf * && cmake .. && make -j$(nproc)
```
