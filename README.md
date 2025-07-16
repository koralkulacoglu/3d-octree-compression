# 3D Octree Compression

A C++ implementation of 3D image compression using octree data structures, with OpenGL visualization.

<p align="center">
  <img src="https://github.com/user-attachments/assets/bfb98448-b7a4-4b69-8182-5201847749fa" alt="bunny_gif" width="49.7%" />
  <img src="https://github.com/user-attachments/assets/70507d24-57bd-4922-ac48-ced328385b10" alt="bunny_all_nodes" width="45%" />
</p>

# How it Works

Octree compression works by recursively subdividing a 3D image into eight smaller cubes (octants). If all voxels within an octant have similar or identical values, the region is represented by a single node instead of individual voxels. This reduces the amount of data by efficiently summarizing uniform areas, allowing for compact storage and faster processing.

<p align="center">
  <img width="507" height="188" alt="image" src="https://github.com/user-attachments/assets/b7ccb38a-95a3-44cb-ac2c-31cfd53cc35f" />
</p>

# Local Setup

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
