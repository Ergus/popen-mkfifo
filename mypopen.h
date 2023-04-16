/*
 * Copyright (C) 2022  Jimmy Aguilar Mena
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MYPOPEN_H
#define MYPOPEN_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
   A struct pipe_set.
   This a struct returned by mypopen calls. The function allocates them with
   malloc, and mypclose releases it with free.
 */
struct pipe_set {
	pid_t pid;    /**< PID of the new process. */
	FILE *fd[3];  /**< Array of file descriptors; null when not connected. */
};

/**
   Function to create a new process.

   Depending of the modes argument the function creates a new process with
   fork+exec, and then returns a struct pipe_set where fd are connected to the
   process' stdin, stdout and stderr.
   \param modes is an unsigned integer which works like a mask of the connected
   outputs.
   1: fd[0] is connected to the process stdin, and it is possible to
   send commands to the input by writing to it.
   2: fd[1] is connected to process stdout, and it is possible to read the
   processes' output.
   4: fd[2] is connected to process stderr.
   To connect all the descriptors use: (1|2|4) => 7.
   \param cmd a string with the full command line to start the new process. This
   arguments are passed to execv.
   \return a pipe_set object with the pid and file descriptors information based
   on mask.
 */
struct pipe_set *mypopen(const unsigned int modes, char *const cmd[]);

/**
   Close the connection to the process and release the pipes object.
   \param the process descriptor as returned by mypopen. This function calls
   free to release the heap allocated memory.
 */
int mypclose(struct pipe_set *pipes);

/**
   Create a process using mkfifo instead of fork+exec

   mkfifo creates a physical file descriptor in the filesystem which is possible
   to write like: 'echo "bla" > file'; and the command will be passed to the
   processes standard input.
   \param pipename The path of the desired file descriptor
   \param cmd similar to similar argument in mypopen.
   \return a pipe_set object with the pid and file descriptors information. Only
   fd[0] is set in this function.
 */
struct pipe_set *mymkfifo(const char pipename[], char *const cmd[]);

/**
   Wait for the process described in pipes to finish.
 */
void mywaitpid(struct pipe_set *pipes);

#endif // MYPOPEN_H
