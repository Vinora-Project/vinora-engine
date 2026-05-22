# Vinora Project Conventions
(Modified from raylib's conventions)

In Vinora Project we use C99. We would use C89 (for a lot of reasons), but
raylib use C99, so we kinda concede here.

Still, we have no respect for so called "technical standarts" here.  
_Compability_ is our sacred cow, especially backwards compatibility.

## Banned Technologies

1. **VLA**: Just don't. [Please](https://lkml.org/lkml/2018/3/7/621).  
> AND USING VLA'S IS ACTIVELY STUPID!  
> -- Linus Torvalds

2. **Non-ASCII characters in source code and file paths**  
*Compatibility above everything.*  
You can use whatever you want on *your* machine, but we have no right to 
expect that people have Unicode support for system paths and editors.  
P.S. But *users* could use Unicode in Vinora Screenplay, since **we**
grant them this ability with Vinora Engine.

3. **To be continiued...**


## General Conventions
1. Lines **MUST NOT** exceed **80 characters** (in **ANY** file).
2. Files and directories are named in **snake_case**. For important files, use
**ALL_CAPS**
3. All names and code are **ASCII-only**. Unicode and other encodings are 
allowed only in .vnrs and .conf files (which are created by user).
4. **User's time > developer's time.**  
So, if we have a choice between making developer's life easier or making 
user's life easier, we **ALWAYS** choose the second option.


## Code Conventions
1. **ALWAYS** initialize all defined variables.
2. **Do not use TABS**, use 4 spaces instead.
3. Avoid trailing spaces, please, avoid them
4. Control flow statements always are followed **by a space**

### Code Style Conventions

It's mostly the same as raylib, **except enum members**.  
If you're an old-school C developer, pls look carefully: this conventions 
may be unusual for you.  

Code element | Convention       | Example
---          | :---:            | ---
Defines      | ALL_CAPS         | `#define PLATFORM_DESKTOP`
Macros       | ALL_CAPS         | `#define MIN(a,b) (((a)<(b))?(a):(b))`
Variables    | lowerCase        | `float targetFrameTime = 0.016f;`
- local      | lowerCase        | `Vector2 playerPosition = { 0 };`
- global     | lowerCase        | `bool windowReady = false;`
Constants    | lowerCase        | `const int maxValue = 8;`
Pointers     | MyType \*pointer | `Texture2D *array = NULL;`
Float values | always x.xf      | `float gravity = 10.0f`
Operators    | value1*value2    | `int product = value*6;`
Operators    | value1/value2    | `int division = value/4;`
Operators    | value1 + value2  | `int sum = value + 10;`
Operators    | value1 - value2  | `int res = value - 5;`
Enum         | TitleCase        | `enum TextureFormat`
- *members*  | *snake_case*     | `pixelformat_uncompressed_r8g8b8`
Struct       | TitleCase        | `struct Texture2D`, `struct Material`
- members    | lowerCase        | `texture.width`, `color.r`
Functions    | TitleCase        | `InitWindow()`, `LoadImageFromMemory()`
Parametrs    | lowerCase        | `func(int screenWidth, int screenHeight)`


### Code example 
```c
if (condition) value = 0;

while (!WindowShouldClose())
{

}

for (int i = 0; i < NUM_VALUES; i++) printf("%i", i);

// Be careful with the switch formatting!
switch (value)
{
    case 'a':
        // Some code
        break;
    case 'b':
        // Also code.
        break;
    default: def(); break; // One-line
}
```
 - All conditions checks are **always between parenthesis** but not boolean 
 values:
```c
if ((value > 1) && (value < 50) && valueActive)
{

}
```
 - When dealing with braces or curly brackets, open-close them in aligned
mode:
```c
void SomeFunction()
{
   // TODO: Do something here!
}
```

**If proposing new functions, please try to use a clear naming for 
function-name and functions-parameters, in case of doubt, open an issue 
for discussion.**

## Files and Directories Naming Conventions

  - Directories will be named using `snake_case`:
`resources/models`, `resources/fonts`
  
  - Files will be named using `snake_case`:
`main_title.png`, `cubicmap.png`, `sound.wav`
  
_NOTE: Avoid any spaces, special or non-ASCII characters in the 
files/dir naming!_
  
## Games/Examples Directories Organization Conventions
  
 - Data files should be organized by context and usage in the engine, think
about the loading requirements for data and put all the resources that need
to be loaded at the same time together.
 - Use descriptive names for the files, it would be perfect if just reading
the name of the file, it was possible to know what is that file and where 
fits in the game.
 - Here is an example, note that some resources require to be loaded all 
at once whileother require to be loaded only at initialization (gui, font).
  
```
resources/audio/fx/long_jump.wav
resources/audio/music/main_theme.ogg
resources/screens/logo/logo.png
resources/screens/title/title.png
resources/screens/gameplay/background.png
resources/characters/player.png
resources/characters/enemy_slime.png
resources/common/font_arial.ttf
resources/common/gui.png
```

NOTE: all of this applies only for developing versions. In release version,
all of file should be embedded in to binary.
