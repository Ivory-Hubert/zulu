# Zulu CLI

> Performant directory stats in readable format

* **Requirements:** C compiler and an Unix system

* **Platforms:** Linux or macOS (I've only used it on Linux)

## Appearance

That's the basic output and summary version in action:

![Nice CLI](docs/pics/basic_output.png)

More images in [pics](./docs/pics)


## About

I made this with efficiency in mind, and what's less efficient than having to use bash aliases or bloated tools to
check how large a folder is, and what's the biggest file?
So Zulu was born, it takes less flags than du and ls take combined to output a quick summary of directories like Zulu,
and if you really need the bytes, that's possible too.

Anyway, if you need a TUI that shows you every files full metadata and many folders at once, Zulu is not it.
But if you need to check source code size, build artifacts, /any/system/directory etc... Zulu is your < 1 ms friend (usually).

Do note that you should either trust my provided binary (reverse engineer it to be safe) or compile it yourself, Zulu is small so that's not an issue, I have the compile.sh script in the root for gcc users. Then make a bash alias to the binary or whatever you wish for easy access.


## License

Copyright © 2026 Ivori Huobolainen

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
[GNU General Public License version 3](https://www.gnu.org/licenses/gpl-3.0.md) for more details.

