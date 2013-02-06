all: cpuben diskben memben serverben clientben

cpuben: cpuben.c
	gcc cpuben.c -lm -pthread -o cpuben

diskben: diskben.c
	gcc diskben.c -lm -pthread -o diskben

memben: memben.c
	gcc memben.c -lm -pthread -o memben

serverben: serverben.c
	gcc serverben.c -pthread -o serverben

clientben: clientben.c
	gcc clientben.c -pthread -o clientben

clean:
	rm cpuben diskben memben serverben clientben
