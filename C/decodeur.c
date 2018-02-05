/*******************************************************************************************************************
* Titre : decodeur.c
* Auteurs : Cedric Delaunay et Romain Dussot
* Date : 19 janvier 2018
* Resume : traduit le pseudo_assembleur present dans "pseudo_assembleur.txt" en instructions lisibles par notre VM
********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX        100
#define MAX_INSTRUCTIONS  100
#define MAX_LABEL         50
#define LONGUEUR_MOT      5
#define TAILLE_TAB_LABELS 100

char ref_registre = 'R';
char char_label_deb = '<';
char char_label_fin = '<';
char char_instr = '>';

struct Instruction
{
    //Structure definissant une instruction
    char *mot;
    char *no_reg1; //numero de registre si y a besoin
    char *no_reg2;
    char *nombre; // le nombre o ou a ou n selon les cas (a pour braz et branz, n pour scall)
};

struct Label
{
  //Structure definissant les labels
  char *nom;
  int  no_instr;
};

unsigned long int instr2unsignedlongint(struct Instruction *instr) {
  //convertit une instruction en integer exploitable par le fichier principal
  char possibleCommande[] = "STOP-ADD-SUB-MULT-DIV-AND-OR-XOR-SHL-SHR-SLT-SLE-SEQ-LOAD-STORE-JMP-BRAZ-BRANZ-SCALL-";
  unsigned long int ret = 0;


  char *token;
  int no_instr = 0;

  for (token = strtok(possibleCommande, "-"); token; token = strtok(NULL, "-")) {
    //printf(" valeur du token :%set valeur du mot :%s.\n", token, instr->mot);
    if (strcmp(token, instr->mot)==0) {
      //printf("func=%s\n", func); // c'est la fonction
      break;
    }
    no_instr++;
  }
  ret = 0;
  ret = no_instr << 27;
  ret = ret & 0xFFFFFFFF;
  printf("ret : %lu \n", ret);

  int imm = 1;
  int o;
  int reg1;
  int reg2;
  int flag_label = 0; //creation d'une condition de passage pour le cas d'un JMP
  int lettre1; //utilisee dans jmp
  int lettre2; //utilisee dans jmp

  switch(no_instr){

    case 19:
      ret = 0;
      break;

    case 15:
      //il faudra comprendre pourquoi, mais nombre = registre et no_reg1 est le label
      if(instr->no_reg1[0]==ref_registre){
        imm = 0;
        for(int l=0; instr->no_reg1[l]; l++){
          instr->no_reg1[l]= instr->no_reg1[l+1];
        }
      }else{
        flag_label = 1;
        lettre1 = (int)instr->no_reg1[0];
        lettre2 = (int)instr->no_reg1[1];
      }

      for(int k=0; instr->nombre[k]; k++){
        instr->nombre[k]= instr->nombre[k+1];
      }

      reg1 = atoi(instr->nombre);
      ret = ret + (reg1);
      ret = ret + (imm << 26);

      if(flag_label==0){
        //cas de JMP vers une donnee stockee dans un registre
        o = atoi(instr->no_reg1);
        ret = ret + (o << 5);
      }else{
        //cas de JMP vers un label
        ret = ret + (lettre2 << 5);
        ret = ret + (lettre1 << 13);
      }
      break;

    case 16:
      for(int p=0; instr->no_reg1[p]; p++){
        instr->no_reg1[p]= instr->no_reg1[p+1];
      }
      reg1 = atoi(instr->no_reg1);

      if((int)(instr->nombre[0]) >= 48 && (int)(instr->nombre[0]) <= 57){
        o = atoi(instr->nombre);
        ret = ret + o ;
      } //conversion d'un char en integer
      else{
        for(int q = 0; instr->nombre[q]; q++){
          o = (int)(instr->nombre[q]);
          ret = ret + (o << (16 - (q+1)*8));
        }
      }

      ret = ret + (reg1 << 22);
      break;

    case 17:
      for(int k=0; instr->no_reg1[k]; k++){
        instr->no_reg1[k]= instr->no_reg1[k+1];
      }
      reg1 = atoi(instr->no_reg1);
      if((int)(instr->nombre[0]) >= 48 && (int)(instr->nombre[0]) <= 57){
        o = atoi(instr->nombre);
        ret = ret + o ;
      } //conversion d'un char en integer
      else{
        for(int q = 0; instr->nombre[q]; q++){
          o = (int)(instr->nombre[q]);
          ret = ret + (o << (16 - (q+1)*8));
        }
      }

      ret = ret + (reg1 << 22);
      break;

    case 18:
      o = atoi(instr->nombre);
      ret = ret + o;
      break;

    default:
      for(int k=0; instr->no_reg1[k]; k++){
        instr->no_reg1[k]= instr->no_reg1[k+1];
      }
      for(int k=0; instr->no_reg2[k]; k++){
        instr->no_reg2[k]= instr->no_reg2[k+1];
      }
      reg1 = atoi(instr->no_reg1);
      reg2 = atoi(instr->no_reg2);

      if(instr->nombre[0]==ref_registre){
        imm = 0;
        for(int l=0; instr->nombre[l]; l++){
          instr->nombre[l]= instr->nombre[l+1];
        }
      }
      o = atoi(instr->nombre);
      ret = ret + (reg1 << 22);
      ret = ret + (imm  << 21);
      ret = ret + (o << 5);
      ret = ret + reg2;
  }

  return ret;
}
/*
struct Label chaine2label(char *chaine, int i) {
  //Methode de conversion d'une chaine de caractere en un label
  struct Label label;
  int cpt=1;
  while (chaine[cpt] != char_label_fin) {
    label.nom[cpt] = chaine[cpt];
    cpt++;
  }
  label.no_instr = i;
  return label;
}
*/

