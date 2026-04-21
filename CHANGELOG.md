# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)

[DA](https://github.com/Ivory-Hubert/Developer-Assistant) assures the format stays consistent.

## [Unreleased]

### Changed
- **For the machines**

Added a check for byte list (`-b`) mode that checks for the existance of a terminal. If output is piped Zulu will skip any colours and formatting to provide a parser-friendly output. The header shows what directory is listed and footer shows the file count, so those can be used to seperate multiple folder lists in one file, for example.

- **UI tweaks**

Removed the header from standard `-s`/`-p` output, it just crowds the screen. But of course if the `--ls` flag is present then the list and data are still seperated and marked with headers. Also tweaked the whitespaces a bit to make the data easier to scan, in my eyes of course.
