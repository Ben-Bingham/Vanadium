# Vanadium
### A work in progress chunk based voxel rendering engine

![alt text](https://github.com/Ben-Bingham/Vanadium/raw/main/gallery/default.png "A voxel world")

### Current Features
* Can generate arbitrarily sized cubic chunks of voxels centerd on the camera
* By default voxels are generated using two dimensional perlin noise, although this can be easily customized within the C++ code
* Geometry is optimized for space as much as possible, by applying a greedy geometry creation process
* Handy ImGui settings menu to customize noise parameters and material settings
![alt text](https://github.com/Ben-Bingham/Vanadium/raw/main/gallery/settings.png "The settings menu")
* A curvature system to curve the voxel surface further away from the camera to fake the look of being on a round planet
![alt text](https://github.com/Ben-Bingham/Vanadium/raw/main/gallery/curve.png "A curved voxel world")

![alt text](https://github.com/Ben-Bingham/Vanadium/raw/main/gallery/wireframe.png "The wireframe view of a voxel world")
