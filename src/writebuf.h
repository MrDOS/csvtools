/*
 * File:    writebuf.h
 * Author:  Samuel Coleman
 *
 * Description:
 *
 * A simple buffered writer.
 */

#ifndef WRITEBUF_H
#define WRITEBUF_H

#include <stdio.h>

#define WB_BUF_SIZE 8096

typedef struct
{
    char buf[WB_BUF_SIZE];
    size_t pos;
    size_t max;
    FILE *dest;
} WriteBuf;

/*
 * Allocate the write buffer.
 */
void WriteBuf_new(WriteBuf **buf, FILE *dest);

/*
 * Close the write buffer. Flushes the buffer on the way out.
 */
void WriteBuf_close(WriteBuf **buf);

/*
 * Write a single character to the buffer.
 */
void WriteBuf_writec(WriteBuf *buf, char val);

/*
 * Write a number of bytes to the buffer.
 */
void WriteBuf_write(WriteBuf *buf, char *src, size_t num);

/*
 * Flush the contents of the buffer to output.
 */
void WriteBuf_flush(WriteBuf *buf);

#endif
