all: cpuben diskben memben server

cpuben: cpuben.c
	gcc cpuben.c -lm -pthread -o cpuben

diskben: diskben.c
	gcc diskben.c -lm -pthread -o diskben

memben: memben.c
	gcc memben.c -lm -pthread -o memben

server: server.c
	gcc server.c -pthread -o server

clean:
	rm cpuben diskben memben server
