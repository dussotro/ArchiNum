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
#define LONGUEUR_MOT 5
#define TAILLE_PC 114

struct Instruction
{
    char *mot;
    char *no_reg1; //numero de registre si y a besoin
    char *no_reg2;
    char *nombre; // le nombre o ou a ou n selon les cas (a pour braz et branz, n pour scall)
};

long int instr2longint(struct Instruction *instr) {
  //convertit une instruction en integer exploitable par le fichier principal
  char possibleCommande[TAILLE_PC] = "STOP -ADD  -SUB  -MULT -DIV  -AND  -OR   -XOR  -SHL  -SHR  -SLE  -SEQ  -LOAD -STORE-JMP  -BRAZ -BRANZ-SCALL-";
  long int ret = 0;

  char *func;
  char *token;
  int no_instr = 0;
  func = NULL;

  for (token = strtok(possibleCommande, "-"); token; token = strtok(NULL, "-")) {
    //printf(" valeur du token :%set valeur du mot :%s.\n", token, instr->mot);
    if (strcmp(token, instr->mot)==0) {
      func = token;
      printf("func=%s\n", func); // c'est la fonction
      break;
    }
    no_instr++;
  }

  ret = no_instr << 27;

  int imm = 0;
  int o;
  int reg1;

  switch(no_instr){

    case 15:
      if(instr->nombre[0]=="R"){
        imm = 1;
        for(int l=0; instr->nombre[l]; l++){
          instr->nombre[l]= instr->nombre[l+1];
        }
      }
      for(int k=0; instr->no_reg1[k]; k++){
        instr->no_reg1[k]= instr->no_reg1[k+1];
      }
      reg1 = atoi(instr->no_reg1);
      o = atoi(instr->nombre);
      ret = ret + (o << 5);
      ret = ret + (imm << 26);
      ret = ret + (reg1);
      break;

    case 16:
      for(int k=0; instr->no_reg1[k]; k++){
        instr->no_reg1[k]= instr->no_reg1[k+1];
      }
      reg1 = atoi(instr->no_reg1);
      o = atoi(instr->nombre);

      ret = ret + (reg1 << 26);
      ret = ret + o ;
      break;

    case 17:
      for(int k=0; instr->no_reg1[k]; k++){
        instr->no_reg1[k]= instr->no_reg1[k+1];
      }
      reg1 = atoi(instr->no_reg1);
      o = atoi(instr->nombre);

      ret = ret + (reg1 << 26);
      ret = ret + o ;
      break;

    case 18:
      o = atoi(instr->nombre);
      ret = ret + o;
      break;

    default:
      for(int k=0; instr->no_reg1[k]; k++){
        instr->no_reg1[k]= instr->no_reg1[k+1];
      }
      reg1 = atoi(instr->no_reg1);
      if(instr->nombre[0]=="R"){
        imm = 1;
        for(int l=0; instr->nombre[l]; l++){
          instr->nombre[l]= instr->nombre[l+1];
        }
      }
      o = atoi(instr->nombre);
      ret = ret + (imm << 26);
      ret = ret + (o << 5);
      ret = ret + reg1;
  }

  return ret;
}

struct Instruction chaine2instr(char *chaine) {
  printf("Decodage de %s", chaine);
  struct Instruction instruction;
  instruction.mot = (char *)malloc(LONGUEUR_MOT * sizeof(char));

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
  instruction.mot = tokens[0];
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

  //Il faut compléter les noms des instrucions pour que toutes les instructions soient de la meme taille (avec store la plus grande) --> FAIT

  if(strlen(instruction.mot)==2){
    strcat(instruction.mot, "   ");
  }else if(strlen(instruction.mot)==3){
    strcat(instruction.mot, "  ");
  } else if(strlen(instruction.mot)==4){
    strcat(instruction.mot, " ");
  }

  return instruction;
}

int main(int argc, char *argv[])
{

    FILE* fichier = fopen("program.txt", "r");
    //char *chaine  = malloc (sizeof(char) * TAILLE_MAX);
    long int program[MAX_INSTRUCTIONS]; //variable qui contiendra l'ensemble des instructions
    int i = 0;
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
        //printf("nombre de l'instruction : %s\n", instruction.nombre);


        long int int_instr = instr2longint(&instruction);

        program[i] = int_instr;
        i++;
      }
    }
    fclose(fichier);
    //printf("apres boucle de lecture\n");
    //Ca fonctionne au moins jusque la, il faut rajouter la mise de l'instruction traduite dans le tableau

    /*
    char *instruction = (char *)malloc(TAILLE_MAX * sizeof(char));
    *instruction = "add  ";
    int a = decodageInstr(&instruction);
    printf("nro_Instr ressorti : %d\n", a);
    */
    for(int j = 0; j<i ; j++){
        printf("commande %d : %08x\n", j, program[j]);
    }

    FILE* fichierH = fopen("program_hexa.txt", "w");
    if(fichierH == NULL){
      printf("Erreur lors de la lecture du fichier de programme");
    }else{
      //L'ouverture du fichier a reussi, on va pouvoir proceder a la traduction
      int i=0;
      while (program[i] != NULL) {
        fputs(program[i], fichierH);
        i++;
      }
    fclose(fichierH);
  }

    return 0;
}
