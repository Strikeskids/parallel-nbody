herdShell: herdShell.c
	gcc -lm -lGL -lGLU -lglut herdShell.c -o herdShell

clean:
	rm -f herdShell

