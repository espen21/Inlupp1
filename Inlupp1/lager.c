#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include <time.h>
#include "tree.h"
#include "list.h"


typedef struct shelf shelf_t;
typedef struct ware ware_t;

typedef struct action action_t;

struct shelf
 {
  char *shelfname;
  int amount;

};

struct ware
{
  char *name;
  char *desc;
  int price;
  list_t *list;
};

struct action
{
  int type;
  ware_t *merch;
  ware_t copy;
};

void print_ware(char *name, char *desc, int price, char *shelf_name, int amount)
{
  printf(" Name:%s\n Desc: %s\n Price: %d\n Shelf: %s\n Amount: %d\n \n",name,desc,price,shelf_name,amount);
}

L make_shelf(K shelfname, int amount)
{
  L shelf = calloc(1,sizeof(L));
  *shelf = (shelf_t){
    .shelfname = shelfname,
    .amount = amount
  };
  return shelf;
}

ware_t *make_ware( K name,char *desc, int price, list_t *list)
{
  ware_t *ware = calloc(1,sizeof(ware_t));
    *ware = (ware_t){
    .name= name,
    .desc = desc,
    .price= price,
    .list = list
  };

  return ware;
}

struct foo
{
  char *shelfname;
  bool found;
};

typedef struct foo foo_t;

void list_has_shelf(L elem, foo_t *foo)
{
  if(strcmp(foo->shelfname, elem->shelfname) == 0)
    {
      
      foo->found = true;
    }
  
}

bool shelves_free(tree_t *tree, char *shelfname)
{
  ware_t **elements =(ware_t**) tree_elements(tree);

  for (int i = 0; i < tree_size(tree); i++)
    {
      ware_t *current_element = elements[i];
      list_t *current_list = current_element->list;
      struct foo foo = {.shelfname = shelfname};
      list_apply(current_list,(list_action2)list_has_shelf,&foo);
      if (foo.found)
	{
	  puts("Upptagen plats \n");
	  return true;
	}
    }
  return false;
}


void print_tree(tree_t *tree)
{
  char **tree_names = tree_keys(tree);
  int max = 0;
  for(int i = 0; i < tree_size(tree); i++)
    {
      
      max = max + 20;
      printf("%d: %s\n",i+1,tree_names[i]);
      if(i>=max)
	{
	  char *answer = ask_question_string("Vill du blädda till nästa sida [J]a eller [N]ej?\n");
	  if(toupper(*answer) == 'J')
	    {
	      continue;
	    }
	  else
	    {
	      break;
	    }
	}
    }


}
     
/*
void print_tree(tree_t *tree)
{
  char **tree_names = tree_keys(tree);
  for(int i = 0; i < tree_size(tree); i++)
    {
      printf("%d: %s\n",i+1,tree_names[i]);
    }

}
*/


ware_t *print_ware_index(tree_t *tree)
{
  int index = ask_question_int("Vilken vara?\n");
   
  while(index > (tree_size(tree)) || index <= 0)
    {
      puts("Skirv ett giltigt index\n");
      index = ask_question_int("Vilken vara \n");
      
    }
  ware_t **elements = (ware_t **)tree_elements(tree);
  ware_t *elem = elements[index-1];
  list_t *wares_list = elem->list;
  int _amount = 0 ;
  printf("Namn: %s\n",elem->name);
  printf("Desc: %s\n",elem->desc);
  printf("Pris: %d\n",elem->price);
  printf("Shelf:");
  for (int i =0; i < list_length(wares_list); i++)
    {
      shelf_t **shelf1 = list_get(wares_list, i);
      char *namnshelf = (*shelf1)->shelfname;
      
      _amount = _amount + (*shelf1)->amount;//öka antalet
      printf(" %s",namnshelf);		  
		}
  
  printf("\nAmount: %d\n",_amount);
  
  return elem;
}

list_t *list_copy(list_t *old_list)
{
  
  list_t *copy = list_new();
  
  for(int index = 0; index < (list_length(old_list));  index++)
        {
	  shelf_t *copy_of_shelf = calloc(1,sizeof(shelf_t));// kan vara L istället för shelt_t *
	  shelf_t *list_get_result = *list_get(old_list,index);
	  *copy_of_shelf = (struct shelf){ .shelfname = list_get_result->shelfname, .amount = list_get_result->amount};
          //list_insert(copy,index,copy_of_shelf);
	  list_append(copy,copy_of_shelf);
	}
  return copy;
}

