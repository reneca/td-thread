/**
 * \file ThreadConcurrency.cpp
 * \brief Main file for threads
 * \author Jeremy HERGAULT (reneca)
 * \version 1.0
 * \date 2017-09-31
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

#include "ThreadConcurrency.h"
#include "ThreadObject.h"
#include <unistd.h>

pthread_t ThreadConcurrency::threads[NBTHREAD];
unsigned int ThreadConcurrency::NbCandy = 0;

void ThreadConcurrency::run_candy(void* (*start_routine)(void*))
{
	cout << "Candy before = " << NbCandy << endl;

	unsigned short i;
	for (i = 0; i < NBTHREAD; i++)
	{
		// Start threads
		pthread_create(&threads[i], nullptr, start_routine, nullptr);
#ifndef __APPLE__
		pthread_setname_np(threads[i], ThreadObject::getThreadName("candy_", i).c_str());
#endif
	}

	for (i = 0; i < NBTHREAD; i++)
	{
		// Wait threads
		pthread_join(threads[i], nullptr);
	}

	cout << "Candy after  = " << NbCandy << endl;
}


void* ThreadConcurrency::candy_thread_process(void* arg)
{
	int wNbCandy = NbCandy;

	sleep(1);

	wNbCandy += 21;
	NbCandy = wNbCandy;

	return nullptr;
}


// End of file
