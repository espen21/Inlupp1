#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"


answer_t convertchartostring (char *str)
{
  return (answer_t) toupper(*str);
}

bool is_shelf(char *str)
{
  if (isalpha(str[0])&& strlen(str)==3) //tar in \0
    { 
      for (int i = 1; i < 3; ++i)
        {
          if   (!isdigit (str[i]))
            {
              return false;
            }
          
        }
      return true;
    }
         
  else{
    return false;
  }
     
}


char *ask_question_shelf(char *question)
{
  return ask_question(question, is_shelf, (convert_func) strdup).s;
}

bool right_answer(char *c)
{
  if(toupper(*c) == 'N' || toupper(*c)  == 'J' || toupper(*c) == 'R')
    {
      return true; 
    }
  return false;
}
//" [J]a för att lägga till en vara\n [N]ej för att inte lägga till en vara \n [R]edigera för att ändra varan \n"

char ask_question_ware(char *question)
{
  //char *question = ();
  return ask_question(question, right_answer,(convert_func) convertchartostring).c;
}

bool not_empty(char *str)
{
  if (strlen(str) > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

answer_t ask_question (char *question, check_func check, convert_func convert)
{
  
  int buf_siz = 255;
  char buf[buf_siz];
  do
    {
      printf("%s\n", question);
      read_string(buf, buf_siz);
      //check att funktionen är i rätt format
      if (!check(buf))
        {
          printf("Felmeddelande '%s'\n", buf);
        }
    }
  while( !check(buf) ); //read_string "sparar i buf" och returnernar den.
  
  return convert(buf);
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i; // svaret som ett heltal
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func) strdup).s;
}




bool right_menu(char *c)
{
  if(toupper(*c) == 'A'|| toupper(*c) == 'L' || toupper(*c) == 'R' || toupper(*c) == 'T'|| toupper(*c) == 'G' || toupper(*c) == 'H')    {
  return true;
}     
return false;
}


char ask_question_char(char *str)
{
  //char *question = " [L]ägga till en vara\n [T]a bort en vara\n [R]edigera en vara\n Ån[g]ra senaste ändringen\n Lista [h]ela varukatalogen\n [A]vsluta\n" ;

    
  return ask_question(str, right_menu, (convert_func) convertchartostring).c ;//behöver jag getchar i detta fall?

} 




/*
int ask_question_int(char *question) 
{                                   
                                    
  int result = 0;
  int conversions = 0;
  do
    {
   printf("%s\n", question);
   conversions = scanf("%d", &result);
   int c;
   do
     {
       c = getchar();                //clear input buffer
     }
   while (c != '\n'&& c != EOF);
   putchar('\n');
    } 
  while(conversions < 1);
  return result;
  }
 */

/*int clear_input_buffer()
{-W
  int c;
  do
    {
      c = getchar();
    }
  while ( c != '\n' && c !=EOF);
  putchar('\n');
  }*/


int read_string(char *buf, int buf_siz)
{
int i = 0;
int c;
 for(i = 0; i <= buf_siz-1; ++i)
   {     
   c = getchar();
     
  if
    ( c == EOF || c == '\n')
    {
      *(buf + i) = '\0';
      //buf[i]= '\0';
      putchar('\n');
      return i;
      
    }
  else
    {
      *(buf + i) = c;
      //buf[i] = c;
    }
 
   }
 while (getchar() != '\n');
 putchar('\n');
 return i;
}

/*

char *ask_question_string(char *question, char *buf, int buf_siz)
{
  int chars_read=0;
  
  do
    {
      printf("%s\n", question);
      chars_read = read_string(buf, buf_siz);
    }
  while ( chars_read == 0 );
  return strdup(buf);
  
}

*/

bool is_number(char *str)
{
  int slength = strlen(str);
  int i = 0;
  
    if (  (isdigit(str[0])) || (str[0] == '-'))
       {
         for (i=1; i < slength; i +=1)
             {
               if (isdigit(str[i])== false)
                 {
                  return false;
                 }
               
             }
         return true;
        }
    
        
     else
     {
     return false;
     }
          
  }





