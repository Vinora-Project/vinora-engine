# VINORA SCREENPLAY SPECIFICATION
Version 0.0.3 (21.08.2026)
Evgeniy "Parthen" Parfenyuk

# 1. Introduction
## 1.1 What is Vinora Screenplay?
Vinora Screenplay is a plain text format for writing
[visual novel](https://en.wikipedia.org/wiki/Visual_novel) screenplays (and
interactive fiction as well).

It's a Markdown-inspired language: its inline syntax (bold, italic, links,
headers) is drawn from the same conventions popularized by John Gruber and
Aaron Swartz in 2004. Block-level syntax, however, is repurposed to carry
visual-novel semantics that plain Markdown was never meant to express (see
S.4 for what this means in practice).

## 1.2 Why it's needed?
Vinora Screenplay is designed for use in the Vinora Engine. The goal of this
visual novel engine is to be as simple and approachable as possible.

Markdown is a popular format, and there's a good chance you're already
familiar with \**italic*\* or \_\___bold__\_\_ syntax. By keeping Vinora
Screenplay's inline syntax Markdown-like, we lower the learning curve for
anyone who has ever written a README or a forum post.

## 1.3 Comparison with other languages

**Ren'Py.** The 800-pound gorilla. It is a Python engine with a
scripting language on top. Vinora Screenplay has no scripting.
If you need a minigame in the middle of a date, Ren'Py is still
the tool. If you want a screenplay that looks like a screenplay,
you are in the right place.

**Ink / Yarn Spinner.** Dialogue DSLs with a lot of branching
machinery. Vinora Screenplay puts every branch in a different
file on purpose (see S.2.2). Less clever. Easier to diff.

**Twine.** Hypertext, not a visual novel. Close in spirit (links
are the structure), different on screen.

**Markdown.** Vinora Screenplay *looks* like Markdown. It is not
Markdown. See S.4.


# 2. Definitions

## 2.1 Technical definitions

**Character**
is the smallest unit of text that carries a semantic or
visual meaning in a writing system. It can be a letter, digit,
punctuation mark, space, special character (e.g., "@" or newline),
or another element used to represent information in text form.
*NOTE: emojis are images, and not text characters.
So-called Unicode "combined characters" are considered (and rendered) as
multiple separate characters*.

**Newline characters**
are special characters that end a line.
Depending on the system, the default newline character would be CR (carriage
return, `\r`), LF (line feed, `\n`) or even both (CR + LF).
*NOTE: All of them are considered a valid end of line, but they are replaced
with a single LF (`\n`) character.*

**Whitespace characters**
are characters which are rendered into whitespace.
They are invisible, but they affect formatting. For example, the space
character separates different words.
*All newline characters are whitespace characters*.
Full list of whitespace characters: *tab (horizontal and vertical), space,
carriage return, line feed, form feed.*

**Line** is a sequence of zero or more characters, that ends with newline
or End Of File (EOF) condition. A line that has zero characters or only
whitespace characters is called an **empty line**.

**Paragraph** A sequence of lines terminated by an empty line. A paragraph
consisting solely of empty lines is called an **empty paragraph**.

