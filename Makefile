# Copyright (C) 2022  Jimmy Aguilar Mena

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

all: main_popen.x main_mkfifo.x libmypopen.so

%.x: %.c libmypopen.so
	gcc -g -Wall -Wl,-rpath,. -L . -lmypopen $< -o $@

%x: %cxx
	g++ -g -Wall $< -o $@

lib%.so: %.o
	gcc -shared $< -o $@

%.o: %.c
	gcc -c -Wall -Werror -fpic $<

.PHONY: clean test

clean:
	rm -rf *.o *.so *.x

test: main_popen.x main_mkfifo.x test.x test.py
	./main_popen.x test.x
	./main_popen.x test.py
	./main_popen.x /usr/bin/ls -l
	./main_mkfifo.x /usr/bin/python -i
