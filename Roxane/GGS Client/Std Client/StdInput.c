/*
 *  StdInput.c
 *
 *  Input/output routines for the engine and the engine protocol.
 *
 *  Created by Stéphane Nicolet on 03/10/09.
 *
 *  Most of the code below in taken from Robert Hyatt's chess 
 *  program "crafty" (thanks). See http://www.craftychess.com/ 
 *  for the crafty source code.
 *  
 *
 */


#include "StdInput.h"

/* global variables */
FILE *input_stream;
char cmd_buffer[BUFFER_SIZE];
char line_buffer[BUFFER_SIZE];
char *args[512];

static int standard_input_unit_initialized = 0;


/*
 *******************************************************************************
 *                                                                             *
 *   SetReadStream() will be used to set the read stream to the standard input *
 *                                                                             *
 *******************************************************************************
 */
void SetReadStream(FILE *stream) {
	int i;
	
	standard_input_unit_initialized = 1;
	
	input_stream = stream;
	for (i = 0; i < 512; i++)
		args[i] = (char *) malloc(128);
	
	ReadClear();
	
}


/*
 *******************************************************************************
 *                                                                             *
 *   ReadClear() clears the input buffer when input_stream is being switched to*
 *   a file, since we have info buffered up from a different input stream.     *
 *                                                                             *
 *******************************************************************************
 */
void ReadClear(void) {
	
//	if (!standard_input_unit_initialized) 
//		SetReadStream(stdin);
	
	cmd_buffer[0] = 0;
}

/*
 *******************************************************************************
 *                                                                             *
 *   ReadInput() reads data from the input_stream, and buffers this into the   *
 *   command_buffer for later processing.                                      *
 *                                                                             *
 *******************************************************************************
 */
int ReadInput(void) {
	
	char buffer[BUFFER_SIZE], *end;
	long long bytes;
	
//	if (!standard_input_unit_initialized) 
//		SetReadStream(stdin);
	
	
	do
		bytes = read(fileno(input_stream), buffer, BUFFER_SIZE);
	while (bytes < 0 && errno == EINTR);
	
	
	if (bytes == 0) {
		
		if (input_stream != stdin)
			fclose(input_stream);
		input_stream = stdin;
		return (0);
		
	} else if (bytes < 0) {
		
		printf("ERROR!  input I/O stream is unreadable, exiting.\n");
				
	}
	
	end = cmd_buffer + strlen(cmd_buffer);
	memcpy(end, buffer, bytes);
	*(end + bytes) = 0;
	
	return (1);
}


/*
 *******************************************************************************
 *                                                                             *
 *   Read() copies data from the command_buffer into a local buffer, so that   *
 *   the user can call ReadParse to break this command up into tokens for      *
 *   processing.                                                               *
 *                                                                             *
 *******************************************************************************
 */
int Read(char *buffer) {
  char *eol, *ret, readdata;
	
//	if (!standard_input_unit_initialized) 
//		SetReadStream(stdin);
	
  *buffer = 0;
	/*
	 case 1:  We have a complete command line, with terminating
	 N/L character in the buffer.  We can simply extract it from
	 the I/O buffer, parse it and return.
	 */
	
  if (strchr(cmd_buffer, '\n')) ;
	
	
	/*
	 case 3:  The buffer does not contain a complete line, but we
	 were asked to wait until a complete command is entered.  So we
	 hang by doing a ReadInput() and continue doing so until we get
	 a N/L character in the buffer.  Then we parse and return.
	 */
  else 
    while (!strchr(cmd_buffer, '\n')) {
      readdata = ReadInput();
      if (!readdata)
        return (-1);
    }
	
	
  eol = strchr(cmd_buffer, '\n');
  *eol = 0;
  ret = strchr(cmd_buffer, '\r');
  if (ret)
    *ret = ' ';
  strcpy(buffer, cmd_buffer);
  memmove(cmd_buffer, eol + 1, strlen(eol + 1) + 1);
	
  return (1);
}

/*
 *******************************************************************************
 *                                                                             *
 *   ReadParse() takes one complete command-line, and breaks it up into        *
 *   tokens. You must pass common delimiters (such as " ", ",", "/" and ";")   *
 *   in the "delims" string, any caracter of which will delimit tokens.        *
 *                                                                             *
 *******************************************************************************
 */
int ReadParse(char *buffer, char *args[], const char *delims) {
  char *next, tbuffer[BUFFER_SIZE];
  int nargs;
	
//	if (!standard_input_unit_initialized) 
//		SetReadStream(stdin);
	
  strcpy(tbuffer, buffer);
	
  for (nargs = 0; nargs < 512; nargs++)
    *(args[nargs]) = 0;
	
  next = strtok(tbuffer, delims);
  if (!next)
    return (0);
	
  strcpy(args[0], next);
	
  for (nargs = 1; nargs < 512; nargs++) {
    next = strtok(0, delims);
    if (!next)
      break;
    strcpy(args[nargs], next);
  }
	
  return (nargs);
}






