# PROTON ENGINE MAP LOADING

NOTE: THIS IS CURRENT A IN PROGRESS PROJECT AND SOMETHING IN THIS DOCUMENT ARE PURELY FOR PLANNING

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
SCRIPT (Planned)
  the path to the script used for movement, npcs, ect
  NOTE: You can specify 'NONE' to just have a static Object
```

EXAMPLE

```
"NAME": "cube1",
    "TYPE_OF_OBJECT": "CUBE",
    "X": 0.0,
    "Y": 0.0,
    "Z": 0.0,
    "W": 1.0,
    "H": 1.0,
    "D": 1.0,
    "R": 1.0,
    "G": 1.0,
    "B": 1.0,
    "PATH_TO_TEXTURE": "cat.jpg",
    "MIPMAP_LEVELX": 1.0,
    "MIPMAP_LEVELY": 1.0,
    "MIPMAP_LEVELZ": 1.0,
    "SHINYNESS": 0.1,
    "ROTATION_X": 0.0,
    "ROTATION_Y": 0.0,
    "ROTATION_Z": 0.0
```

# .pobj FILES - PROTON ENGINE OBJECTS

A Proton Engine Object file (or a .pobj file) Is used for storing the data of multiple objects that you want fused together
EXAMPLE OF A .pboj FILE
```
{
  NAME: Object1
  Objects: {
  [ObjectInfo]
  [ObjectInfo]
  [ObjectInfo]
  }
}
```

## Proton Engine Object Attributes


```
"NAME": "ProtonObject",
"TYPE_OF_OBJECT": "OBJ",
"OBJECT_SOURCE": "Object1.pobj"
"X": 0.0,
"Y": 0.0,
"Z": 0.0,
"ROTATION_X": 0.0,
"ROTATION_Y": 0.0,
"ROTATION_Z": 0.0
```

