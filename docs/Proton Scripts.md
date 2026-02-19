### PROTON SCRIPT FILES
> This Document Is Currently Just a planning document and Nothing has been added yet - 2/19/26

Proton Scripts will be used for Objects Moving them and doing other things with them such as using them as light sources, sound sources, Aplpying VFX, ect
As well as being able to use them as interactive blocks

Proton Scripts still act as

**SYNTAX EXAMPLES**
```
#include <Shapes.h>
#include <Models.h>
#include <MapParser.h>
#include <AudioController.h>
#include <VFX.h>
#include <Scripts.h>

// Import Command
ParseMap("map.pmap"); // Imports All Objects into the current Scene from the Map

InitRendering(); // Loads the Rendering Script and Begins Rendering Everything

Object Object1 = Map.GetObject("CUBE1");

// Movement
Move(Object1, 0.0f, 0.0f, 10.0f); // Move(OBJECT, XDISTANCE, YDISTANCE, ZDISTANCE);
Shift(Object1, 0.0f, 0.0f, 10.0f, 1000.0f); // Shift(OBJECT, XDISTANCE, YDISTANCE, ZDISTANCE, MILLISECONDS);

Rotate(Object1, 0.0f, 180.0f, 0.0f); // Rotate(OBJECT, XROTATION, YROTATION, ZROTATION);
Turn(Object1, 0.0f, 270.0f, 0.0f, 500.0f); // Turn(OBJECT, XROTATION, YROTATION, ZROTATION, MILLISECONDS);

// Hitbox Manipulation

Hitbox Hitbox1 = Object1.Hitbox;
Object Sword = Map.GetObject("SWORD");
Hitbox SwordHitbox = Sword.Hitbox;

Int playerHealth;

Camera player;

player.position = glm::vec3(6.0f, 3.0f, 5.0f);
player.yaw = 0.0f;
player.pitch = 0.0f;
player.hitbox.position = glm::vec3(player.position.x, (player.position.y - 0.6f), player.position.z);
player.hitbox.dimensions = glm::vec3(0.1f, 0.7f, 0.1f);
player.hitbox.dir = glm::vec3(player.pitch, player.yaw, 0.0f);
player.last_position = player.position;
player.yvelocity = 0;

Move(Hitbox1, 0.0f, 0.0f, 10.0f) // Move(OBJECT, XDISTANCE, YDISTANCE, ZDISTANCE);
Shift(Hitbox1, 0.0f, 0.0f, 10.0f, 1000.0f) // Shift(OBJECT, XDISTANCE, YDISTANCE, ZDISTANCE, MILLISECONDS);

Rotate(Hitbox1, 0.0f, 180.0f, 0.0f) // Rotate(OBJECT, XROTATION, YROTATION, ZROTATION);
Turn(Hitbox1, 0.0f, 270.0f, 0.0f, 500.0f) // Turn(OBJECT, XROTATION, YROTATION, ZROTATION, MILLISECONDS);

Deactive(Hitbox1); // Turns off collison for that Hitbox
Active(Hitbox1); // Turns on collison for that Hitbox

bool Game = True;

Audio hurtSound;

hurtSound.source = "hurt.mp3"
hurtSound.speed = 1.0f
hurtSound.volume = 1.0f

while(Game) {
    if (HitboxIntersect(Player.Hitbox, SwordHitbox)) {
        playerHealth--;
        std::cout << "Player Hit!\n";
        hurtSound.play();

        VFX("Fire", Player.position, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.4f, 0.0f), 500.0f) // VFX(TYPE, POSTION, STRENGTH, COLOR1, COLOR2, MILLISECONDS)
    }
}

UnloadMap(); // Unloads all currently existing objects
```

Now Lets go over all the Synatx Planned (2/19/26)

```
ParseMap(MAPSOURCE)
String MAPSOURCE -> the location inside of the maps folder
Loads A Map and It Objects

InitRendering()
Intalizes the Rendering and Begins Rendering the Objects in the Scene

Map.GetObject(OBJECTNAME)
String OBJECTNAME -> The Name you assigned to the Object in the .pmap file
Calls a Objcet from within the currently Loaded Map


```


