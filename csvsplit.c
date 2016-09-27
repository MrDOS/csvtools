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

#define IN_SPLIT '\n'
#define IN_DELIM ','
#define IN_QUOTE '"'

#define OUT_SPLIT '\x1E'
#define OUT_DELIM '\x1F'

typedef enum
{
    STATE_NORMAL,
    STATE_QUOTED,
    STATE_QUOTE_MAYBE_ESCAPE
} ParseState;

int main(int argc, char *argv[])
{
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
                putc(in, stdout);
            }
            break;
        case STATE_QUOTE_MAYBE_ESCAPE:
            if (in == IN_QUOTE)
            {
                putc(in, stdout);
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
                putc(OUT_SPLIT, stdout);
            }
            else if (in == IN_DELIM)
            {
                putc(OUT_DELIM, stdout);
            }
            else if (in == IN_QUOTE)
            {
                state = STATE_QUOTED;
            }
            else
            {
                putc(in, stdout);
            }
        }
    }

    return EXIT_SUCCESS;
}
