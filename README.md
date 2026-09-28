# mk

Plan 9's `mk`, in the 9base port for Unix, built standalone: the lib9 it needs is vendored under `lib9/`
(9base's sources, unmodified; see `lib9/LICENSE` and `NOTICE`). One addition of ours in `unix.c`: `MKROOT`
defaults to `/usr/share/mk` when the environment does not set it, and modification times are nanoseconds (`st_mtim`),
so that a target made in the same second as its prerequisite is not stale on the next run (mk compares with `<=`,
rightly for second-resolution stamps; with nanoseconds that no longer relinks every program after every compile).

The proto mkfiles that projects include - `<$MKROOT/$objtype/mkfile`, `<$MKROOT/proto/mkone`, `mklib`,
`mkmany` - are the [mkroot](https://github.com/Levalicious/mkroot) repository, not this one.

    make              # mk
    make test         # mkfile.test with the fresh mk
    make install      # under $PREFIX/bin (default /usr/local; PREFIX=$HOME/.local for a user install)

Every project of ours needs `objtype` (x86_64) and `MKROOT` in the environment and `mk` on the PATH.

## In CI: `Levalicious/mk` as an action

    - uses: Levalicious/mk@<commit>        # builds mk at that commit (cached), puts it on PATH, exports objtype

Pin the commit: a dependant fetches the version it pinned, nothing else.
