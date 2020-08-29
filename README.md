# MinuteEngine
C++ framework using modern OpenGL that supports basic rendering functions and provides graphics programmingenvironment : Scene graph, Geometry, Lighting, Shading, Texturing, Shadow, etc.

<p align="center">
  <img src="/Image/main.png" width="1080" height="540">
</p>

1. Scene graph

<p align="center">
  <img src="/Image/scene_graph.gif" width="1080" height="540">
</p>

<p align="center">With simple GUI (which is implemented using ImGui), you can control objects in the scene.</p>
<p align="center">They form a hierarchy and has their own properties for rendering and transformation.</p>

2. Geometry

<p align="center">
  <img src="/Image/geometry_simple.png" width="1080" height="540">
  <img src="/Image/geometry_teapot.png" width="1080" height="540">
</p>

<p align="center">You can draw various simple objects such as cube or sphere using triangles or quads.</p>
<p align="center">Also, if you have surface information (of teapot, for example), you can draw it with triangles or quads.</p>

3. Lighting

<p align="center">
  <img src="/Image/lighting.gif" width="1080" height="540">
</p>

<p align="center">There are two basic lights implemented in MinuteEngine : Directional and Point light.</p>
<p align="center">You can change their positions, directions, and color traits at runtime.</p>


4. Shading

<p align="center">
  <img src="/Image/shading.gif" width="1080" height="540">
</p>

<p align="center">You can draw objects in the scene with various shading methods.</p>
<p align="center">You can draw them with only wireframe, or with simple color, or with materials, or with textures.</p>

5. Texturing

<p align="center">
  <img src="/Image/texture.png" width="1080" height="540">
</p>

<p align="center">You can decorate objects in the scene with marvelous textures.</p>
<p align="center">Not only diffuse and specular textures, you can also use normal maps and parallax maps.</p>

6. Shadow

<p align="center">
  <img src="/Image/shadow.gif" width="1080" height="540">
</p>

<p align="center">MinuteEngine uses shadow maps to draw shadows of objects in real time.</p>
<p align="center">When geometry of objects or lights in the scene change, shadows are updated immediately.</p>
