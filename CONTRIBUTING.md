# Contribute to Vinora Engine 
First of all -- thank you for your interest in this project! 
We deeply appreciate any contributiion -- big or small.

If you have any questions feel free to e-mail me:
[parthen@riseup.net](mailto:parthen@riseup.net) or telegram me:
[@parthen](https://t.me/parthen)

### Preamble 
This is *private* git repository. That means me (Evgeniy "Parthen" Parfenyuk)
and only me own this website (git.parthen.site) and git server.

(And yeah, if you seeing this on Github -- it's just a mirror, all of
development goes [here](https://git.parthen.site)).

If you don't agree with me -- all of Vinora Project programs are free 
as in freedom, so make your own fork (on *your* site) and your own rules.

### Newbies are welcome! (It *is* a Good First Issue)

Vinora's programs meant to be simple not only in use, but also in develop.
Don't afraid to ask silly questions :)

## Get involved:
All project files should follow [CONVENTIONS.md](CONVENTIONS.md) please,
read them carefully.

### Git workflow
There are two important branches: `master` and `develop`.

`Master` is read-only for everyone except owner and `develop` is avaiable
only for pull requests.

When you want to make a new feature, you make a new branch from `develop`
with name like `feat/my-branch-name`. After you've done, you make a pull 
request (or ask me to do it) back to `develop`.

Same with fixes (`fix/my-branch-name`).


### Development 
Development is going primary on [git.parthen.site](https://git.parthen.site).
(With mirror on Github)

It can be overkill if you want to submit a simple fix. So, you can email
me patch instead:

1. Make a new branch (edit branch-name):
```
git checkout -b fix/branch-name
```
2. Make a patch:
```
git format-patch develop --stdout > branch-name.patch
```
3. You will have a new .patch file with all changes. Email me this file
(also, write how to mention you in AUTHORS.md file).