**ASCII character** is a character supported by
[US-ASCII](https://en.wikipedia.org/wiki/ASCII) encoding. Since any
existing encoding is an extension of ASCII, characters from ASCII are
supported by nearly every computer.

## 2.2 Visual novels definitions (Only technical definitions go here)

**Visual novel** (from [VNDB](https://vndb.org/d6)) A visual novel can be
seen as a combination of a novel and a computer game: they're computer
games with a large text-based storyline and only little interaction from
the player.  A typical visual novel consists of text over character
sprites with an anime-style background image.  Throughout the game, the
player usually has to answer a few questions which will have an effect on
the story, thus playing a visual novel a second time while giving other
answers may result in an entirely different plot.

**ADV / NVL** mode
- Adventure (**ADV**) is a VN mode, where text takes only a small part of
 the screen (and therefore, images take up more space).
- Novel (**NVL**) is a VN mode, where text takes up a large part of the
 screen (sometimes even all of it, while images are in the background).
 Commonly, a VN plays in ADV mode and switches to NVL for monologues.
 **Kinetic** VN is a VN without choices or gameplay. The term was
 popularized to name games from the 'KineticNovel' publisher. Games
 *with* gameplay, but without choices, are called **linear plot**.

**Character points** Points awarded or deducted based on positive or
negative interactions with a character. Typically hidden from the player,
these points influence the storyline.

**Hybrid** VN mix of traditional games (e.g. RPGs) and visual novel.

**Scene** is a single `.vnrs` file. A scene is the smallest indivisible
unit of narrative in Vinora Screenplay: it has no internal branching.  Any
decision point that would change what the player sees next must resolve to
a transition into a *different* scene file (see S.3.2.4, S.3.2.5). This is
a deliberate constraint, not a limitation of the parser -- it keeps a scene
linear and self-contained, which simplifies both authoring and tooling
(diffing, translation, per-scene testing).

# 3. Parser
The parser reads a chunk of data -- usually a paragraph, but it
can also be a single line (so there are *paragraph chunks* and *one-line
chunks*).  After a chunk is read by the chunk parser, **it goes to the
inline parser**, and then to display or a converter.

## 3.1 Chunk parser

For compatibility and readability reasons, a line shouldn't be longer than
80 characters (and the parser **MUST** throw a warning if a line is
longer). Since all possible symbols fit in UTF-32 (or any other 4-byte
encoding), we can expect a line to be no longer than 80*4=320 bytes.
Therefore, the line buffer should be >320 bytes.

Chunk boundaries are determined as follows:

1. The file is split into paragraphs on one or more empty lines (see
  S.2.1).  The exact number of consecutive empty lines between two chunks
  has no meaning -- one empty line and five empty lines are equivalent
  separators.

2. Each paragraph is classified into exactly one chunk type (S.3.2) by
  inspecting the first line of the paragraph against the chunk-type
  markers (`::`, `#`, `+` `-` `*`,  `[`, `![`, `[`, `>`, `{{`, ```` ).
  If no marker matches, the paragraph is a Dialog chunk (S.3.2.1)
   -- the default.

3. A Character name chunk (S.3.2.2) is the one exception that spans
  past its own paragraph: once a `Name::` line is seen, every following
  chunk is attributed to that character until a new `Name::` (or bare
  `::`) chunk appears, or the file ends.

```
1   One-line chunk with some text and two spaces:
2
3   Another one-line chunk
4
5
6   A big paragraph chunk with some Lorem ipsum text:__
7   Lorem ipsum dolor sit amet, consectetur__
8   adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore
9   aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco.
10
11
12
13  It doesn't matter how many empty lines are between chunks.
```

## 3.2 Chunk types
### 3.2.1 Dialog (base) chunk
Default paragraph and one-line chunk.

**Vinora Screenplay:**
```
1   Hello!
2
3   Goodbye!
```

**HTML:**
```HTML
<p>Hello!</p>
<p>Goodbye!</p>
```

### 3.2.2 Character name chunk
To make a *character* say something we have this syntax:

```
1   Alex::
2
3   This text would be displayed as Alex's words.
4
5   This too! And any text chunk after, until we declare a new character.

```

Here we combine character and dialog chunks:

```
1   ::
2   Evening in the park. ALICE and BOB sit on a bench.
3
4   Alice::
5   Look how beautifully the moon is shining.
6
7   Bob::
8   Yeah, but your eyes shine brighter.
9
10  Alice::
11  Oh, Bob, you're always such a romantic.
12
13  But I need to go...
```

On line 1, there is a character name chunk without a name. It's a normal
and valid chunk, used for author/narrator lines.

Any name in this kind of chunk is considered a macro and can be
replaced/customized in a `characters.ini` file:

```ini
[ALICE]
name = Alice Smith
color_name = #FF0000

[BOB]
name = Bob Doe
color_name = #0000FF

[]
font_type = italic
# All narrative text would be in italic!
```

As you can see, we can define names differently from the text in the
scene file. We can also change color (both for names and text).

*Scope note: `characters.ini` applies project-wide (one file
per project root). This keeps character definitions consistent across
scenes without repetition.*

**HTML:**
```HTML
1   <p><i>
2   Evening in the park. ALICE and BOB sit on a bench.</i></p>
3
4   <p><b>Alice Smith</b><br>
5   Look how beautifully the moon is shining.</p>
6
7   <p><b>Bob Doe</b><br>
8   Yeah, but your eyes shine brighter.</p>
9
10  <p><b>Alice Smith</b><br>
11  Oh, Bob, you're always such a romantic.</p>
12
13  <p>But I need to go...</p>
```

In-game, names would also be replaced.

### 3.2.3 Chapter screen chunk
Chapter screen chunks work like Markdown headers:

```markdown
1   # Header 1
2   ## Header 2
3   ### Header 3
4   #### Header 4
5   ##### Header 5
6   ###### Header 6
```

```HTML
1   <h1>Header 1</h1>
2   <h2>Header 2</h2>
3   <h3>Header 3</h3>
4   <h4>Header 4</h4>
5   <h5>Header 5</h5>
6   <h6>Header 6</h6>
```
In-game, it would be a chapter screen.

### 3.2.4 Choice chunk
With bullet points and a link we can give the player a choice:

```markdown
1   ALICE::
2   Should I go with Bob or Chloe?
3
4   + [I will go with Bob](bob_route.vnrs)
5   - [(TODO) I will go with Chloe](chloe_route.vnrs)
6   * [Go alone](secret_route.vnrs){social<0}
```

In Markdown, it doesn't matter which sign you use (+ / * / -), but in
Vinora Screenplay they have their own functionality:

`+` is for ordinary choices
`-` is for blocked choices. They are viewable but can't be chosen. It's
useful for planning future chapters.
`*` is for hidden choices. They are only viewable if the condition in
`{}` brackets is true.

**Every choice target MUST be a link to another scene file.** There is no
form of choice chunk that branches within the current file -- per S.2.2, a
scene is indivisible, and any narrative fork always means "go read a
different file." This also means choice chunks cannot be used to set a
flag and continue in place; state changes that don't redirect the player
are out of scope for this chunk type (see S.5).

**Disambiguation from plain Markdown lists:** a line only becomes a choice
chunk marker when the bullet (`+`, `-`, or `*`) is immediately followed by
a space and a Markdown link `[...](...)`, as in the examples above. A
bullet followed by anything else (plain text, no link) is parsed as an
ordinary Dialog chunk list item and carries no game semantics. This also
disambiguates against Markdown's use of `-`, `*`, `_` (repeated three or
more times) as a horizontal rule: a rule requires the marker to repeat
with no intervening link syntax, which never matches the choice-chunk
pattern.

### 3.2.5 Link chunk
Goto statements are considered harmful since
[1968](https://dl.acm.org/doi/epdf/10.1145/362929.362947).

Well, **sometimes** they are useful. More often they are harmful.

Good news is we can easily avoid them by using Markdown links:

```markdown
1   Let's call this a day...
2
3   + [](next_day.vnrs)
```
**Yes, it's just a single choice chunk** with no visible label -- used for
a linear "continue" transition rather than a real decision. It is still
subject to the same rule as S.3.2.4: it can only point to another scene
file, never branch within the current one.

This is how we move from one file to another. Writing anything *after* it
is effectively unreachable, since the scene ends at the transition.

### 3.2.6 Images chunk
Markdown only supports image embedding, so this chunk exists solely for
character sprites and backgrounds.

```markdown
1   ![](sprites/alice/normal.png){left}
2   ALICE::
3   Hi there!
4
5   ![](sprites/bob/happy.png){right}
6   BOB::
7   Hello, nice to meet you!
8
9   ![](bg/office/evening.png)
```

Images with a `sprites` path work like sprites and `bg` works like
backgrounds. For both, we can specify additional attributes through `{}`,
but if no attributes are specified, the engine should automatically place
characters without collision (from left to right).

### 3.2.7 Media chunk
This is for audio and video. We can't embed them with plain Markdown
image syntax, so we use link-like syntax instead:

```markdown
1   [](sound/bell.mp3)
2   [](music/morning.mp3)
3
4   [](video/intro.mp4)
```

Any audio placed in the `sound` directory is played once. `Music` in the
eponymous directory plays on repeat.

Audio plays in the background, but `video` plays *instead of* text (and
no layer can be over it). Video plays once, then the parser reads
further.

### 3.2.8 Commentary chunk

In Markdown, we have quotes with this kind of syntax:

```markdown
1  >This is a quote!
```

In Vinora Screenplay, these are used as author comments. The parser
ignores them, unless it's working in `DEBUG` mode, in which case the
comment is displayed as NARRATOR words, in italic and green text.
*Note: because generic Markdown renderers don't know about DEBUG mode,
the book/web export step (S.4) strips commentary chunks unconditionally --
they are never meant to reach a reader.*

### 3.2.9 Directive chunk

Right now this is only used to change the display mode from/to NVL/ADV.
Syntax is double curlies:

```markdown
1  {{NVL}}
```

### 3.2.10 Literal chunk

If you want a dialog chunk to have no inline markup, you can wrap it with
triple graves:

```
1  ```
2  **This text will _be_ *printed* as it is**.
3  ```
```

*Note: this is currently the only escaping mechanism in the language --
it works at the chunk level, not inline. There is no way to escape a
single conflicting character (e.g. a literal `::` inside a line of
dialog) without wrapping the whole chunk. This is an open question, see
S.5.*

## 3.3 Inline parser

Not specified in this revision. The intended baseline is
Markdown's inline emphasis (`*italic*`, `**bold**`, `_italic_`,
`__bold__`) and nothing else until we have a good reason.

This section is a known hole, not an accidental omission.


# 4. Output pipelines and Markdown compatibility

A `.vnrs` file is meant to serve **two** downstream consumers, and it is
not expected to look sensible to both without an intermediate step:

**1. The Vinora Engine** parses the full chunk grammar (S.3.2) and renders
an interactive visual novel: sprites, backgrounds, audio, branching
choices, debug output, everything.

**2. A "book export" step** transforms a `.vnrs` file (or a sequence of
scene files, following its choice-chunk links) into a single, clean,
standard-compliant Markdown (or HTML) document, suitable for feeding into
off-the-shelf tools -- pandoc, static site generators, ebook packagers,
etc. This transform is a distinct piece of tooling from the engine's
parser, and it makes deliberate decisions about VN-only constructs:

**Dialog** -- passed through as-is.

**Character name** -- expanded to `**Name**` (or similar) followed
by the dialog, using the display name from `characters.ini`.

**Chapter screen** -- passed through as-is (`#` ... `######`).

**Choice** -- resolved along one chosen path (e.g. the `+` route,
or a route selected by export config). Blocked (`-`) and unresolved
hidden (`*`) choices are omitted entirely, so a reader never sees
them.

**Link** -- followed: the next scene's content is appended, not
left as a bare link.

**Images** -- kept as Markdown images, or omitted, depending on
the export target.

**Media** -- omitted (audio/video have no book/web-static
equivalent).

**Commentary** -- stripped unconditionally (see S.3.2.8).

**Directive** -- stripped (NVL/ADV has no meaning outside the
engine).

**Literal** -- unwrapped to plain text; markup is then rendered
normally.

This is why the earlier claim of the format being "compatible with
Markdown" (v0.0.2, S.1.2) has been retired: piping a raw `.vnrs` file into
a generic CommonMark renderer without the export step produces broken or
misleading output (visible debug comments, literal `{left}`/`{{NVL}}`
text, unresolved choice conditions, empty audio links). The book export
step is what actually delivers on the "read this as a webpage / ebook"
goal -- not incidental compatibility with third-party renderers.
