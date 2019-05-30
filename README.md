# Mesh_Collishion_Detection

Written by: Reut Shmuel

Based on the engine of my lecturer Tamir Grossinger: https://github.com/tamirgrBGU/EngineVideoGames

Implementation in openGL.

Detects collision between two 3D objects (represented by mesh) using KD-tree to create Bounding Volume Hierarchy (BVH) for every shape in the scene.

The simulation always renders:
- Two objects (one is moving and rotating - controlled by the arrows) with full mesh representation.
- The biggest bounding box of each object.

When the two objects collide, the simulation stops the moving object and show the smallest bounding boxes which collide.
