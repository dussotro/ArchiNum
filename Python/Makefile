default : run

VM : VM.py
	python3 -i VM.py program_hexa.txt

decodeur : decodeur
	python3 -i decodeur.py syracuse.txt program_hexa.txt

clean:
	rm -rf *.pyc
	@	echo ".pyc supprime"

run:
	@ echo "### Lancement de la VM ###\n"
	python3 decodeur.py syracuse.txt program_hexa.txt
	@ echo "### Decodage termine   ###\n"
	python3 VM.py program_hexa.txt
	@ echo "### Programme termine  ###\n"


tar:
	tar -cvf VM.tar *

man:
	gedit README.md
