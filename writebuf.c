/*
 * File:    writebuf.c
 * Author:  Samuel Coleman
 *
 * Description:
 *
 * A simple buffered writer.
 */

#include <stdlib.h>
#include <string.h>
#include "writebuf.h"

#define WB_MIN(a, b) ((a < b) ? a : b)

void WriteBuf_new(WriteBuf **buf, FILE *dest)
{
    *buf = malloc(sizeof(WriteBuf));
    (*buf)->pos = 0;
    (*buf)->max = WB_BUF_SIZE;
    (*buf)->dest = dest;
}

void WriteBuf_close(WriteBuf **buf)
{
    WriteBuf_flush(*buf);
    free(*buf);
    *buf = NULL;
}

void WriteBuf_writec(WriteBuf *buf, char val)
{
    buf->buf[buf->pos++] = val;

    if (buf->pos == buf->max)
    {
        WriteBuf_flush(buf);
    }
}

void WriteBuf_write(WriteBuf *buf, char *src, size_t num)
{
    size_t written = 0;
    while (written < num)
    {
        size_t chunk = WB_MIN(buf->max - buf->pos, num - written);
        memcpy(buf->buf + buf->pos, src + written, chunk);
        buf->pos += chunk;
        written += chunk;

        if (buf->pos == buf->max)
        {
            WriteBuf_flush(buf);
        }
    }
}

void WriteBuf_flush(WriteBuf *buf)
{
    fwrite(buf->buf, buf->pos, 1, buf->dest);
    buf->pos = 0;
}
