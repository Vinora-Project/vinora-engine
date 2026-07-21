# VINORA SCREENPLAY SPECIFICATION
Version 0.0.1 (14.10.2025)  
Evgeniy "Parthen" Parfenyuk 

# 1. Introduction
## 1.1 What is Vinora Screenplay?
Vinora Screenplay is plain text format for writing 
[visual novel](https://en.wikipedia.org/wiki/Visual_novel) screenplays.

It's based on (and one-way compatible with) Markdown, simple markup language 
developed by John Gruber and Aaron Swartz in 2004. It was developed to 
simplify web pages writing.  
While Markdown originally doesn't have a specification (only `Markdown.pl`
as a reference implementation), we use
[CommonMark](https://spec.commonmark.org) for this role.

## 1.2 Why it's needed?
Vinora Screenplay is designed for use in the Vinora Engine. The goals of this
visual novels engine is to be as simple and compatible as possible.

Markdown is a popular format. There is a good chance that you're already
familiar with \**italic*\* or \_\___bold__\_\_ syntax. So, by making 
Vinora Screenplay a Markdown-like language we fulfill both of our goals 
(it's simple to use since it's known and it's compatible with dozens
of existing Markdown interpreters).

## 1.3 Comparison with another languages (TODO)

[...]


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
Depending of the system, default newline character would be CR (carriage 
return, `\r`), LF (line feed, `\n`) or even both (CR + LF).  
*NOTE: All of them are considered valid end of line, but they are replaced
with single LF (`\n`) character.*

**Whitespace characters**  
are characters which are rendered into whitespace. 
They are invisible, but they are affect formatting. For example, space
character separate different words.  
*All newline characters are whitespace characters*.  
Full list of whitespace characters: *tab (horizontal and vertical), space, 
carriage return, line feed, form feed.*

**Line**  
is a sequence of zero or more characters, that ends with newline 
or End Of File (EOF) condition. Line that has zero characters or only 
whitespace characters is called an **empty line**. 

**Paragraph**
A sequence of lines terminated by an empty line. A paragraph consisting 
solely of empty lines is called an **empty paragraph**.

**ASCII character**  
is a character supported by [US-ASCII](https://en.wikipedia.org/wiki/ASCII) 
encoding. Since any existing encoding is an extension of ASCII,
characters from ASCII is supported by nearly every computer.

## 2.2 Visual novels definitions
(Only technical definitions goes here)

**Visual novel** (from [VNDB](https://vndb.org/d6))  
A visual novel can be seen as a combination of a novel and a computer game:
they're computer games with a large text based storyline and only little 
interaction of the player.  
A typical visual novel consists of text over on character sprites with 
anime-style background image.   
Throughout the game, the player usually has to answer a few questions which 
will have an effect on the story, thus playing a visual novel a second time 
while giving other answers may result in an entirely different plot.

**ADV / NVL** mode  
- Adventure (**ADV**) is a VN mode, where text takes only a small part of
the screen (and therefore, images take more space).  
- Novel (**NVL**) is a VN mode, where text takes up a large part of the screen
(sometimes even all of it, while images are in the background).  
Commonly, VN plays in ADV mode and switch to NVL for monologues.

**Kinetic** VN  
is a VN without choices or gameplay. Term was popularised to name games from 
'KineticNovel' publisher. Games *with* gameplay, but without choices 
called **linear plot**.

**Character points**  
Points awarded or deducted based on positive or negative interactions 
with a character. Typically hidden from the player, these points influence 
the storyline.

**Hybrid** VN  
mix of traditional games (e.g. RPGs) and visual novel.

# 3. Parser
Parser read a chunk of data -- usually paragraph, but it also can be 
a line, terminated by two spaces. (So, there are *paragraph chunks* 
and *one-line chunks*).  
After chunk was read by chunk parser, **it goes to inline parser**, and then
to display or converter.

## 3.1 Chunk parser

For compatibility and readability reasons, line shouldn't be longer than 
80 characters. (And parser **MUST** throw a warning if line is longer).
Since all possible symbols fit in UTF-32 (or any other
4-byte encoding), we can expect line wouldn't be longer than 80*4=320 bytes. 
Therefore, line buffer should be 320-512 bytes.


**For example ( '__' symbolizes two spaces):**
```
1   One-line chunk with some text and two spaces:__
2
3   Another one-line chunk__
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
1   @Alex__
2
3   This text would be displayed as Alex's words.
4
5   This too! And any text chunk after, until we declare new character.

```
This is *one-line chunk*, so you **MUST** terminate it with two spaces.

Here we combine character and dialog chunks.

```
1   @NARRATOR__
2   Evening in the park. ALICE and BOB sit on a bench.
3
4   @ALICE__
5   Look how beautifully the moon is shining.
6
7   @BOB__
8   Yeah, but your eyes shine brighter.
9
10  @ALICE__
11  Oh, Bob, you're always such a romantic.
12
13  But I need to go...
```

**NARRATOR** is pre-defined character which has no name. You can define 
your own characters in `characters.ini` file:

*characters.ini*:

```ini
[ALICE]
name = Alice Smith
#color_name = #FF0000

[BOB]
name = Bob Doe
#color_name = #0000FF

[NARRATOR]
name = 
```

As you can see, we can define names differently from text.
We can also change color (both for names and text).


**HTML:**
```HTML
1   <p><br> <i>
2   Evening in the park. ALICE and BOB sit on a bench.</i></p>
3
4   <p><b>Alice Smith</b> <br>
5   Look how beautifully the moon is shining. </p>
6
7   <p><b>Bob Doe</b> <br>
8   Yeah, but your eyes shine brighter.</p>
9
10  <p><b>Alice Smith</b> <br>
11  Oh, Bob, you're always such a romantic. </p>
12
13  <p>But I need to go...</p>
```

In-game, names would also be replaced.

### 3.2.3 Chapter screen chunk
Chapter screen chunks are working like Markdown headers:

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
In-game, it would be chapter screen.

### 3.2.4 Choice chunk
With bullet points and link we can give player a choice:

```markdown
1   @ALICE__
2   Should I go with Bob or Chloe?
3
4   + [I will go with Bob](bob_route.vnrs)
5   - [(TODO) I will go with Chloe](chloe_route.vnrs) 
6   * [Go alone](secret_route.vnrs){social<0}
```

In Markdown, it doesn't matter which sign you use (+ / * / -), but 
in Vinora Screenplay they have their own functionality:

'+' is for ordinary choices  
'-' is for blocked choices. They are viewable but can't be chosen. It's
useful for planning future chapters.  
'\*' is for hidden choices. They are only viewable if the condition in 
{} brackets is true.

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
**Yes, it's just a single choice chunk.** 

This is how we can go from one file to another. Writing anything *after*
is kinda useless, since it wouldn't be viewed anyway.


### 3.2.6 Images chunk
Markdown only supports images embedding, so this chunk exists solely for
character sprites and backgrounds.

```markdown
1   ![](sprites/alice/normal.png){left}
2   @ALICE__
3   Hi there!
4
5   ![](sprites/bob/happy.png){right}
6   @BOB__
7   Hello, nice to meet you!
8
9   ![](bg/office/evening.png)
```

Images with `sprites` path work like sprites and `bg` works like 
backgrounds. For both of them we can specify additional attributes
through `{}`, but if no attributes specified engine should 
automatically place characters without collision (from left to right).

### 3.2.7 Media chunk
This is for audio and video. Well, we can't embed them with Markdown,
so we use link-like syntax:


```markdown
1   [](sound/bell.mp3)
2   [](music/morning.mp3)
3
4   [](video/intro.mp4)
```

Any audio placed in `sound` directory would be played once. `Music` in 
eponymous directory will be played on repeat.

Any audio is playing in background, but `video` is played *instead* of
text (and no layer can be over it). Video played once, then parser
reads further.

In HTML, they all will be embedded. User should click to play them, sadly. 

### 3.2.8 Commentary chunk

In Markdown, we have quotes with this kind of syntax:

```markdown
1  >This is a quote!
```

In Vinora Screenplay, they are used as comments. Parser will ignore it,
unless he's working in `DEBUG` mode, then it will be displayed as
NARRATOR words, but in italic and green text.

### 3.2.9 Directive chunk

Right now it's only used to change display mode from/to NVL/ADV.
Syntax is double curlies:

```markdown
1  {{NVL}}
```

### 3.2.10 Literal chunk

If you want dialog chunk to have no inline markup, you can wrap
it with triple graves:

```
1  ```
2  **This text will _be_ *printed* as it is**.
3  ```
```

## 3.3 Inline parser
