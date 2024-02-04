run:
	gcc -o shell shell.c -lreadline -pthread -lSDL2 -lcurl -ljson-c -lopencv_core -lopencv_highgui -lopencv_imgproc `pkg-config --cflags --libs opencv` -lpthread
	./shell
clean:
	rm -f shell
