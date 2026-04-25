# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)

[DA](https://github.com/Ivory-Hubert/Developer-Assistant) assures the format stays consistent.

## [Unreleased]

### Changed
- **Unit math & display**

Changed the math engine a bit, so now for every size category (*total, biggest, smallest*) two units are always displayed. So: `GiB | MiB`, `MiB | KiB`, `KiB & bytes`, or just bytes. Also the basic no colour output will now show the second unit aswell. 

- **UI updates**

Yet again some more UI tweaking going on. I also now count the subfolders that Zulu finds and display that count along side the file count too. So now you get a good picture of a folders contents without the overhead of recursion. Also made the summary display more structured. 


## [0.1.1] - 2026-04-24

### Added
- **Byte converter**

This one is simple and effective, takes a raw byte count and converts it to KiB, MiB & GiB. Can be used with the byte list effectively too.

- **File stats**

Now one can't say it's just a dumb file size tool, drop a file name from your CWD or a file path to Zulu with the `-f` flag and you get similar output to simple `stat`, Zulu just follows symlinks automatically and formats the output (*no colors*).

### Changed
- **For the machines**

Added a terminal check for byte list (`-b`) mode. If output is piped Zulu will skip any colours and formatting to provide a parser-friendly output. The header shows what directory is listed and footer shows the file count, so those can be used to seperate multiple folder lists in one file, for example.

- **List subfolders**

Now the list flag won't just list the files Zulu counts, also displays the found subdirectories, albeit greyed out to not confuse the eye. Copy that folder and dump another summary if you want.

- **UI style**

Removed the header from standard `-s`/`-p` output, it just crowds the screen. But of course if the `--ls` flag is present then the list and data are still seperated and marked with headers. Also tweaked the whitespaces a bit to make the data easier to scan, in my eyes atleast.
Also added more colors to the summary mode to eliminate the wall of cyan & white text. Then tweaked the element placement a bit.

