# Contribute to Vinora Engine

First of all -- thank you for your interest in this project!
We deeply appreciate any contribution -- big or small.

If you have any questions feel free to e-mail me:
[parthen@riseup.net](mailto:parthen@riseup.net) or telegram me:
[@parthen](https://t.me/parthen)

### Preamble

The canonical repository lives on a git server that I
(Evgeniy "Parthen" Parfenyuk) own:
[git.parthen.site](https://git.parthen.site).

(And yeah, if you are seeing this on GitHub -- it's just a mirror.
All of the development goes [here](https://git.parthen.site).)

Vinora programs are free as in freedom. If you don't agree with me,
fork the thing onto *your* site and write your own rules.

### Newbies are welcome! (It *is* a Good First Issue)

Vinora's programs are meant to be simple not only in use, but also
in development.
Don't be afraid to ask silly questions :)

## Get involved

All project files should follow [CONVENTIONS.md](CONVENTIONS.md) --
please read them carefully. The 80-column rule is not a suggestion.

### Git workflow

There are two important branches: `develop` and `master`.

`develop` is where the work happens. Feature branches start from
`develop`. Pull requests go to `develop`. That is the only branch
you open a PR against.

`master` is releases. Only the owner writes there. You cannot
open a pull request to `master`. Do not try. It will not work,
and that is not a bug.

When you want a new feature:

```
git checkout develop
git pull
git checkout -b feat/my-branch-name
```

When you want to fix something:

```
git checkout develop
git pull
git checkout -b fix/my-branch-name
```

Then open a pull request against `develop` -- or ask me to do it,
if the website is being unfriendly.

Please keep commits small and about one thing. "Fix stuff" is not a
commit message. "Fix CJK wrap in WrapText()" is.

### Code style (the short version)

Full rules are in [CONVENTIONS.md](CONVENTIONS.md). The brace rule
people get wrong:

Opening `{` goes on a **new line only for functions**. `if`,
`while`, `for`, `switch`, `struct`, `enum` keep `{` on the same
line as the header. The body of a statement never sits on the
header's line.

```c
void Foo(int x)
{
    if (x > 0) {
        Bar(x);
    } else {
        Baz();
    }
}
```

Naming is raylib-ish (TitleCase functions, snake_case enum
members). See CONVENTIONS.md for the rest.

Source, comments, string literals and other formal strings are ASCII. User
text in `.vnrs` files may be Unicode.
Your C file may not.

### Development

Development happens primarily on
[git.parthen.site](https://git.parthen.site)
(with a mirror on GitHub).

That can be overkill if you want to submit a simple fix. So, you can
email me a patch instead:

1. Make a new branch from `develop` (edit the name):
```
git checkout develop
git checkout -b fix/branch-name
```
2. Make a patch:
```
git format-patch develop --stdout > branch-name.patch
```
3. You will have a new `.patch` file with all changes. Email me this
file (also, write how to mention you in AUTHORS.md).

### License of contributions

By sending a patch or a pull request you agree to license your
changes under the same GNU GPL v3 or later as the rest of the
engine. New source files get the same copyright header as the
existing ones.

If that does not work for you -- fork is that-a-way.

### A note on code

- C99. See [CONVENTIONS.md](CONVENTIONS.md).
- `make` must still work after your change.
- `make style-check` exists. Use it.
- Do not "improve" raylib while you are here. That is a different
  project, and Ramon already has opinions.
