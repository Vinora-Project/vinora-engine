# Vinora Engine

Vinora Engine is a visual novel engine with a focus on *simplicity* and
*portability*.
It doesn't use any scripting language (like Python in Ren'Py),
but instead it uses *markup* (well, a Markdown dialect).

It's in an early stage of development, so it has little use for an
average user yet.
But if you are a programmer -- you are welcome to
[contribute](CONTRIBUTING.md).

## Status

Right now the engine can:

- open a `.vnrs` scene and show it as dialogue
- type the text out, skip it, go to the next chunk
- wrap UTF-8 text (Cyrillic, CJK, and friends)
- resize the window without crying

It cannot yet do the fun parts of a visual novel: choices, sprites,
music, NVL mode, and the rest of
[the language](docs/vnrs_specification.md).
The specification is ahead of the code. That is on purpose.

Linux / X11 is the only platform that `make` knows about.
Portability is a goal, not a checkbox we already ticked.

## Quickstart

You need a C99 compiler, `make`, and the usual X11 + OpenGL libs.

On Debian/Ubuntu:

```
sudo apt install build-essential libgl1-mesa-dev libx11-dev \
    libxcursor-dev libxinerama-dev libxrandr-dev libxi-dev
```

Then, from the repository root:

```
make run
```

(`make help` lists the other targets.)

It will build the bundled copy of raylib, then the engine, then
launch Alice falling down a rabbit-hole.

The first build takes a bit (raylib is not small). Later ones are
quick.

To play another scene:

```
./vinora assets/hamlet.vnrs
make run assets/i18n.vnrs
```

Controls: click, Space, or Enter to advance. Close the window to
quit. There is no settings menu. There is no menu.

If that fails on a missing X11/GL header, you are in raylib's
Linux notes, not in a Vinora-specific circle of hell:

https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

## Sample scenes

- `assets/alice.vnrs` -- Alice in Wonderland, chapter I (the default)
- `assets/hamlet.vnrs` -- Hamlet, act I scene 1
- `assets/i18n.vnrs` -- multilingual smoke test (and a rickroll)
- `assets/lorem.vnrs` -- lorem ipsum, plus one line that is *too*
  long on purpose

See [docs/vnrs_specification.md](docs/vnrs_specification.md) for the
language these files are written in. The engine understands a small
subset of it today (plain dialogue chunks). The rest is waiting for
somebody -- maybe you.

## Repository layout

```
src/          the engine
assets/       scenes, fonts, sprites, backgrounds
docs/         Vinora Screenplay spec
ext/raylib/   vendored raylib (zlib license)
```

raylib lives in-tree so `make` does not start with a package-manager
quest. We did not write it; Ramon did. Credit is in
[AUTHORS.md](AUTHORS.md). The copy is the library itself -- no
upstream IDE projects, tools, or extra docs.

## Contributing

Read [CONVENTIONS.md](CONVENTIONS.md) *before* you write code, and
[CONTRIBUTING.md](CONTRIBUTING.md) before you send it.

Work happens on `develop`. Pull requests go to `develop`. `master`
is releases -- you cannot PR there.

Patches by email are fine. Pull requests are also fine.
Silly questions are fine. 80-column lines are not optional.

## License

Vinora Engine is free software under the
[GNU GPL v3 or later](LICENSE.md).

Third-party bits keep their own licenses: raylib is zlib, the fonts
are OFL, the house backgrounds are CC BY 4.0, and Sumi has her own
rules. Credit is not optional -- see [AUTHORS.md](AUTHORS.md).
