### PROTON SCRIPT FILES
> This Document Is Currently Just a planning document and Nothing has been added yet - 2/19/26

Proton Scripts will be used for Objects Moving them and doing other things with them such as using them as light sources, sound sources, Aplpying VFX, ect
As well as being able to use them as interactive blocks

Proton Scripts still act as C++ Code just with the help of the Proton Engine Libraries for Audio, 3d Graphics, Ect

**SYNTAX EXAMPLES**
```
ParseMap(MAPSOURCE)
String MAPSOURCE -> the location inside of the maps folder
Loads A Map and It Objects

InitRendering()
Intalizes the Rendering and Begins Rendering the Objects in the Scene

Map.GetObject(OBJECTNAME)
String OBJECTNAME -> The Name you assigned to the Object in the .pmap file
Calls a Objcet from within the currently Loaded Map

Move(OBJECT, DISTANCE);
Object OBJECT -> What Object are we moving
VEC3 DISTANCE -> How Far is it Moving in the x, y, and z directons
Moves an Object by a Distance

MoveTo(OBJECT, POSITION)
Object OBJECT -> What Object are we moving
VEC3 POSITION -> What Position
Moves an object TO a position

Shift(OBJECT, DISTANCE, MILLISECONDS);
Object OBJECT -> What Object are we shfting
VEC3 DISTANCE -> How far are we transfering said object
Float MILLISECONDS -> Time In milliseconds
Moves a object over a certain amount of time in milliseconds

ShiftTo(OBJECT, POSITION, MILLISECONDS)
Object OBJECT -> What Object are we shfting
VEC3 POSITION -> where are we shifting the Object
Float MILLISECONDS -> Time In milliseconds
Move a object over a certain amount of time in milliseconds TO a positon 

Rotate(OBJECT, ROTATION);
Object OBJECT -> The Object we are modifying
VEC3 ROTATION -> Rotation in X, Y, and Z Axes
Rotates an Object By a Rotation

RotateTo(OBJECT, ROTATION)
Object OBJECT -> The Object we are modifying
VEC3 ROTATION -> Rotation in X, Y, and Z Axes
Rotates an Object TO a Rotation

Turn(OBJECT, ROTATION, MILLISECONDS);
Object OBJECT -> The Object we are modifying
VEC3 ROTATION -> Rotation in X, Y, and Z Axes
Float MILLISECONDS -> Time In milliseconds
Rotates an Object by a Rotation over a period of time in milliseconds

TurnTo(OBJECT, ROTATION, MILLISECONDS);
Object OBJECT -> The Object we are modifying
VEC3 ROTATION -> Rotation in X, Y, and Z Axes
Float MILLISECONDS -> Time In milliseconds
Rotates an Object TO a Rotation over a period of time in milliseconds

DeactiveHitbox(HITBOX)
Hitbox HITBOX -> Hitbox to be modifyed
Turns off collison for that Hitbox

ActiveHitbox(HITBOX)
Hitbox HITBOX -> Hitbox to be modifyed
Turns on collison for that Hitbox

Audio Sound;
CLASS FOR AUDIO
        Sound.source = SOUNDSOURCE
            string SOUNDSOURCE -> Source of the Sound to Play
        Sound.speed = SPEED
            Float SPEED -> Speed that the audio clip is to be played at
        Sound.volume = VOLUME
            Float VOLUME -> Volume at which the audio is played at from its source (If = to 0 it will be played universally)
        Sound.position = POSITION
            VEC3 POSITION -> Place for audio to play from
        Sound.Play()
            Plays the Sound

VFX(TYPE, POSTION, QUANITITY, COLOR1, COLOR2, SIZE, MILLISECONDS)
String TYPE -> Predefined or file
VEC3 POSITON -> Positon for  to play VFX from
Float QUANITITY -> Amount of VFX
VEC3 COLOR1 -> First Color of VFX (WHITE = Pure Texture)
VEC3 COLOR2 -> Second Color of VFX (WHITE = Pure Texture)
Float SIZE -> Size of Particals
Float MILLISECONDS -> Time in Milliseconds of VFX PLaying
Plays a VFX Effect

UnloadMap()
Unloads the Current MAP

Class Object
    String NAME -> Name of Object
    VEC3 POSITION -> Positon of Object
    String PATH_TO_OBJECT_FILE -> Object File Location
    VEC3 ROTATION -> Rotation of Object
};
```
You can also access all types of just basic Cubes, Slopes, Meshes, ect and Manipulate those


