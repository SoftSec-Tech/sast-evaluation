COMPILER=gcc

default: all

.PHONY: default all

all: compliance1 compliance2 compliance3 compliance4 compliance5 cross1 quality1 quality2 quality3

compliance1:
	$(COMPILER) -c compliance/compliance01.c
compliance2:
	$(COMPILER) -c compliance/compliance02.c
compliance3:
	$(COMPILER) -c compliance/compliance03.c
compliance4:
	$(COMPILER) -c compliance/compliance04.c
compliance5:
	$(COMPILER) -c compliance/compliance05.c

cross1:
	$(COMPILER) -c cross/test1.cpp

quality1:
	$(COMPILER) -c quality/res-leak01.cpp
quality2:
	$(COMPILER) -c quality/res-leak02.cpp
quality3:
	$(COMPILER) -c quality/res-leak03.cpp

clean:
	rm -r *.o
