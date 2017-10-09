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
typedef struct node node_t;
typedef struct tree tree_t;
typedef struct link link_t;
typedef struct list list_t;

struct link
{
  L shelf;
  link_t *next;
};

struct list
{
   link_t *first;
   link_t *last;
};

struct shelf
 {
  char *shelfname;
  int amount;

};

struct node
{
  node_t *left;
  node_t *right;
  K key;
  T value;
};

 
struct tree
{
  node_t *rootnode;  
};

struct ware
{
  char *name;
  char *desc;
  int price;
  list_t *list;
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
  ware_t **elements = tree_elements(tree);

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

int print_nodes(node_t *node, int i)
{
  if(node == NULL)
    {
      return 0;
    }
  else
    {
      if(node->left != NULL)
	{
	  i = print_nodes(node->left, i) +1;
	}
      printf("%d: %s\n",i,node->key);
      if(node->right !=NULL)
	{
	  i = print_nodes(node->right, i+1);
	}
      return i;
    }
}

void print_tree(tree_t *tree)
{
  if(tree->rootnode == NULL)
    {
      return;
    }
  
  print_nodes(tree->rootnode, 1);
}

void print_ware_index(tree_t *tree)
{
  int index = ask_question_int("Skriv in index vara vill du se \n");
  ware_t **elements = tree_elements(tree);
  ware_t *elem = elements[index-1];
  list_t *list_ware = elem->list;
  shelf_t **shelf1 = list_get(list_ware,index);
  char *namnshelf =(*shelf1)->shelfname;
  if(index > (tree_size(tree)) || index < 0)
    {
      puts("Skirv ett giltigt index\n");
      index = ask_question_int("Skriv in index vara vill du se \n");
    }
  printf("Namn: %s\n",elem->name);
  printf("Desc: %s\n",elem->desc);
  printf("Pris: %d\n",elem->price);
  printf("Shelf: %s\n",);
  printf("Amount: %d\n");
}

ware_t *input_ware(char *name,char *desc, int price, char *shelf_name, int amount)
{
  L shelf = make_shelf(shelf_name, amount);
  list_t *list1 = list_new();
  list_prepend(list1, shelf);
  return make_ware(name, desc,price, list1);
     
}
void add_item(tree_t *tree)
{
  char *name = ask_question_string("Mata in namnet: \n");
  char *desc = ask_question_string("Skriv beskrivning: \n");
  int price = ask_question_int("Pris: \n");
  char *shelfname = ask_question_shelf("Hyllan: \n");
  int amount = ask_question_int("Amount: \n");
  ware_t *ware;
  print_ware(name,desc,price,shelfname,amount);

  char c = ask_question_ware();
  if (c == 'J')
    {
      
      
      if(tree_has_key(tree,name) == false)
	{
	  if(shelves_free(tree, shelfname) == false)
	  {
	      ware =  input_ware(name,desc,price,shelfname,amount);
	      tree_insert(tree,ware->name,ware);
	  }
	}
      else
	{
	  
	  printf("%s already exist \n", name);
	  
	  
	}
    }
  if ( c == 'N')
    {
      return;
    }
  else if( c == 'R')
    {
      //edit_item();//FIXME
    }
}


char* welcome = ("\nVälkommen till lagerhantering 1.0\n ================================= \n [L]ägga till en vara \n [T]a bort en vara \n [R]edigera en vara \n Ån[g]ra senaste ändringen \n Lista [h]ela varukatalogen \n [A]vsluta \n Vad vill du göra idag? _");

void main_loop (tree_t *tree)
{
  
  do
    {
    char c = ask_question_char(welcome);

    if(c == 'L')
      {
    add_item (tree);
      }
    else if( c == 'T')
      {
	int d = tree_size(tree);
	printf("%d \n", d);
      	// void remove_item(void); fixme
      }
    else if( c == 'R')
      {
        void  edit_item (void);
      }
    else if(c == 'G')
      {
        void  regret_choice (void);
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
	answer= ask_question_string("Vill du verkligen avsluta? Skriv ja för att avsluta eller nej för att fortsätta\n");
	
	if (strcmp(answer,"ja") == 0)
	    {
	      break;
	    }
	else if(strcmp(answer,"nej") == 0)
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


