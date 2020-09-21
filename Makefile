# \file Makefile
# \brief Make to build program and execute program
# \author Jeremy HERGAULT (reneca)
# \version 1.0
# \date 2017-09-17
#
# This file is part of TD-Thread, which is a TD for thread learning
# Copyright (C) 2017  HERGAULT Jeremy, Alexandre, Thierry ( reneca )
#
# TD-Thread is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# TD-Thread is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with TD-Thread. If not, see <http://www.gnu.org/licenses/>.


.PHONY: all debug clean test ps top candy mutex readwrite help


OBJS=$(subst .cpp,.o,$(subst src,lib,$(wildcard src/*.cpp)))
CXX=g++ -O2
CXXFLAGS=-std=c++14

# For C++ objs
lib/%.o: src/%.cpp
	@mkdir -p lib
	$(CXX) $(CXXFLAGS) -o $@ -c $<

all: thread ## Build thread by default

thread: $(OBJS) ## Build thread bin
	$(CXX) $(CXXFLAGS) $(OBJS) -o thread -pthread

debug: CXX=g++ -g3
debug: thread ## Build thread debug bin

clean: ## Clean all compil files
	rm -rf lib thread

docker: ## Run debian docker to run correction
	docker run -ti --rm -v $(shell pwd):/opt/td-thread overware/ovbuilder:debian


# TD1
test: thread ## TD1 - Test pthread
	./thread pthread

ps: thread ## TD1 - Show processus threads
	./thread pthreadps 8&
	sleep 2
	#ps -Llf -p `pgrep -f thread.o | head -1`
	ps H -o 'flag,state,pid,ppid,pgid,pmem,rss,rsz,pcpu,time,cmd,comm' -p `pgrep -f ./thread | head -1`

top: thread ## TD1 - Show live processus threads
	./thread pthreadps 8&
	top -H -p `pgrep -f ./thread | head -1`


# TD2
candy: thread ## TD2 - Build candy first example
	./thread candy

mutex: thread ## TD2 - Build candy with mutex protection
	./thread candymutex&
	@sleep 1
	@ps H -o 'flag,state,pid,ppid,pgid,pmem,rss,rsz,pcpu,time,cmd,comm' -p `pgrep -f ./thread | head -1`

readwrite: thread ## TD2 - Build candy reader / writer
	./thread candyrw&
	@sleep 1
	@ps H -o 'flag,state,pid,ppid,pgid,pmem,rss,rsz,pcpu,time,cmd,comm' -p `pgrep -f ./thread | head -1`

prodconsum: thread ## TD2 - Build candy producer / consumer
	./thread candypc&
	@sleep 1
	@ps H -o 'flag,state,pid,ppid,pgid,pmem,rss,rsz,pcpu,time,cmd,comm' -p `pgrep -f ./thread | head -1`


# Help
help:
	@grep -hE '(^[\.a-zA-Z_-]+:.*?##.*$$)|(^##)' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[32m%-10s\033[0m %s\n", $$1, $$2}' | sed -e 's/\[32m##/[33m/'
	@if [ -f "thread" ]; then echo; ./thread help; fi


# End of Makefile
