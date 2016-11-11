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
#include "writebuf.h"

#define BUF_SIZE 8096

#define IN_SPLIT '\x1E'
#define IN_DELIM '\x1F'

#define OUT_SPLIT '\n'
#define OUT_DELIM ','
#define OUT_QUOTE '"'

WriteBuf *buf;

char field_buf[BUF_SIZE + 1] = {'\0'};
size_t field_buf_pos = 0;

bool quoted = false;

void terminate_field(char separator)
{
    if (quoted)
    {
        WriteBuf_writec(buf, OUT_QUOTE);
    }

    WriteBuf_write(buf, field_buf, field_buf_pos);

    if (quoted)
    {
        WriteBuf_writec(buf, OUT_QUOTE);
    }

    WriteBuf_writec(buf, separator);

    field_buf_pos = 0;
    quoted = false;
}

int main()
{
    WriteBuf_new(&buf, stdout);

    int in;
    while ((in = getc(stdin)) != EOF)
    {
        if (in == OUT_SPLIT || in == OUT_DELIM)
        {
            quoted = true;
            field_buf[field_buf_pos++] = in;
        }
        else if (in == OUT_QUOTE)
        {
            quoted = true;
            field_buf[field_buf_pos++] = in;
            field_buf[field_buf_pos++] = in;
        }
        else if (in == IN_SPLIT)
        {
            terminate_field(OUT_SPLIT);
        }
        else if (in == IN_DELIM)
        {
            terminate_field(OUT_DELIM);
        }
        else
        {
            field_buf[field_buf_pos++] = in;
        }
    }

    if (field_buf_pos > 0)
    {
        terminate_field(OUT_SPLIT);
    }

    WriteBuf_close(&buf);

    return EXIT_SUCCESS;
}
