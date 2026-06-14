# Zulu CLI

* **Requirements:** C compiler and a POSIX compliant system, no other dependencies.

* **Versions:** in [releases](https://github.com/Ivory-Hubert/zulu/releases)

Provides quick summaries of one directory:
* biggest/smallest file
* total size of all files
* total count of files/subdirectories

Allows choosing between allocated & apparent sizes and 1000 or 1024 based units.

Can make a machine friendly listing of a directory, with raw byte sizes for usage in pipelines.

Due to the lack of recursion or complex sorting/filtering does its intended job rather quickly.
There are also other quality of life features sprinkled on top, see below.


## Appearance

That's the basic output and summary in action:

![Nice CLI](docs/pics/basic_output.png)


## Features

* Lists only useful units together, depending on sizes: `GiB & MiB` | `MiB & KiB` | `KiB & bytes` | `just bytes`
also supports SI units (powers of 1000) and allocated sizes.
* Can list the files & subdirectories found, *always lists the biggest & smallest file*.
* Simple output mode that skips some logic, to display only total size & count, and only in the largest units needed.
* Table listing mode, optionally with converted units, *outputs in a parser-friendly format when piped*.
* Built-in byte converter, displays provided bytes in chosen units.
* Can display detailed file stats for CWD files or full paths, *follows symlinks by default*.
* Uses colors by default but drops them when piped, see below for more details.

See also:
* Some more output [screenshots](./docs/pics/README.md)
* Integration [examples](./docs/EXAMPLES.md) using piped output

> [!NOTE]
> Zulu does not report any metadata other than file size, *outside of file stat view*. Does not dive into subdirectories. Ignores non-files entirely, *will still list subdirectories*.

When it comes to colored output, Zulu checks for `NO_COLOR` and `ZULU_NO_COLOR` environment variables on startup.
Whether you don't want colors at all or just no Zulu colors, you're covered. There's still some bold/italic text when not piped.


## Installation

There is a pre built binary in GitHub releases for comfort, but compliation should be trivial.

I naturally have the `compile.sh` & `Makefile` in root for gcc users. 
Then drop it in `~/.local/bin` or make a bash alias to the binary for easy access.

---
Feedback, suggestions and contributions are always welcome.
Licensed under the terms of the GNU [General Public License version 3](https://www.gnu.org/licenses/gpl-3.0.html)
