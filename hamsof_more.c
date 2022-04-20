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
      puts(":::::::::::::::::::::::::::::");
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
            char buff[LINELEN];

			//getting target string from stdin
            fgets(str,LINELEN,stdin);
			int len_str = strlen(str);
            str[strlen(str)-1]='\0';
            //because this string is appending /n at the end due to pressing of enter 
            //tab so thats why we need to chnage that /n to /0           
            
            while(fgets(buff,LINELEN,fp))
            {
                actual_lines++;
		        if(strstr(buff,str))
                {
                  printf("\033[2A \033[2K \033[1G...........Skipping .....\n");
                  printf("%s",buff);
                  num_of_lines=2;
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
