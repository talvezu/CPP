PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = aligned_ptr.o
CFLAGS += -g3 -fpermissive

all:    Experiment

aligned_ptr:     $(OBJS)
        $(CXX) -o $@ $^

%.o:    $(PROJECT_ROOT)%.cpp
        $(CXX) -std=c++1y -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:    $(PROJECT_ROOT)%.c
        $(CC) -std=c++1y -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
        rm -fr aligned_ptr $(OBJS)
~                                   
