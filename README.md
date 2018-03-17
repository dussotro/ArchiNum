ArchiNum
========

Creation d'une ISS en python

Made by Cédric DELAUNAY and Romain Dussot

Fonctionnement
==============

Dans le dossier Python, lancer le bash script.sh suivi de la commande à effectuer (les privilèges super-user sont requis)

Exemple :
```
bash
sudo bash script.sh syracuse
```

Commandes possible : "syracuse" et "matrice"

Calcul matriciel
================

Le calcul matriciel 3x3 demande de rentrer les 18 composantes une par une, ligne par ligne.
Ensuite, le résultat de chaque cellule est affiché via un appel système 1 du registre 23. En remontant la console, vous allez donc voir apparaître une par une les composantes de la matrice résultat.

Cache
=====

Le cache a été implémenté, mais ses performances non mesurées (problème à l'exécution non résolu à ce jour, la partie qui posait problème a été retirée de la VM)

Le dossier C
============

Vous pourrez remarquer un dossier appelé C. L'idée était initialement de développer notre ISS en C, mais suite à quelques problèmes épineux nous avons décidé de passer sous Python. Le contenu de ce dossier a été laissé à l'abandon.
