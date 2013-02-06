all: cpuben diskben memben

cpuben: cpuben.c
	gcc cpuben.c -lm -pthread -o cpuben

diskben: diskben.c
	gcc diskben.c -lm -pthread -o diskben

memben: memben.c
	gcc memben.c -lm -pthread -o memben

clean:
	rm cpuben diskben memben
