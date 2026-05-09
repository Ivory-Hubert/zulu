# Zulu CLI

* **Requirements:** C compiler and a POSIX system, no other dependencies.

* **Versions:** in [releases](https://github.com/Ivory-Hubert/zulu/releases)
 

## Appearance

That's the basic output and summary in action:

![Nice CLI](docs/pics/basic_output.png)


## Features

* Lists only useful units together, depending on sizes: `GiB & MiB` | `MiB & KiB` | `KiB & bytes` | `just bytes`
* Can list the files & subfolders found, *always lists the biggest & smallest file*.
* Simple output mode that skips some logic, to display only total size & count, and only in the largest units needed.
* Byte output mode that skips math entirely, only lists as it reads. *This mode outputs in a parser-friendly format when piped*.
* Built-in byte converter, displays provided bytes in GiB, MiB & KiB.
* Can display detailed file stats for CWD files or full paths, *follows symlinks by default*.
* Uses colors by default, but is also redirection friendly and there's a no color mode, see below for more details.

See also:
* Some more output [screenshots](./docs/pics/README.md)
* Integration [examples](./docs/EXAMPLES.md) using piped output

> [!NOTE]
> Zulu does not report any metadata other than file size, *outside of file stat view*. Does not go into subdirectories. Ignores non-files entirely, *will still list the subfolders*.

When it comes to colored output, Zulu checks for `NO_COLOR` and `ZULU_NO_COLOR` environment variables on startup.
Whether you don't want colors at all or just no Zulu colors, you're covered. There's still some bold text though.

## About

I made this with efficiency in mind, and what's less efficient than having to use bash aliases or bloated tools to
check how large some folders files are, how many there are, and list the biggest/smallest files?
So Zulu was born, it takes less flags than du and ls take combined to show a similar summary,
and it runs laps on TUI's if you don't need every detail (at once).

Do note that you should either trust my provided binary (*reverse engineer for assurance*), or of course compile yourself.
I think the self compile hustle makes sense with Zulu though, since there are no external dependencies it'll be quick,
and you can tweak some while you're at it, if you want.

I naturally have the `compile.sh` & `Makefile` in root for gcc users. 
Then drop it in `~/.local/bin` or make a bash alias to the binary for easy access.

---
Feedback, suggestions and contributions are always welcome. Licensed under the terms of the GNU [General Public License version 3](https://www.gnu.org/licenses/gpl-3.0.html)
