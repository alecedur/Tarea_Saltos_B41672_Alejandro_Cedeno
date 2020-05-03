branchmake:
	g++ main.cpp -o a.o

compile:
	gunzip -c branch-trace-gcc.trace.gz |./a.o -s 3 -bp 3 -gh 4 -ph 3 -o 1

torneo_compile:
	gunzip -c branch-trace-gcc.trace.gz |head -5000 |./a.o -s 3 -bp 3 -gh 4 -ph 3 -o 1

priv_compile:
	gunzip -c branch-trace-gcc.trace.gz |head -5000 |./a.o -s 5 -bp 2 -gh 3 -ph 2 -o 1	

glob_compile:
	gunzip -c branch-trace-gcc.trace.gz |head -5000 |./a.o -s 3 -bp 1 -gh 4 -ph 3 -o 1	

bim_compile:
	gunzip -c branch-trace-gcc.trace.gz |head -5000 |./a.o -s 3 -bp 0 -gh 4 -ph 3 -o 1