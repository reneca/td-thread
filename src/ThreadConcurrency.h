/**
 * \file ThreadConcurrency.h
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

#ifndef THREADCONCURRENCY_H
#define THREADCONCURRENCY_H

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <thread>
#include <pthread.h>

#define NBTHREAD 6

using namespace std;

class ThreadConcurrency
{
	private :
		static pthread_t threads[NBTHREAD];		/*< Tab which contain threads' ids */
		static unsigned int NbCandy;			/*< Candy number shared variable */

	public :
		/**
		 * \fn static unsigned int getNbCandy()
		 * \brief Getter to get Number of candy
		 *
		 * \return the number of candy
		 */
		static unsigned int getNbCandy();

	public :
		/**
		 * \fn static void run_candy(void* (*start_routine)(void*))
		 * \brief Method to run N thread routine concurrently
		 *
		 * \param[in] start_routine thread routine to execute
		 */
		static void run_candy(void* (*start_routine)(void*));

	public :
		/**
		 * \fn static void* candy_thread_process(void* arg)
		 * \brief static function for thread routine
		 *
		 * \param[in] arg pointer on object thread argument
		 *
		 * \return pointer on returned thread object
		 */
		static void* candy_thread_process(void* arg);
};


inline unsigned int ThreadConcurrency::getNbCandy() { return NbCandy; }


#endif /* THREADCONCURRENCY_H */
