#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LINELEN 512
#define PAGELEN 25

void do_more(FILE *);
int get_input(FILE *);

int main(int argc, char* argv[])
{
   if(argc==1)
   {
      do_more(stdin);
   }

   int i = 0;
   FILE * fp;
   

   while(++i < argc){
      fp = fopen(argv[i] , "r");
      if (fp == NULL){
         perror("Enter a valid file name\n");
         exit (1);
      }
      do_more(fp);
      fclose(fp); 
   }
   return 0;
}
void do_more(FILE *fp){

   char buffer[LINELEN];
   int num_of_lines=0;
   int rv;
   FILE *fp_tty = fopen("/dev//tty", "r");
   int actual_lines=0;
   int total_lines=0;
   float percentage;

   //first pass to check the total_lines
   while(fgets(buffer,LINELEN,fp)){
		total_lines++;
	}
	fseek(fp,0,SEEK_SET);


   while(fgets(buffer,LINELEN,fp)){

      fputs(buffer,stdout);
      num_of_lines++;
      actual_lines++;


      if (num_of_lines == PAGELEN)
      {

         percentage = (actual_lines/(float)total_lines)*100;
         printf("\033[7m --more--(%.1f)%% \033[m",percentage);

         rv = get_input(fp_tty);		
         if (rv == 0)
         {
            printf("\033[1A \033[2K \033[1G");
            break;
         }
         else if (rv == 1)
         {
            printf("\033[1A \033[2K \033[1G");
            num_of_lines -= PAGELEN;
         }
         else if (rv == 2)
         {
            printf("\033[1A \033[2K \033[1G");
	         num_of_lines --;
         }
         else if(rv == 4)
         {
            char str[LINELEN];
				char temp[20];
            char buff[LINELEN]; //whole line

				fgets(str,LINELEN,stdin);
				int len_str = strlen(str);
            int pre = ftell(fp); // will be used if we dont find the string
            int i=0;


            // when we press enter it will automatically add '/0' at the end
            // string so will compare one less character

            while(fgets(buff,LINELEN,fp))
            {
               actual_lines++;
		         if(strncmp(buff,str,len_str-1)==0)
               {
                  //printf("search : %sactual : %s",str,buff)
                  printf("\033[2A \033[2K \033[1G");
                  printf("%s",buff);
                  num_of_lines=0;
                  break;
               }
	         }
         }
      }
      else if (rv == 3)
         break; 
   }
}

int get_input(FILE* fp_tty)
{
   int c;		

   c = getc(fp_tty);

   if(c == 'q')
	   return 0;
   if ( c == ' ' )			
	   return 1;
   if ( c == '\n' )	
	   return 2;
   if ( c == '/' )	
	   return 4;   	
   return 3;
}
