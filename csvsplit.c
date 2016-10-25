/*
 * File:    csvsplit.c
 * Author:  Samuel Coleman
 *
 * Description:
 *
 * Split CSV data into records and fields separated by the ASCII record and unit
 * separator characters (0x1E and 0x1F).
 */

#include <stdio.h>
#include <stdlib.h>
#include "writebuf.h"

#define IN_SPLIT '\n'
#define IN_DELIM ','
#define IN_QUOTE '"'

#define OUT_SPLIT '\x1E'
#define OUT_DELIM '\x1F'

typedef enum
{
    STATE_NORMAL,
    STATE_QUOTED,
    STATE_QUOTE_MAYBE_ESCAPE,
    STATE_JUST_SPLIT
} ParseState;

int main()
{
    WriteBuf *buf;
    WriteBuf_new(&buf, stdout);

    ParseState state = STATE_NORMAL;

    int in;
    while ((in = getc(stdin)) != EOF)
    {
        switch (state)
        {
        case STATE_QUOTED:
            if (in == IN_QUOTE)
            {
                state = STATE_QUOTE_MAYBE_ESCAPE;
            }
            else
            {
                WriteBuf_writec(buf, in);
            }
            break;
        case STATE_QUOTE_MAYBE_ESCAPE:
            if (in == IN_QUOTE)
            {
                WriteBuf_writec(buf, in);
                state = STATE_QUOTED;
            }
            else
            {
                state = STATE_NORMAL;
            }
            /* We're out of the quote. Fall through to the default handler. */
        default:
            if (in == IN_SPLIT)
            {
                if (state != STATE_JUST_SPLIT)
                {
                    WriteBuf_writec(buf, OUT_SPLIT);
                    state = STATE_JUST_SPLIT;
                }
            }
            else if (in == IN_DELIM)
            {
                WriteBuf_writec(buf, OUT_DELIM);
                state = STATE_NORMAL;
            }
            else if (in == IN_QUOTE)
            {
                state = STATE_QUOTED;
            }
            else
            {
                WriteBuf_writec(buf, in);
                state = STATE_NORMAL;
            }
        }
    }

    WriteBuf_close(&buf);

    return EXIT_SUCCESS;
}
