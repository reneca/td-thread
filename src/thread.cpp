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
	NONE			= 0,
	PTHREAD		= 10,
	PTHREADPS,
	STDTHREAD,

	CANDY			= 20,
	CANDY_MUTEX,
	CANDY_READWRITE,
	CANDY_PRODUCTCONSUM
} typeThread = PTHREAD;

// By default run 6 threads
unsigned int nbConcurentThread = 6;

// Not print thread infos by default
bool showThreadInfos = false;


/**
 * \fn void run_pthread()
 * \brief Function to run a Pthread and control the return value
 */
void run_pthread()
{
	cout << " pthread[" << nbConcurentThread << ']' << endl;

	pthread_t threads[nbConcurentThread];
	int wReturnThread;
	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		wReturnThread = pthread_create( &threads[i], nullptr, ThreadObject::thread_start, (void*)new ThreadObject(i, showThreadInfos));
	}

	unsigned int* wReturnValue;
	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		wReturnThread = pthread_join( threads[i], (void**)&wReturnValue);
		if (wReturnValue != nullptr)
		{
			if (*wReturnValue != 0)
				cerr << "The thread " << i << " return an error : " << *wReturnValue << endl;

			delete wReturnValue;
		}
	}
}

/**
 * \fn void run_pthreadps()
 * \brief Function to run thread to be watched by ps
 */
void run_pthreadps()
{
	cout << " pthreadps[" << nbConcurentThread << ']' << endl;

	pthread_t threads[nbConcurentThread];
	ThreadObject* wThreadObject;
	int wReturnThread;
	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		wThreadObject = new ThreadObject(i, false);
		wReturnThread = pthread_create( &threads[i], nullptr, ThreadObject::thread_wait, (void*)wThreadObject);
#if !defined(__APPLE__)
		pthread_setname_np(threads[i], wThreadObject->getThreadName().c_str());
#endif
	}

	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		wReturnThread = pthread_join(threads[i], nullptr);
	}
}

/**
 * \fn void run_stdthread()
 * \brief Function to run a std::thread
 */
void run_stdthread()
{
	cout << " stdthread[" << nbConcurentThread << ']' << endl;

	thread* threads[nbConcurentThread];
	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		threads[i] = new thread(ThreadObject::thread_bench, new ThreadObject(i, showThreadInfos));
	}

	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		threads[i]->join();
	}
}

/**
 * \fn void printHelp()
 * \brief Function to print program help
 */
void printHelp(const char* progName)
{
	cout << progName << endl
	     << "TP1 :" << endl
	     << "\t- pthread" << endl
		 << "\t- stdthread" << endl
		 << "\t- pthreadps" << endl
		 << "TP2 :" << endl
		 << "\t- candy" << endl
		 << "\t- candymutex" << endl
		 << "\t- candyrw" << endl
	     << "\t- candypc" << endl;
}


int main(int argc, char *argv[])
{
	// Get the main parameters
	if (argc > 1)
	{
		string wThreadType(argv[1]);
		std::transform(wThreadType.begin(), wThreadType.end(), wThreadType.begin(), ::tolower);
		if (wThreadType == "pthread")
			typeThread = ThreadType::PTHREAD;
		else if (wThreadType == "stdthread")
			typeThread = ThreadType::STDTHREAD;
		else if (wThreadType == "pthreadps")
			typeThread = ThreadType::PTHREADPS;
		else if (wThreadType == "candy")
			typeThread = ThreadType::CANDY;
		else if (wThreadType == "candymutex")
			typeThread = ThreadType::CANDY_MUTEX;
		else if (wThreadType == "candyrw")
			typeThread = ThreadType::CANDY_READWRITE;
		else if (wThreadType == "candypc")
			typeThread = ThreadType::CANDY_PRODUCTCONSUM;
		else
		{
			printHelp(argv[0]);
			return 0;
		}

		// Get number of concurrent thread
		if (argc > 2)
		{
			nbConcurentThread = strtoul(argv[2], nullptr, 0);
			if (nbConcurentThread == 0)
				nbConcurentThread = 1;
		}
	}
	else
	{
		printHelp(argv[0]);
		return 0;
	}

	// Print thread infos only if it's not to wide
	if (nbConcurentThread < 1000)
		showThreadInfos = true;

	// Run threads
	switch (typeThread)
	{
		case ThreadType::PTHREAD:
			run_pthread();
			break;

		case ThreadType::PTHREADPS:
			run_pthreadps();
			break;

		case ThreadType::STDTHREAD:
			run_stdthread();
			break;

		case ThreadType::CANDY:
			ThreadConcurrency::run_candy(ThreadConcurrency::candy_thread_process);
			break;

		case ThreadType::CANDY_MUTEX:
			ThreadConcurrency::run_candy(ThreadConcurrency::candy_thread_mutex);
			break;

		case ThreadType::CANDY_READWRITE:
			ThreadConcurrency::run_two_candy(ThreadConcurrency::candy_thread_writer, ThreadConcurrency::candy_thread_reader);
			break;

		case ThreadType::CANDY_PRODUCTCONSUM:
			ThreadConcurrency::run_two_candy(ThreadConcurrency::candy_thread_producer, ThreadConcurrency::candy_thread_consumer);
			break;

		default :
			return -1;
	}

	return 0;
}


// End of file
