#https://stackoverflow.com/questions/17834582/run-make-in-each-subdirectory
print-%  : ; @echo $* = $($*)

#(Just to point out this is GNU make specific; you didn't mention any restrictions on the version of make you're using).
#ETA Here's a version which supports multiple top-level targets.

TOPTARGETS := all clean

SUBDIRS := $(wildcard */.)

#SOURCES := $(shell find . -name '*.cpp')
#FILES := $(filter-out ./sources/poll.cpp, $(SOURCES))

SUBDIRS := $(filter-out includes/., $(SUBDIRS))
$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
