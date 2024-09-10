#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
   char label[10],opcode[10],operand[10],symbol[10],code[10],mnemonic[3],objectcode[10];
   int locctr,start,length,address,sym_count=0,op_found;
   char sym_tab[50][50],sym_tab_val[50][10];
   FILE *fp1,*fp2,*fp3,*fp4;

   fp1=fopen("output.txt","r");
   fp2=fopen("objectcode.txt","w");
   fp3=fopen("optab.txt","r");
   fp4=fopen("symtab.txt","r");

   while(fscanf(fp4,"%s %X",symbol,&address)!= EOF)
   {
      strcpy(sym_tab[sym_count],symbol);
      sprintf(sym_tab_val[sym_count],"%X",address);
      sym_count++;
   }
   fscanf(fp1,"%X %s %s %s",&locctr,label,opcode,operand);

   if(strcpy(opcode,"START")==0)
   {
      fprintf(fp2,"%s\t%s\t%s\n",label,opcode,operand);
      fscanf(fp1,"%X %s %s %s",&locctr,label,opcode,operand);
   }
   while(strcmp(opcode,"END")!=0)
   {
      op_found=0;
      fscanf(fp3,"%s %s",code,mnemonic);
      rewind(fp3);

      while(fscanf(fp3,"%s %s",code,mnemonic)!= EOF)
      {
         if(strcmp(opcode,code)==0)
         {
            op_found=1;
            strcpy(objectcode,mnemonic);

            for(int i=0;i<sym_count;i++)
            {
               if(strcmp(operand,sym_tab[i])==0)
               {
                  strcat(objectcode,sym_tab_val[i]);
                  break;
               }
            }
            break;
         }
      }
      if(op_found)
      {
         fprintf(fp2,"%X\t%s\t%s\%s\t%s\n",locctr,label,opcode,operand,objectcode);
      }
      else if(strcmp(opcode,"WORD")==0)
      {
         sprintf(objectcode,"%06X",atoi(operand));
         fprintf(fp2,"%X\t%s\t%s\t%s\t%s\n",locctr,label,opcode,operand,objectcode);
      }
      else if(strcmp(opcode,"BYTE")==0)
      {
         int len=strlen(operand)-3;
         strncpy(objectcode,operand+2,len);
         objectcode[len]='\0';
         fprintf(fp2,"%X\t%s\t%s\t%s\t%s\n",locctr,label,opcode,operand,objectcode);
      }
      else if(strcmp(opcode,"RESW")==0 || strcmp(opcode,"RESB")==0)
      {
         fprintf(fp2,"%X\t%s\t%s\t%s\t%s\n",locctr,label,opcode,operand,objectcode);
      }


      fscanf(fp1,"%X %s %s %s",&locctr,label,opcode,operand);
   }
   fprintf(fp2,"%X\t%s\t%s\t%s\n",locctr,label,opcode,operand);



   fclose(fp1);
   fclose(fp2);
   fclose(fp3);
   fclose(fp4);
}

