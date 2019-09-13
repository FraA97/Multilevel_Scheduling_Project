CC = gcc
CFILES = main.c list.h list.c os.h os.c pcb.h pcb.c

scheduling_simulator:
	$(CC) -o scheduling_simulator $(CFILES)
