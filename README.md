# MinuteEngine


<p align="center">
  <img src="/Image/main.png" width="1080" height="540">
</p>

**C++ framework using modern OpenGL that supports basic rendering functions and provides graphics programming environment : Scene graph, Geometry, Lighting, Shading, Texturing, Shadow, etc**.

# Scene Graph
With simple GUI (which is implemented using [ImGui](https://github.com/ocornut/imgui)), user can control objects located in the scene. The objects in the scene can form a hierarchy and render calls follow the structure. Each object in the scene has its own properties for rendering and transformation. 

<p align="center">
  <img src="/Image/scene_graph.gif" width="1080" height="540">
</p>

# Geometry
User can draw various simple geometric objects - like cube and sphere - using triangles or quads. Also, if user has surface information (of teapot, for example), the user can render it with triangles or quads.

<p align="center">
  <img src="/Image/geometry_simple.png" width="1080" height="540">
  <img src="/Image/geometry_teapot.png" width="1080" height="540">
</p>

# Lighting
There are two basic lights implemented in this framework : directional and point light. User can change a light's position, direction, and color traits at runtime.
<p align="center">
  <img src="/Image/lighting.gif" width="1080" height="540">
</p>

# Shading
User can render objects in the scene with various shading methods. The user can render them with only wireframe, or with simple color, or with materials, or with textures.
<p align="center">
  <img src="/Image/shading.gif" width="1080" height="540">
</p>

# Texturing
User can decorate objects with fancy textures. The user can use diffuse, specular, normal, and parallax textures to render his/her objects.
<p align="center">
  <img src="/Image/texture.png" width="1080" height="540">
</p>

# Shadow
This framework uses shadow maps to render shadows of objects in real time. When geometry of objects or lights in the scene change, shadows are updated immediately.
<p align="center">
  <img src="/Image/shadow.gif" width="1080" height="540">
</p>
