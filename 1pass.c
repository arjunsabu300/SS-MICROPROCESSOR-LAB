#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fp1,*fp2,*fp3,*fp4,*fp5;
char label[20],operand[20],opcode[20],t1[20],op[20],add[20],hex[20],a[10],obj[20];
int start,locctr,i,j,flag,size,k=0,x,s=0,length;

struct sym
{
  char sym[20];
  int co;
} sy[20];

struct optab
{
  char code[20],mem[20];
} op1[20];

void readoptab()
{
  while(1)
  {
    s++;
    fscanf(fp2, "%s %s", op1[s].code, op1[s].mem);
    if (getc(fp2) == EOF)
      break;
  }
}

int main()
{
  fp1 = fopen("pass1co.txt", "r");
  fp2 = fopen("pass1tab.txt", "r");
  fp4 = fopen("Obj11.txt", "w+");
  fp5 = fopen("objtxt.txt", "w");

  if (!fp1 || !fp2 || !fp4 || !fp5) {
    printf("Error opening files.\n");
    return 1;
  }

  readoptab();

  fscanf(fp1, "%s %s %s", label, opcode, operand);
  if (strcmp(opcode, "START") == 0)
  {
    start = strtol(operand, NULL, 16);
    locctr = (0x1) * start;
    fprintf(fp5, "**\t%s\t%s\t%s\t**\n", label, opcode, operand);
    fscanf(fp1, "%s %s %s", label, opcode, operand);
  }
  else
  {
    locctr = 0;
  }

  while (strcmp(opcode, "END") != 0)
  {
    fprintf(fp5, "%x\t", locctr);
    if (strcmp(label, "**") == 0)
    {
      for (i = 0; i <= k; i++)
      {
        if (strcmp(sy[i].sym, operand) == 0)
        {
          x = sy[i].co;
        }
      }
      for (i = 0; i <= s; i++)
      {
        if (strcmp(op1[i].code, opcode) == 0)
        {
          locctr += 0x3;
          size += 3;
          fprintf(fp5, "%s\t%s\t%s\t%s%x\n", label, opcode, operand, op1[i].mem, x);
          break;
        }
      }
    }
    else if (strcmp(opcode, "WORD") == 0)
    {
      k++;
      sy[k].co = locctr;
      locctr += 0x3;
      size += 3;
      sprintf(a, "%x", atoi(operand));
      strcpy(sy[k].sym, label);
      fprintf(fp5, "%s\t%s\t%s\t00000%s\n", label, opcode, operand, a);
    }
    else if (strcmp(opcode, "RESW") == 0)
    {
      k++;
      sy[k].co = locctr;
      locctr += ((0x3) * (atoi(operand)));
      strcpy(sy[k].sym, label);
      fprintf(fp5, "%s\t%s\t%s\t**\n", label, opcode, operand);
    }
    else if (strcmp(opcode, "RESB") == 0)
    {
      k++;
      sy[k].co = locctr;
      locctr += (0x1) * atoi(operand);
      strcpy(sy[k].sym, label);
      fprintf(fp5, "%s\t%s\t%s\t**\n", label, opcode, operand);
    }
    else if (strcmp(opcode, "BYTE") == 0)
    {
      int len = strlen(operand);
      if (operand[0] == 'C' || operand[0] == 'c')
      {
        len = len - 3;
      }
      else
      {
        len = (len - 3) / 2;
      }
      k++;
      sy[k].co = locctr;
      locctr += (0x1) * len;
      size += len;
      strcpy(sy[k].sym, label);
      j = 0;
      for (i = 2; i < strlen(operand) - 1; i++)
      {
        sprintf(hex + j, "%02x", operand[i]);
        j += 2;
      }
      fprintf(fp5, "%s\t%s\t%s\t%s\n", label, opcode, operand, hex);
    }
    fscanf(fp1, "%s %s %s", label, opcode, operand);
  }

  fprintf(fp5, "%x\t%s\t%s\t%s\t**\n", locctr, label, opcode, operand);
  length = locctr - start;

  fclose(fp1);
  fclose(fp2);
  fclose(fp5);

  fp5 = fopen("objtxt.txt", "r");
  fscanf(fp5, "%s %s %s %s %s", add, label, opcode, operand, obj);
  if (strcmp(opcode, "START") == 0)
  {
    fprintf(fp4, "H^%s  ^00%x^0000%x\n", label, start, length);
    fscanf(fp5, "%s %s %s %s %s", add, label, opcode, operand, obj);
    fprintf(fp4, "T^00%s^%x^%s", add, size, obj);
    fscanf(fp5, "%s %s %s %s %s", add, label, opcode, operand, obj);
  }
  else
  {
    locctr = 0;
  }
  while (strcmp(opcode, "END") != 0)
  {
    if (strcmp(obj, "**") != 0)
    {
      fprintf(fp4, "^%s", obj);
    }
    fscanf(fp5, "%s %s %s %s %s", add, label, opcode, operand, obj);
  }

  fprintf(fp4, "\nE^00%x\n", start);

  fclose(fp5);
  fclose(fp4);

  return 0;
}