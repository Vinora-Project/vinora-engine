# Vinora Project Conventions
(Modified from raylib's conventions)

In Vinora Project we use C99. We would use C89 (for a lot of reasons),
but raylib uses C99, so we kinda concede here.

Still, we have no respect for so called "technical standards" here.
_Compatibility_ is our sacred cow, especially backwards compatibility.

## Banned Technologies

1. **VLA**: Just don't. [Please](https://lkml.org/lkml/2018/3/7/621).
> AND USING VLA'S IS ACTIVELY STUPID!
> -- Linus Torvalds

2. **Non-ASCII characters in source, comments, and string literals**
*Compatibility above everything.* If it is not in ASCII, it is not
in the program text.
You can use whatever you want on *your* machine, but we have no right
to expect that people have Unicode support for system paths and
editors.
P.S. But *users* could use Unicode in Vinora Screenplay, since **we**
grant them this ability with Vinora Engine.

3. That's it for now. We will add more when something new makes us
angry enough to write it down.


## General Conventions
1. Lines **MUST NOT** exceed **80 characters** (in **ANY** file).
2. Files and directories are named in **snake_case**. For important
files, use **ALL_CAPS**.
3. All names, comments, and string literals are **ASCII-only**.
Unicode is allowed only in `.vnrs` and `.conf` files (user text).
4. **User's time > developer's time.**
So, if we have a choice between making the developer's life easier
or making the user's life easier, we **ALWAYS** choose the second
option.


## Code Conventions
1. **ALWAYS** initialize all defined variables.
2. **Do not use TABS**, use 4 spaces instead.
3. Avoid trailing spaces, please, avoid them.
4. Control flow statements are always followed **by a space**.
5. Opening `{` goes on a **new line only for functions**. For `if`,
`while`, `for`, `switch`, `struct`, `enum` it stays on the header
line. Closing `}` gets its own line, under the header.
6. **Never** put a statement body on the same line as its header.
`if (x) return;` is wrong. The debugger steps lines, not
semicolons.
7. Wrapped continuations take **one extra indent** (4 spaces). Do
not hang-align arguments to the opening `(`. Function calls are
not blocks: do not indent the lines between `BeginDrawing();`
and `EndDrawing();` extra, they are ordinary statements.

### Code Style Conventions

It's mostly the same as raylib, **except enum members**.
If you're an old-school C developer, pls look carefully: these
conventions may be unusual for you.

Code element | Convention       | Example
---          | :---:            | ---
Defines      | ALL_CAPS         | `#define PLATFORM_DESKTOP`
Macros       | ALL_CAPS         | `#define MIN(a,b) (((a)<(b))?(a):(b))`
Variables    | lowerCase        | `float targetFrameTime = 0.016f;`
  local      | lowerCase        | `Vector2 playerPosition = {0};`
  global     | lowerCase        | `bool windowReady = false;`
Constants    | lowerCase        | `const int maxValue = 8;`
Pointers     | MyType \*pointer | `Texture2D *array = NULL;`
Float values | always x.xf      | `float gravity = 10.0f`
Operators    | value1 * value2  | `int product = value * 6;`
Operators    | value1 / value2  | `int division = value / 4;`
Operators    | value1 + value2  | `int sum = value + 10;`
Operators    | value1 - value2  | `int res = value - 5;`
Enum         | TitleCase        | `enum TextureFormat`
- *members*  | *snake_case*     | `pixelformat_uncompressed_r8g8b8`
Struct       | TitleCase        | `struct Texture2D`, `struct Material`
- members    | lowerCase        | `texture.width`, `color.r`
Functions    | TitleCase        | `InitWindow()`, `LoadImageFromMemory()`
Parameters   | lowerCase        | `func(int screenWidth, int screenHeight)`


### Code example
```c
if (condition)
    value = 0;

while (!WindowShouldClose()) {
    /* ... */
}

for (int i = 0; i < NUM_VALUES; i++)
    printf("%i", i);

switch (value) {
    case 'a':
        // Some code
        break;
    case 'b':
        // Also code
        break;
    default:
        def();
        break;
}
```
 - All condition checks are **always between parenthesis**, but not
 boolean values:
```c
if ((value > 1) && (value < 50) && valueActive) {
    /* ... */
}
```
 - Function braces are the exception: `{` on the next line, in the
 same column as the header. Never indent that `{`.
```c
void SomeFunction()
{
    // TODO: Do something here!
}
```
 - `else` stays with the closing brace of the `if`:
```c
if (ok) {
    go();
} else {
    stop();
}
```

**If proposing new functions, please try to use a clear naming for
the function and its parameters. In case of doubt, open an issue
for discussion.**

## Files and Directories Naming Conventions

  - Directories will be named using `snake_case`:
`assets/fonts`, `src/vnrs`

  - Files will be named using `snake_case`:
`font_manager.c`, `mplus_regular.ttf`

_NOTE: Avoid any spaces, special or non-ASCII characters in the
files/dir naming!_

## Asset directories

 - Data files should be organized by context and usage. Put things
 that load together in the same place.
 - Use descriptive names. Reading the filename should be enough to
 guess what the file is.
 - Here is the current tree, in broad strokes:

```
assets/alice.vnrs
assets/hamlet.vnrs
assets/i18n.vnrs
assets/lorem.vnrs
assets/fonts/mplus_regular.ttf
assets/fonts/unifont.otf
assets/spiral_atlas/bedroom.jpg
assets/spiral_atlas/lounge.jpg
assets/sumi/smile.png
```

NOTE: all of this applies only for developing versions. In a release
build, files should be embedded into the binary.
