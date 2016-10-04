/*
 * File:    csvjoin.c
 * Author:  Samuel Coleman
 *
 * Description:
 *
 * Join CSV data into records and fields separated by the ASCII record and unit
 * separator characters (0x1E and 0x1F).
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 8000

#define IN_SPLIT '\x1E'
#define IN_DELIM '\x1F'

#define OUT_SPLIT '\n'
#define OUT_DELIM ','
#define OUT_QUOTE '"'

char buf[BUF_SIZE + 1] = {'\0'};
size_t buf_pos = 0;

bool quoted = false;

void flush(char separator)
{
    if (quoted)
    {
        putc(OUT_QUOTE, stdout);
    }

    fwrite(buf, buf_pos, 1, stdout);

    if (quoted)
    {
        putc(OUT_QUOTE, stdout);
    }

    putc(separator, stdout);

    buf_pos = 0;
    quoted = false;
}

int main()
{
    int in;
    while ((in = getc(stdin)) != EOF)
    {
        if (in == OUT_DELIM)
        {
            quoted = true;
            buf[buf_pos++] = in;
        }
        else if (in == OUT_QUOTE)
        {
            quoted = true;
            buf[buf_pos++] = in;
            buf[buf_pos++] = in;
        }
        else if (in == IN_SPLIT)
        {
            flush(OUT_SPLIT);
        }
        else if (in == IN_DELIM)
        {
            flush(OUT_DELIM);
        }
        else
        {
            buf[buf_pos++] = in;
        }
    }

    return EXIT_SUCCESS;
}
