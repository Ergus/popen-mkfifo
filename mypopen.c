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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <paths.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>

struct pipe_set {
	pid_t pid;
	FILE *fd[3];
};

struct pipe_set *mypopen(const unsigned int modes, char *const cmd[])
{
	if ((modes & 7) == 0) {
		errno = EINVAL;
		return NULL;
	}

	struct pipe_set * volatile ret = malloc(sizeof(struct pipe_set));
	if (ret == NULL) {
		return NULL;
	}

	int pdes[3][2];

	if (((modes & (1 << 0)) && pipe(pdes[0]) < 0)
	    || ((modes & (1 << 1)) && pipe(pdes[1]) < 0)
	    || ((modes & (1 << 2)) && pipe(pdes[2]) < 0)) {
		free(ret);
		return NULL;
	}

	pid_t pid  = fork();

	if (pid == 0) { // Child
		for (int i = 0; i < 3; ++i) {
			if (modes & (1 << i)) {
				close(pdes[i][1 - (bool)i]);
				dup2(pdes[i][(bool) i], i);
				close(pdes[i][(bool) i]);
			}
		}

		execv(cmd[0], cmd);

		exit(127);

	} else if (pid == -1) {			/* Error. */
		for (size_t i = 0; i < 3; ++i) {
			close(pdes[i][0]);
			close(pdes[i][1]);
		}
		free(ret);
		return NULL;
	}


	// Parent; assume fdopen can't fail.
	ret->pid = pid;
	for (int i = 0; i < 3; ++i) {
		ret->fd[i] = NULL;

		if (modes & (1 << i)) {
			ret->fd[i] = fdopen(pdes[i][1 - (bool)i], (i ? "r" : "w"));
			close(pdes[i][(bool)i]);
		}
	}

	return ret;
}

struct pipe_set *mymkfifo(const char pipename[], char *const cmd[])
{
	struct pipe_set * volatile ret = malloc(sizeof(struct pipe_set));
	if (ret == NULL) {
		return NULL;
	}

	mkfifo(pipename, 0666);
	pid_t pid  = fork();

	if (pid == 0) {
		int fd = open(pipename, O_RDONLY);
		dup2(fd, 0);
		close(fd);

		execv(cmd[0], cmd);
		exit(127);
	}  else if (pid == -1) {			/* Error. */
		return NULL;
	}

	ret->pid = pid;
	ret->fd[0] = fopen(pipename, "w");
	assert(ret->fd[0] != NULL);
	ret->fd[1] = NULL;
	ret->fd[2] = NULL;

	return ret;
}

int mywaitpid(struct pipe_set *pipes)
{
	int pstat;
	pid_t pid;
	do {
		pid = waitpid(pipes->pid, &pstat, 0);
	} while (pid == -1 && errno == EINTR);
	return (pid == -1 ? -1 : pstat);
}

int mypclose(struct pipe_set *pipes)
{
	for (int i = 0; i < 3; ++i) {
		if (pipes->fd[i] != NULL) {
			fclose(pipes->fd[i]);
		}
	}

	int ret = mywaitpid(pipes);

	free(pipes);
	return ret;
}
