all: sshell
main.o: tools.h main.c
	$(CC) $(CCFLAGS) -c main.c
sshell: main.o 
	$(CC) $(CCFLAGS) -o sshell main.o
clean:
	rm -f sshell *~ *.o *.bak core \#*
