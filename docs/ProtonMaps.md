# PROTON ENGINE MAP LOADING

NOTE: NONE OF THIS IS MADE YET THIS IS MAINLY A PLANNING DOCUMENT

ProtonMaps or pmap files are used to store data for maps
It includes the full data needed to render the object and scripts linked to the object (if you choose to do so)

# DATA STRUCTURE
```
NAME(TYPE_OF_OBJECT, POSITION, DIMENSIONS, COLOR_MODIFYER, PATH_TO_TEXTURE, MIPMAP_LEVEL, SHINYNESS, SCRIPT)

TYPE OF OBJECT:
 Used to Clarify how to render the object, EX: if its a cube use hardcoded indices and normals because its a cube,
 TYPES:
   Cube
   Slope
   Pyramid (Planned)
   Sphere (Planned)
   2d Plane (Planned)
   3d Model (Planned)
   Object - Mutiple Objects in one special file called a .pobj (contains the info for every object and they are all linked together.)
POSITION:
  A Vec3 that contains the x, y, and z cordinates of the object
DIMENSIONS:
  A Vec3 that contains the width (x), height (y), and depth (z) of the object
COLOR_MODIFYER:
  A Vec3 Color Value that Modifys the texture by itself
PATH_TO_TEXTURE
  The Path to the texture that its using
  Note: You can specify 'NONE' and it will just render the colored object instead (planned)
MIPMAP_LEVEL
  How many times th texture repeats on the object
SHINYNESS
  Tells the shader how much specullar strength to apply to the object
SCRIPT
  the path to the script used for movement, npcs, ect
  NOTE: You can specify 'NONE' to just have a static Object
```

