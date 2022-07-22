all:
	gcc main.c -o 1
	gcc mul-main.c -o 2 -lpthread
	./2 > out.0 && python3 an.py

clean:
	rm 1 2 out.0
