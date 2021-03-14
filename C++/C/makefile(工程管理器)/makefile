OBJS=main.o tool1.o tool2.o #变量代替
	
CC=gcc
CFLAGS+=-c -Wall -g

mytool:$(OBJS)
	$(CC) $^ -o $@


%.o:%.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	$(RM) *.o mytool -r #rm *.o mytool -rf
