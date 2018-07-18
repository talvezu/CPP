SOURCE=threads_order.cpp
MYPROGRAM=threads_order
MYINCLUDES=.

CDEBUG = -g3 
CPPVER = -std=c++1z
MYLIBRARIES=pthread
CC=g++

all: $(MYPROGRAM)

$(MYPROGRAM): $(SOURCE)

	@echo $(SOURCE)
	$(CC) $(CDEBUG) $(CPPVER) -I$(MYINCLUDES) $(SOURCE) -o$(MYPROGRAM) -l$(MYLIBRARIES)

clean:

	rm -f $(MYPROGRAM)

PROG = threads_order
