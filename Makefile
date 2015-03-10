all: compile

compile:
	$(CC) hr.c -o hr.exe

clean:
	rm hr.exe
