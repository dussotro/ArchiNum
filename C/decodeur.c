/*******************************************************************************************************************
* Titre : decodeur.c
* Auteurs : Cedric Delaunay et Romain Dussot
* Date : 19 janvier 2018
* Resume : traduit le pseudo_assembleur present dans "pseudo_assembleur.txt" en instructions lisibles par notre VM
********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 100
#define MAX_INSTRUCTIONS 100

struct Instruction
{
    char *mot;
    char *no_reg1; //numero de registre si y a besoin
    char *no_reg2;
    char *nombre; // le nombre o ou a ou n selon les cas (a pour braz et branz, n pour scall)
};

long int instr2longint(struct Instruction *instr) {
  char possibleCommande[19*5] = "STOP ADD SUB MULT DIV AND OR XOR SHL SHR SLE SEQ LOAD STORE JMP BRAZ BRANZ SCALL ";
  long int nombre = 0;

  char *func;
  char *token;
  func = NULL;

  for (token = strtok(possibleCommande, " "); token; token = strtok(NULL, " ")) {
    printf("lel\n");
    printf("%d\n", strcmp(token, instr->mot));
    if (strcmp(token, instr->mot) == 0) {
      func = token;
      break;
    }
  }
  printf("lel\n");
  printf("func=%s\n", func); // c'est la fonction



  printf("lel\n");
  return nombre;
}

struct Instruction chaine2instr(char *chaine) {
  printf("Decodage de %s", chaine);
  struct Instruction instruction;

  //on compte les espaces dans la commande pour différencier les cas
  int esp = 0;
  for (int i = 0; i<strlen(chaine); i++) {
    if (chaine[i] == ' ') {
      esp++;
    }
  }


  char * tokens[4];
  char * token;
  int i;

  i = 0;
  for (token = strtok(chaine, " "); token; token = strtok(NULL, " ")) {
    tokens[i] = token;
    printf("token=%s\n", tokens[i]);
    i++;
  }
  instruction.mot = tokens[i];
  switch (esp) {
    case 1:
      instruction.nombre = tokens[1];
      break;
    case 2:
      break;
    case 3:
      instruction.no_reg1 = tokens[1];
      instruction.nombre  = tokens[2];
      instruction.no_reg2 = tokens[3];
      break;
    default:
      perror("Erreur \n");
      break;
  }

  //Il faut compléter les noms des instrucions pour que toutes les instructions soient de la meme taille (avec store la plus grande)
  return instruction;
}

int main(int argc, char *argv[])
{

    FILE* fichier = fopen("program_simple.txt", "r");
    char *chaine  = malloc (sizeof(char) * TAILLE_MAX);
    long int program[MAX_INSTRUCTIONS]; //variable qui cntiendra l'ensemble des instructions
    //printf("avant boucle de lecture\n");
    if(fichier == NULL){
      printf("Erreur lors de la lecture du code assembleur");
    }
    else {
      //L'ouverture du fichier a reussi, on va pouvoir proceder a la traduction
      char *chaine= (char *)malloc(TAILLE_MAX * sizeof(char));
      while (fgets(chaine, TAILLE_MAX, fichier) != NULL) {
        printf("%s", chaine); //afficher les lignes de la fiche d'instruction
        struct Instruction instruction = chaine2instr(chaine);

        //test chaine2instr
        //printf("%s\n", instruction.no_reg1);


        long int int_instr             = instr2longint(&instruction);
      }
    }
    fclose(fichier);
    //printf("apres boucle de lecture\n");

    FILE* fichier = fopen("program_hexa.txt", "w");
    if(fichier == NULL){
      printf("Erreur lors de la lecture du fichier de programme");
    }
    else {
        //L'ouverture du fichier a reussi, on va pouvoir proceder a la traduction
        int i=0;
        while (program[i] != NULL) {
          fputs(program[i], fichier);
          i++;
      }
      fclose(fichier);
    return 0;
}
