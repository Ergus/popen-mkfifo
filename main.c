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


#include <stdio.h>
#include "mypopen.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: ./%s executable\n", argv[0]);
		exit(1);
	}

	char **cmd = &argv[1];

	// Open the pipes: 1 in; 2 out; 4 err -> 7 = (1|2|4)
	struct pipe_set *myset = mypopen(7, cmd);

	// Write to process, fflush is very needed.
	fprintf(myset->fd[0], "HelloWorld\n");
	fflush(myset->fd[0]);

	// Read from the stdout
	char buffer[128];
	while (fgets(buffer, 128, myset->fd[1]) != NULL) {
		printf("FROM STDOUT: %s", buffer);
	}

	// Read from the stderr
	while (fgets(buffer, 128, myset->fd[2]) != NULL) {
		printf("FROM STDERR: %s", buffer);
	}

	// Close the pipes
	mypclose(myset);

	return 0;
}
