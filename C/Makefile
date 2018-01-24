default : VM.o decodeur.o

VM.o : VM.c
	gcc -g -std=c99 -Wall -o VM.o VM.c

decodeur.o : decodeur.c
	gcc -g -std=c99 -Wall -o decodeur.o decodeur.c

VM_C.o : VM_C_versionCed.c
	gcc -g -Wall -o VM_C.o VM_C_versionCed.c

sure:
	gcc -g -Wall -o VM_base.o VM_base.c

clean:
	rm -rf *.o
	@	echo ".o supprime"

run:
	@ echo "### Lancement de la VM ###\n"
	./decodeur.o program.txt program_hexa.txt
	@ echo "### Decodage termine   ###\n"	
	./VM.o program_hexa.txt
	@ echo "### Programme termine  ###\n"

runsure:
	@ echo "Lancement de la VM qui fonctionne\n"
	./VM_base.o

tar:
	tar -cvf VM.tar *

man:
	gedit README.md
