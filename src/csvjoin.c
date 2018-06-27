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
#include <ctype.h>
#include "writebuf.h"

#define BUF_SIZE 16384

#define IN_SPLIT '\x1E'
#define IN_DELIM '\x1F'

#define OUT_SPLIT '\n'
#define OUT_DELIM ','
#define OUT_QUOTE '"'

WriteBuf *buf;

char field_buf[BUF_SIZE];
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

    int record = 1;
    int field = 1;
    bool overflowed = false;

    int status = EXIT_SUCCESS;
    int in;
    while ((in = getc(stdin)) != EOF)
    {
        int copies = 1;
        if (in == IN_SPLIT)
        {
            terminate_field(OUT_SPLIT);
            record++;
            field = 1;
            overflowed = false;
            continue;
        }
        else if (in == IN_DELIM)
        {
            terminate_field(OUT_DELIM);
            field++;
            overflowed = false;
            continue;
        }

        if (in == OUT_DELIM || iscntrl(in))
        {
            quoted = true;
        }
        else if (in == OUT_QUOTE)
        {
            quoted = true;
            /* Escape the quote character by emitting it twice. */
            copies = 2;
        }

        /* Don't buffer past our limit. */
        if (overflowed)
        {
            continue;
        }
        else if (field_buf_pos + copies >= BUF_SIZE)
        {
            fprintf(stderr,
                    "csvjoin: field too long at record %d, field %d\n",
                    record,
                    field);
            overflowed = true;
            status = EXIT_FAILURE;
        }
        else
        {
            for (int copy = 0; copy < copies; copy++)
            {
                field_buf[field_buf_pos++] = in;
            }
        }
    }

    if (field_buf_pos > 0)
    {
        terminate_field(OUT_SPLIT);
    }

    WriteBuf_close(&buf);

    return status;
}