struct Instruction chaine2instr(char *chaine) {
  //Methode de conversion d'une chaine de caracteres en une instruction
  printf("Decodage de %s", chaine);

    struct Instruction instruction;
    instruction.mot = (char *)malloc(LONGUEUR_MOT * sizeof(char));

    //on compte les espaces dans la commande pour différencier les cas
    int esp = 0;
    for (int i = 1; i<strlen(chaine); i++) {
      if (chaine[i] == ' ') {
        esp++;
      }
    }


    char * tokens[4];
    char * token;
    int i;

    i = 0;
    for (token = strtok(chaine, "> "); token; token = strtok(NULL, "> ")) {
      tokens[i] = token;
      printf("token=%s\n", tokens[i]);
      i++;
    }
    instruction.mot = tokens[0];
    switch (esp) {
      case 0:
        break;
      case 1:
        instruction.nombre = tokens[1];
        break;
      case 2:
        instruction.no_reg1 = tokens[1];
        instruction.nombre  = tokens[2];
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

  return instruction;
}

unsigned long int label2unsignedlongint (struct Label *label) {
  unsigned long int nombre = 0;

  nombre += ((int)label->nom[0] << 24);
  nombre += ((int)label->nom[1] << 16);
  nombre += ((int)label->nom[2] << 8);
  nombre += (int)label->nom[3];
  return nombre;
}


int main(int argc, char *argv[])
{

    FILE* fichierL = fopen(argv[1], "r");
    struct Label *tableauL = (struct Label*) malloc(TAILLE_TAB_LABELS * sizeof(struct Label));

    if(fichierL == NULL){
      printf("Erreur lors de la lecture du code assembleur");
    }
    else {
      int cptL = 0;
      int ligneL = 0;
      char* chainelab = (char *)malloc(TAILLE_MAX * sizeof(char));
      while (fgets(chainelab, TAILLE_MAX, fichierL) != NULL) {
        printf("salut salut %c\n", chainelab[0]);
        if(chainelab[0] == char_label_deb){
          printf("j'ai trouvé un label \n");
          (tableauL[ligneL]).no_instr = cptL+1;
          (tableauL[ligneL]).nom = chainelab;
          ligneL++;
        }
        cptL++;
      }
      fclose(fichierL);
    }

    FILE* fichier = fopen(argv[1], "r");
    //char *chaine  = malloc (sizeof(char) * TAILLE_MAX);
    unsigned int *program = (unsigned int *) malloc(MAX_INSTRUCTIONS * sizeof(unsigned int)); //variable qui contiendra l'ensemble des instructions
    unsigned long int int_instr = 0;
    int i = 0;

    if(fichier == NULL){
      printf("Erreur lors de la lecture du code assembleur");
    }
    else {
      //L'ouverture du fichier a reussi, on va pouvoir proceder a la traduction
      char *chaine= (char *)malloc(TAILLE_MAX * sizeof(char));
      while (fgets(chaine, TAILLE_MAX, fichier) != NULL) {
        printf("%s", chaine); //afficher les lignes de la fiche d'instruction
        if (chaine[0] == char_instr) {
          struct Instruction instruction = chaine2instr(chaine);
          int_instr                      = instr2unsignedlongint(&instruction);
        }

        printf("int_instr : %lu\n", int_instr);
        program[i] = int_instr;
        i++;
      }
    }
    fclose(fichier);


    FILE* fichierH = fopen(argv[2], "w");
    if(fichierH == NULL){
      printf("Erreur lors de la lecture du fichier de programme");
    }else{
      //L'ouverture du fichier a reussi, on va pouvoir proceder a la traduction
      int m=0;
      while (m < i) {
        fprintf(fichierH, "%08X\n", program[m]);
        m++;
      }
    fclose(fichierH);
  }

    return 0;
}
