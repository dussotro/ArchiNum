/*********************************************************
* Exemple de VM avec exemples de manipulations de registre
**********************************************************/

#include <stdio.h>

#define NUM_REGS 32
unsigned regs[ NUM_REGS ];

unsigned program[] = { 0x01000064, 0x010100C8, 0x02020001, 0x03030200,
                       0x04040003, 0x05050300, 0x06060001, 0x0B070102,
                       0x0C080102, 0x0C090301, 0x0E01020A, 0x00000000 };

/* program counter */
int pc = 0;

/* fetch the next word from the program */
int fetch()
{
  return program[ pc++ ];
}

/* instruction fields */
int instrNum = 0;
int reg1     = 0;
int reg2     = 0;
int reg3     = 0;
int imm      = 0;

/* decode a word */
void decode( int instr )
{
  instrNum = (instr & 0xFF000000) >> 24;
  reg1     = (instr & 0xFF0000 ) >>  16;
  reg2     = (instr & 0xFF00  ) >>  8;
  reg3     = (instr & 0xFF   );
  imm      = (instr & 0xFFFF  );
}

/* the VM runs until this flag becomes 0 */
int running = 1;

/* evaluate the last decoded instruction */
void eval()
{
  switch( instrNum )
  {
    case 0:
      /* halt */
      printf( "halt\n" );
      running = 0;
      break;
    case 1:
      /* loadi */
      printf( "loadi r%d #%d\n", reg1, imm );
      regs[ reg1 ] = imm;
      break;
    case 2:
      /* add */
      printf( "add r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] + regs[ reg3 ];
      break;
    case 3:
      /* sub */
      printf( "sub r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] - regs[ reg3 ];
      break;
    case 4:
      /* mult */
      printf( "mult r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] * regs[ reg3 ];
      break;
    case 5:
      /* div */
      printf( "div r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] / regs[ reg3 ];
      break;
    case 6:
      /* and */
      printf( "and r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] & regs[ reg3 ]; //faux, return true ou false et non le &
      break;
    case 7:
      /* or */
      break;
    case 8:
      /* xor */
      break;
    case 9:
      /* shl */
      break;
    case 10:
      /* shr */
      break;
    case 11:
      /* slt */
      printf( "slt r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] < regs[ reg3 ];
      break;
    case 12:
      /* sle */
      printf( "slt r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] <= regs[ reg3 ];
      break;
    case 13:
      /* seq */
      printf( "slt r%d r%d r%d\n", reg1, reg2, reg3 );
      regs[ reg1 ] = regs[ reg2 ] == regs[ reg3 ];
      break;
    case 14:
      /* store */
      printf( "store r%d r%d r%d\n", reg1, reg2, reg3);
      regs [reg3 + reg2] = regs[ reg1 ];


  }
}

/* display all registers as 16-digit hexadecimal words */
void showRegs()
{
  int i;
  int cpt = 0;
  printf( "regs = \n" );
  for( i=0; i<NUM_REGS; i++ ){
    printf( "%016X ", regs[ i ] );
    cpt++;
    if(cpt>=4)
    {
      printf(" \n");
      cpt = 0;
    }
    }
  printf( "\n" );
}

void run()
{
  while( running )
  {
    showRegs();
    int instr = fetch();
    decode( instr );
    eval();
  }
  showRegs();
}

int main( int argc, const char * argv[] )
{
  run();
  return 0;
}