void edit_item(tree_t *tree, action_t *undo)
{
  
  print_tree(tree);
  char *line = "--------------------------------------------------------\n";
  ware_t *ware = print_ware_index(tree);
  char *answer = ask_question_string("\n[B]eskrining\n[P]ris\n[L]agerhylla\nAn[t]al\n Välj rad eller [a]vbryt:_\n");
  
  undo->copy = *ware;
  list_t *copied_list = list_copy(ware->list);
  undo->copy.list = copied_list;
 
  do{ 
    if(toupper(*answer) == 'B')  
      {
	printf("Nuvarande beskrivning %s\n", ware->desc);
	printf("%s",line);
	puts("Ny beskrivning:_\n");
	char *nydesc = ask_question_string("Skriv in ny beskrivning\n");
	ware->desc = nydesc;
	printf("Ny beskrivning: %s\n",ware->desc);
	undo->type = 3;
	undo->merch = ware;
	ware = undo->merch;
	break;
      }
    if(toupper(*answer) == 'P') 
      {
	printf("Nuvarande pris %d\n", ware->price);
	printf("%s",line);
	puts("Nytt pris:_\n");
	int nyttpris = ask_question_int("Skriv in nytt pris\n");
	ware->price = nyttpris;
	printf("Nytt pris: %d\n",ware->price);
	undo->type = 3;
	undo->merch = ware;
	break;
      }
      if(toupper(*answer) == 'L') 
      {

	puts("Shelf:");
	 for (int i =0; i < list_length(ware->list); i++)
	   {
	     shelf_t **shelf1 = list_get(ware->list, i);
	     char *namnshelf = (*shelf1)->shelfname;
	     printf(" %d:%s",i+1, namnshelf);		  
	   }

	 int index_shelf = ask_question_int("\n Skriv index på vilken hylla du vill ändra på\n");
	 index_shelf = index_shelf - 1;
	 L *shelf_to_change = list_get(ware->list,index_shelf);
	 char *new_shelf = ask_question_shelf("Ny shelf_");
	 (*shelf_to_change)->shelfname = new_shelf;
	 undo->type = 3;
	 undo->merch = ware;
	 break;	 
      }
    if(toupper(*answer) == 'T')  
      {
	puts("Shelf:");
	for (int i =0; i < list_length(ware->list); i++)
	  {
	    shelf_t **shelf1 = list_get(ware->list, i);
	    char *namnshelf = (*shelf1)->shelfname;
	    int antal = (*shelf1)->amount;
	    printf("%d:%s-%d\n",i+1, namnshelf,antal);		  
	  }
	
	int index_shelf = ask_question_int("\n Skriv index på vilken hylla du vill ändra antal på\n");
	L *shelf_to_change = list_get(ware->list,index_shelf);
	int nytt_antal = ask_question_int("Skriv in det nya antalet\n");
	(*shelf_to_change)->amount = nytt_antal;
	undo->type = 3;
	undo->merch = ware;
	break;
      }
    
    if(toupper(*answer) == 'A') 
      {
	break;
      }
    
  }
    while(true);
  
}

ware_t *input_ware(char *name,char *desc, int price, char *shelf_name, int amount)
{
  L shelf = make_shelf(shelf_name, amount);
  list_t *list1 = list_new();
  list_prepend(list1, shelf);
  return make_ware(name, desc,price, list1);
     
}


void test_tree(tree_t *tree)
{
  for(int i = 0; i <= 40; i++)
    {
      char *name = "fisk";
      char *desc = "fisk";
      int price =  random() % 10 + '0';
      char *shelfname =  "a21";
      int amount =  random() % 10 + '0';
      ware_t *ware = ware;
      ware =  input_ware(name,desc,price,shelfname,amount);
      tree_insert(tree,ware->name,ware);
      
    }
}

