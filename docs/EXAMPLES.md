# Integration Examples

Those are just some examples I thought of, quite alot more is possible.


## In command line

* **Convert output to CSV**

`zulu -b | awk -F'|' '{print $1 "," $2}'`

* **Pipe to a text file**

`zulu -b >> combined-lists`

* **Filter by sizes**

Show files over 1MB:

`zulu -b | awk -F'|' '$2 > 1048576'`

Show sizes in MB:

`zulu -b | awk -F'|' '{ printf "%s %.2fMB\n", $1, $2/1024/1024 }'`

Summing bytes:

`zulu -b | awk -F'|' '{sum += $2} END {print sum}'`

* **Sorting byte output**

Alphabetically:

`zulu -b | sort -t '|' -k1,1`

Ascending:

`zulu -b | sort -t '|' -k2,2n`

Descending:

`zulu -b | sort -t '|' -k2,2nr`


## In a script

* **Sums up two folder listings**

```bash
#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ]; then
    echo "Usage: $0 <directory1> <directory2>"
    exit 1
fi

DIR1="$1"
DIR2="$2"

OUT="combined-lists"

zulu -b "$DIR1" > "$OUT"
zulu -b "$DIR2" >> "$OUT"

# Sum all byte counts (skip headers and footers)
TOTAL=$(awk -F'|' '
    $2 ~ /^[0-9]+$/ { sum += $2 }
    END { print sum }
' "$OUT")

rm "$OUT"

echo "Total size: $TOTAL bytes"
```
