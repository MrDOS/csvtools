# CSVTools

CSVs are hard. Make your life easier.

## Concept

The field and record separator characters used by the CSV format (`,` and `\n`,
respectively) can (and very often do) appear within the field contents. In turn,
tools which interact with CSVs must have the ability to quote these characters
(and in turn escape the quotes) when encoding to CSV, and to unquote/unescape
when decoding them. This hamstrings numerous utilities (mostly `cut` and `awk`)
which expect a single delimiting character.

Here's the trick: ASCII defines four characters, `1C` through `1F`, as the file,
group, record, and unit separator characters. These are non-printable, binary
characters which do not appear in regular text. You can probably see where I'm
going with this: statefully and appropriately replace `,` with `\x1E` and `\n`
with `\x1F`, perform your processing with ordinary, record- and field-oriented
tools with slightly extraordinary delimiters, then encode back to boring old
(but correct) CSV when finished. So that's what this project is.

## Running

### `csvsplit(1)`

Takes CSV content via stdin, performs the replacements described above, and
writes to stdout.

### `csvjoin(1)`

Does the opposite of `csvsplit(1)`: given a `\x1E`/`\x1F`-delimited data stream,
it outputs well-escaped CSV data.

### `cawk`/`cawkin`

These are convenience wrappers around `awk(1)`. `cawkin` unmarshals CSV content
from stdin into awk invoked with the flags necessary for field and record
separation on the special characters. `cawk` takes this a step further by
rigging things up so the output is also valid, escaped CSV data in case your awk
program doesn't format data for human consumption or input for another program.

Both of these wrappers accept typical awk arguments (e.g., accepting a program
given as an argument or using `-f` to point at a program file) or can be used as
the shebang of an executable awk program (i.e., `#! /usr/bin/env cawk -f`).

## Building

    $ cc -Wall -Wextra -pedantic -Os -o csvsplit csvsplit.c writebuf.c
    $ cc -Wall -Wextra -pedantic -Os -o csvsplit csvjoin.c writebuf.c

Todo: this really deserves a makefile.