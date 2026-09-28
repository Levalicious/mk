# mk

Plan 9's `mk`, in the 9base port for Unix, built standalone: the lib9 it needs is vendored under `lib9/`
(9base's sources, unmodified; see `lib9/LICENSE` and `NOTICE`). One addition of ours in `unix.c`: `MKROOT`
defaults to `/usr/share/mk` when the environment does not set it. `toposort/` is the small helper that
`mkone`'s `all-libs` recipe orders library directories with (it reads `LIBS=` from each directory's mkfile).

The proto mkfiles that projects include - `<$MKROOT/$objtype/mkfile`, `<$MKROOT/proto/mkone`, `mklib`,
`mkmany` - are the [mkroot](https://github.com/Levalicious/mkroot) repository, not this one.

    make              # mk and toposort/toposort
    make test         # mkfile.test with the fresh mk
    make install      # under $PREFIX/bin (default /usr/local; PREFIX=$HOME/.local for a user install)

Every project of ours needs `objtype` (x86_64) and `MKROOT` in the environment, `mk` and `toposort` on the PATH.

## In CI: `Levalicious/mk` as an action

    - uses: Levalicious/mk@<commit>        # builds mk + toposort at that commit (cached), puts them on PATH,
                                          # exports objtype

Pin the commit: a dependant fetches the version it pinned, nothing else.
