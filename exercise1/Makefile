exe: main.o lines.o producer.o consumer.o
	gcc -o exe main.o lines.o producer.o consumer.o -lpthread
	
exe.o: main.c lines.c producer.c consumer.c
	gcc -c main.c lines.c producer.c consumer.c -lpthread

