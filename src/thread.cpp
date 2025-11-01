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
#include <memory>
#include <string>
#include <algorithm>
#include <unistd.h>

#include <thread>
#include <pthread.h>

#include "ThreadObject.h"
#include "ThreadConcurrency.h"

using namespace std;

// Not print thread infos by default
bool showThreadInfos = false;

/**
 * \fn void run_pthread()
 * \brief Function to run a Pthread and control the return value
 *
 * \param[in] nbConcurentThread Number of concurrent thread to run
 */
void run_pthread(const unsigned int nbConcurentThread)
{
	cout << " pthread[" << nbConcurentThread << ']' << endl;

	std::vector<pthread_t> threads;
	int wReturnThread;
	for (size_t i = 0; i < nbConcurentThread; i++)
	{
	    pthread_t threadId;
		wReturnThread = pthread_create( &threadId, nullptr, ThreadObject::thread_start, (void*)new ThreadObject(i, showThreadInfos));
		threads.push_back(threadId);
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
 *
 * \param[in] nbConcurentThread Number of concurrent thread to run
 */
void run_pthreadps(const unsigned int nbConcurentThread)
{
	cout << " pthreadps[" << nbConcurentThread << ']' << endl;

	std::vector<pthread_t> threads;
	ThreadObject* wThreadObject;
	int wReturnThread;
	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		wThreadObject = new ThreadObject(i, false);
		pthread_t threadId;
		wReturnThread = pthread_create( &threadId, nullptr, ThreadObject::thread_wait, (void*)wThreadObject);
#if !defined(__APPLE__)
		pthread_setname_np(threadId, wThreadObject->getThreadName().c_str());
#endif
        threads.push_back(threadId);
	}

	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		wReturnThread = pthread_join(threads[i], nullptr);
	}
}

/**
 * \fn void run_stdthread()
 * \brief Function to run a std::thread
 *
 * \param[in] nbConcurentThread Number of concurrent thread to run
 */
void run_stdthread(const unsigned int nbConcurentThread)
{
	cout << " stdthread[" << nbConcurentThread << ']' << endl;

	std::vector<shared_ptr<thread>> threads;
	for (size_t i = 0; i < nbConcurentThread; i++)
	{
		threads.push_back(std::make_shared<thread>(ThreadObject::thread_bench, new ThreadObject(i, showThreadInfos)));
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

		// Get number of concurrent thread
		// By default run 6 threads
		unsigned int nbConcurentThread = 6;
		if (argc > 2)
		{
			nbConcurentThread = strtoul(argv[2], nullptr, 0);
			if (nbConcurentThread == 0)
				nbConcurentThread = 1;
		}

		// Print thread infos only if it's not to wide
		if (nbConcurentThread < 1000)
			showThreadInfos = true;

		if (wThreadType == "pthread")
		    run_pthread(nbConcurentThread);
		else if (wThreadType == "stdthread")
		    run_stdthread(nbConcurentThread);
		else if (wThreadType == "pthreadps")
		    run_pthreadps(nbConcurentThread);
		else if (wThreadType == "candy")
		    ThreadConcurrency::run_candy(ThreadConcurrency::candy_thread_process);
		else if (wThreadType == "candymutex")
		    ThreadConcurrency::run_candy(ThreadConcurrency::candy_thread_mutex);
		else if (wThreadType == "candyrw")
		    ThreadConcurrency::run_two_candy(ThreadConcurrency::candy_thread_writer, ThreadConcurrency::candy_thread_reader);
		else if (wThreadType == "candypc")
		    ThreadConcurrency::run_two_candy(ThreadConcurrency::candy_thread_producer, ThreadConcurrency::candy_thread_consumer);
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

	return 0;
}
