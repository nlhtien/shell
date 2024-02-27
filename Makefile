run:
	gcc -o shell shell.c -lreadline -pthread -lSDL2 -lcurl -ljson-c -lopencv_core -lopencv_highgui -lopencv_imgproc `pkg-config --cflags --libs opencv` -lpthread -ljansson -lxml2 -I/usr/include/libxml2 -L/usr/lib -lSDL2_gfx
	./shell
clean:
	rm -f shell
