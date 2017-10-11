/**
 * \file ThreadObject.h
 * \brief Object for thread messaging
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

#ifndef THREADOBJECT_H
#define THREADOBJECT_H

#include <iostream>

using namespace std;

class ThreadObject
{
	private :
		size_t ThreadNum;	/**< Instance ID */
		bool PrintThread;	/**< Indicate if the thread infos must be printed */

	public :
		/**
		 * \fn ThreadObject(size_t threadNum, bool printThread = true)
		 * \brief Constuctor of ThreadObject
		 *
		 * \param[in] threadNum Thread instance ID
		 *
		 * \param[in] printThread Indicate if the thread infos will be shows
		 */
		ThreadObject(size_t threadNum, bool printThread = true);
		/**
		 * \fn virtual ~ThreadObject()
		 * \brief Destructor of ThreadObject
		 */
		virtual ~ThreadObject();

	public :
		/**
		 * \fn size_t getThreadNum() const
		 * \brief Getter to retrieve the thread instance ID
		 */
		size_t getThreadNum() const;

	public :
		/**
		 * \fn string getThreadName() const
		 * \brief Method to get the thread name (use for naming Pthread)
		 *
		 * \return the thread name
		 */
		string getThreadName() const;
		/**
		 * \fn static string getThreadName(const char* prefix, unsigned short threadID)
		 * \brief Static method to format a thread name by a thread ID
		 *
		 * \param[in] prefix of the thread name
		 *
		 * \param[in] threadID ID of the thread
		 *
		 * \return the thread name custom with his thread name
		 */
		static string getThreadName(const char* prefix, unsigned short threadID);
		/**
		 * \fn void printThread() const
		 * \brief Method to print thread info to stdout if the PrintThread is true
		 */
		void printThread() const;
};


inline size_t ThreadObject::getThreadNum() const { return ThreadNum; }


#endif /* THREADOBJECT_H */
