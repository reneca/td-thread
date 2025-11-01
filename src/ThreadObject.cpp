/**
 * \file ThreadObject.cpp
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

#include "ThreadObject.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <unistd.h>
#include <sys/syscall.h>

ThreadObject::ThreadObject(size_t threadNum, bool printThread)
	: ThreadNum(threadNum), PrintThread(printThread)
{
	// Do nothing else

}

ThreadObject::~ThreadObject()
{
	// Do nothing
}


string ThreadObject::getThreadName() const
{
	ostringstream wThreadName("polythread_", ios_base::in | ios_base::out | ios_base::ate);
	wThreadName << setfill('0') << setw(3) << ThreadNum;

	return wThreadName.str().substr(0, 15);
}

string ThreadObject::getThreadName(const char* prefix, unsigned short threadID)
{
	ostringstream wThreadName(prefix, ios_base::in | ios_base::out | ios_base::ate);
	wThreadName << setfill('0') << setw(4) << threadID;

	return wThreadName.str().substr(0, 15);
}

void ThreadObject::printThread() const
{
	if (PrintThread)
		cout << "Je suis le thread " << ThreadNum << " avec pour tid : " << getThreadID() << endl;
}

pid_t ThreadObject::getThreadID()
{
#ifdef __APPLE__
	{
		uint64_t tid;
		pthread_threadid_np(NULL, &tid);
		return static_cast<pid_t>(tid);
	}
#else
	return static_cast<pid_t>(syscall(SYS_gettid));
#endif
}

void* ThreadObject::thread_start(void* arg)
{
	ThreadObject* wObject = reinterpret_cast<ThreadObject*>(arg);

	wObject->printThread();
	delete wObject;

	// Only thread 3 return an error
	if (wObject->getThreadNum() != 3)
		return new unsigned int(0);
	else
		return new unsigned int(2);
}

void* ThreadObject::thread_wait(void* arg)
{
	ThreadObject* wObject = reinterpret_cast<ThreadObject*>(arg);
#ifdef __APPLE__
	pthread_setname_np(wObject->getThreadName().c_str());
#endif

	sleep(1);
	wObject->printThread();

	if (wObject->getThreadNum() == 2)
	{
		// Active wait
		time_t wStartTime = time(nullptr);
		while (wStartTime > (time(nullptr) - 4));
	}
	else
		sleep(4);

	delete wObject;

	return nullptr;
}

void* ThreadObject::thread_bench(void* arg)
{
	ThreadObject* wObject = reinterpret_cast<ThreadObject*>(arg);

	wObject->printThread();
	delete wObject;

	return nullptr;
}
