/*
 *  StdInput.h
 *
 *
 *  Created by Stéphane Nicolet on 03/10/09.
 *
 */

#ifndef STANDARD_INPUT_H
#define STANDARD_INPUT_H


#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define BUFFER_SIZE 1024*16

extern FILE *input_stream;
extern char cmd_buffer[BUFFER_SIZE];
extern char line_buffer[BUFFER_SIZE];
extern char *args[512];


void SetReadStream(FILE *stream);
void ReadClear(void);
int ReadInput(void);
int Read(char *buffer);
int ReadParse(char *buffer, char *args[], const char *delims);



#endif  /*  STANDARD_INPUT_H  */
