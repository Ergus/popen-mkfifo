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

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

int main()
{
	std::cout << "PID: " << getpid() << std::endl;
	std::cout << "PPID: " << getppid() << std::endl;

	std::string input;
	std::cin >> input;

	std::cout << "OUT: " << input << std::endl;
	std::cerr << "ERR: " << input << std::endl;

	std::ofstream ss;
	ss.open ("/tmp/log.txt");

	ss << "FILE: " << input << std::endl;
	ss.close();

	return 0;
}