void add_item(tree_t *tree, action_t *undo )
{
  char *name = ask_question_string("Mata in namnet: \n");
   if(tree_has_key(tree,name) == false)
     {
       char *desc = ask_question_string("Skriv beskrivning: \n");
       int price = ask_question_int("Pris: \n");
       char *shelfname = ask_question_shelf("Hyllan: \n");
       int amount = ask_question_int("Amount: \n");
       ware_t *ware;
       print_ware(name,desc,price,shelfname,amount);
       char c = ask_question_ware(" [J]a för att lägga till en vara\n [N]ej för att inte lägga till en vara \n ");
       
       if (c == 'J')
	 {
	   if(shelves_free(tree,shelfname) == false)
	     {
	       ware =  input_ware(name,desc,price,shelfname,amount);
	       tree_insert(tree,ware->name,ware);
	       undo->type = 1;
	       undo->merch = ware;
	       return;
	     }

	   else
	     {
	       puts("Hylla upptagen");
	       shelfname = ask_question_shelf("Skriv in en annan hylla\n");
	       if(shelves_free(tree,shelfname) == false)
		 {
		   ware_t *ware = input_ware(name,desc,price,shelfname,amount);
		   tree_insert(tree,ware->name,ware);
		   return;
		 }
	       else
		 {
		   puts("U suck");
		   return;
		 }
	     }
	 }
       else if( c == 'N')
	 {
	   return;
	 }
     
     }
   printf(" %s finns redan, Vill du lägga till en likadan vara fast på ny hylla eller vill du redigera en vara ?\n",name);
   char c = ask_question_ware("\n [J]a för att lägga till en vara\n [N]ej för att inte lägga till en vara \n [R]edigera för att ändra varan \n");
   if (tree_has_key(tree, name)==true && c == 'J')
     {
       
       
       char *shelfname = ask_question_shelf("Hyllan: \n");
       int amount = ask_question_int("Amount: \n");
       printf("%s har lagts till på lagerhylla: %s och antal: %d\n",name,shelfname,amount);
       ware_t *existing_ware = tree_get(tree, name);//returnerar ware_t * med angivet name i tree.
       list_t *wares_list = existing_ware->list;
       //Kolla om samma ware redan använder samma hylla, om sant, ök på
       for (int i =0; i < list_length(wares_list); i++)
	 {
	   shelf_t **shelf1 = list_get(wares_list, i);
	   if( strcmp((*shelf1)->shelfname, shelfname) == 0)
	     {
	       int old_amount =  (*shelf1)->amount;//öka antalet
	       (*shelf1)->amount = old_amount + amount;
	       puts("Found shelf with same name, increased amount");
	       undo->type = 1;
	       //  undo->merch = ware;
	       return;
                }              
	   //om de har samma hylla öka på antalet
	 }          
       puts("could not find shelf with same name");
       L shelf = make_shelf(shelfname, amount); //{.shelfname = shelfname, .amount = amount}; 
       
       list_append(existing_ware->list, shelf);
       puts("added a new shelf");                    
       undo->type = 1;
       //undo->merch = ware;
       return;
     }
   

  
  if ( c == 'N')
    {
      return;
    }
  else if( c == 'R')
    {
      edit_item(tree,undo);
      return;
    }
}


void undo_action(action_t *undo)
{
  int action_type = undo->type;

  if(action_type == 0)
    {
      undo->type = 0;
    }
  else if(action_type == 1)
    {
      
      undo->type = 0;
    }
  else if(action_type == 2)
    {
      undo->type = 0;
    }
  else if(action_type == 3) //edit
    {
      
      undo->merch->desc =  undo->copy.desc;
      undo->merch->price = undo->copy.price;
      undo->merch->list = undo->copy.list;
      
     
  
      undo->type = 0;
    }
}

char *welcome = ("\nVälkommen till lagerhantering 1.0\n ================================= \n [L]ägga till en vara \n [T]a bort en vara \n [R]edigera en vara \n Ån[g]ra senaste ändringen \n Lista [h]ela varukatalogen \n [A]vsluta \n Vad vill du göra idag? _");

void main_loop (tree_t *tree)
{
  action_t *undo = calloc(1,sizeof(action_t));
  undo->type = 0;
  do
    {
    char c = ask_question_char(welcome);

    if(c == 'L')
      {
	add_item (tree,undo);
      }
    else if( c == 'T')
      {
	int d = tree_size(tree);
	printf("%d \n", d);
      	// void remove_item(void); fixme
      }
    else if( c == 'R')
      {
	edit_item(tree,undo);
      }
    else if(c == 'G')
      {
	undo_action(undo);
      }
    else if( c == 'H')
      {
	//	int i = 1;
	print_tree(tree);
	print_ware_index(tree);
      }
    else if( c == 'A')
      {
	char *answer;
	answer= ask_question_string("Vill du verkligen avsluta? Skriv [J]a för att avsluta eller [N]ej för att fortsätta\n");
	
	if (toupper(*answer) == 'J')
	    {
	      break;
	    }
	else if(toupper(*answer) == 'N')
	    {
	      continue;
	    }
      }
    else
      {
      puts("Please type a valid choice");
      }
    }

  while(true);
    
 
}





int main()
{
  tree_t *tree = tree_new();
  
  main_loop(tree);
 

  

  return 0;
}


