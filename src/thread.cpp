/**
 * \file thread.cpp
 * \brief Main file for threads
 * \author Jeremy HERGAULT (reneca)
 * \version 1.0
 * \date 2017-09-17
 *
 * This file is part of TD-Thread, which is a TD for thread learning
 * Copyright (C) 2017  HERGAULT Jeremy, Alexandre, Thierry ( reneca )
 *
 * TD-Thread is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TD-Thread is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TD-Thread. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>

#include <thread>
#include <pthread.h>

#include "ThreadObject.h"
#include "ThreadConcurrency.h"


using namespace std;


// Define all thread type by default run pthread
enum ThreadType : unsigned char {
	CANDY			= 10,
} typeThread = CANDY;

/**
 * \fn void printHelp()
 * \brief Function to print program help
 */
void printHelp(const char* progName)
{
	cout << progName << endl
	     << "TP1 :" << endl
		 << "TP2 :" << endl
		 << "\t- candy" << endl;
}


int main(int argc, char *argv[])
{
	// Get the main parameters
	if (argc > 1)
	{
		string wThreadType(argv[1]);
		std::transform(wThreadType.begin(), wThreadType.end(), wThreadType.begin(), ::tolower);
		if (wThreadType == "candy")
			typeThread = ThreadType::CANDY;
		else
		{
			printHelp(argv[0]);
			return 0;
		}
	}
	else
	{
		printHelp(argv[0]);
		return 0;
	}

	// Run threads
	switch (typeThread)
	{
		case ThreadType::CANDY:
			ThreadConcurrency::run_candy(ThreadConcurrency::candy_thread_process);
			break;

		default :
			return -1;
	}

	return 0;
}


// End of file
