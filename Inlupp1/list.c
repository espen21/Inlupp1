#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/// Change this definition and replace int with the appropriate type
/// in your program.
/*
struct shelf
 {
  char *shelfname; //"a12"
  int amount;

};
*/


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


/// Creates a new list
///
/// \returns: empty list

list_t *list_new()

{
  list_t *l = calloc(1, sizeof(link_t));
  l ->first = NULL; //calloc gör detta redan?
  l ->last = l->first;
  return l;
} 

/// Inserts a new element at the end of the list
///
/// \param list pointer to the list
/// \param elem the element to be appended
// L är shelf_t och därför är elem shelf_t.
void list_append(list_t *list, L elem)
{
  link_t *new_elem = calloc(1, sizeof(link_t)); //skapar en lista?
  new_elem-> shelf = elem ;      //shelf-delen ersätts av elem variabeln?
  new_elem->next = NULL;
  if ( list -> first == NULL) //om listan är tom
    {
     list->first = new_elem;
     list->last = list->first;
     
    }
  else
    {
      list->last->next= new_elem;
      list->last = new_elem;
    }
}




/// Inserts a new element at the beginning of the list
///
/// \param list pointer to the list
/// \param elem the element to be prepended
void list_prepend(list_t *list, L elem)

{
  link_t *new_elem = calloc(1, sizeof(link_t)); //skapar en lista?
  new_elem-> shelf = elem ;      //shelf-delen ersätts av elem variabeln?
  new_elem->next = list->last;
  if ( list -> first == NULL)
    {
     list->first = new_elem;
     list->last = list->first;
    }
  else
    {
      new_elem -> next = list-> first;
      list->first = new_elem;
    }
}



bool list_insert(list_t *list, int index, L elem)
{
  link_t *new_elem = calloc(1, sizeof(link_t)); 
  new_elem-> shelf = elem ;
  link_t *current = list->first;
  int c = list_length(list);
  if(index == 0 && list->first == NULL)
    {
      new_elem->next = list->first;
      list->first= new_elem;
      list->last = list->first;
      
    }
  else if (index == 0)
    {
      new_elem->next = list->first;
      list->first= new_elem;
      return true;
    }
  else if (index+1 == c)
    {
      list_append(list, elem);
      return true;
    }
  else if (index+1 > c)
    {
      return false;
    }
  if(index>=1)
    {
      for(int i=1; i < index; i++)
        {
          if(current->next == NULL)
            {
              return false;
            } 
          current = current->next;
          
        } 
  
      new_elem->next = current->next;
      current->next = new_elem;
      return true;
  
    }
  
  return true;
}


/// Removes an element from a list.
///
/// Example: (assume l == [e2, e3, e1, e4])
///
/// int elem;

bool list_remove(list_t *list, int index, L *elem)
{
  link_t *current = list->first;
  link_t *tmp_node = NULL;
  if (index == 0 && current == NULL)
    {
      return false; 
    }
  else if (index == 0 && list_length(list) ==1)
    {
      tmp_node= list->first->next;  //varför funkar inte med current?
      *elem = list->first->shelf;
      free(list->first);
      list->first = tmp_node;
      list->last = list->first;      
      return true;
    }
  else if (index == 0)
    {
      tmp_node= list->first->next;  //varför funkar inte med current?
      *elem = list->first->shelf;
      free(list->first);
      list->first = tmp_node;
      return true;
    }
  else if (index +1 == list_length(list))
    {
      for(int i = 0; i < index-1; i++)
        {
          if (current->next == NULL)
            {
              return false;
            } 
          current = current->next;
        }
      tmp_node = current->next;
      current->next = tmp_node->next;
      *elem = current->shelf;
      free(tmp_node);
      list->last = current;  
      return true;    
    }    
  
  for(int i = 0; i < index-1; i++)
    {
      if (current->next == NULL)
        {
          return false;
        } 
      current = current->next;
    }
  tmp_node = current->next;
  current->next = tmp_node->next;
  *elem = current->shelf; //gotta find out about this
  free(tmp_node);  
  return true;
  
  
}

/// Returns the element at a given index
/// \param list  pointer to the list
/// \param index the index to be returns
/// \returns a pointer to the element at index index
L *list_get(list_t *list, int index)
{
  link_t *chosenlink = NULL;
  if (index == 0)
    {
      return list_first(list);
    }
  else if (index+1 == list_length(list))
    {
      return list_last(list);
    }
  for(int i = 0; i<index; i++)
    {
      chosenlink = list->first->next;
    }
  return &chosenlink->shelf;
}
/// A convenience for list_get(list, 0)
L *list_first(list_t *list)
{
  L *chosenlink = NULL;
  chosenlink = &list->first->shelf;
  return chosenlink;
}

/// A convenience for list_get(list, -1)
L *list_last(list_t *list)
{
  L *chosenlink = NULL;
  chosenlink = &list->last->shelf;
  return chosenlink;
}

/// Returns the length of the list. It is undefined
/// whether the length is calculated in O(n) time or
/// whether there is a size counter in the list object
/// that is manipulated by insert, remove, etc. 
/// \param list the list
/// \returns the length of list
int list_length(list_t *list)
{
link_t *current = list->first;
 int count = 0; 
 while(current != NULL)
   {
     current = current->next;
     count++;
   }
     
 return count;
}

void list_apply(list_t *list, list_action2 fun, void *data)
{
  link_t *current = list->first;

  do
    {
      fun(current->shelf, data);
      current = current->next;
    }
  while(current != NULL);
}


/*

int main (void)
{
  
  list_t *list1 = list_new();
  //list_append( list1, 2);
  //list_append( list1, 3);
  list_append(list1,4);
  L *elem = calloc(1,sizeof(L));
  // list_remove(list1,0, elem);
  list_append(list1,10);
  list_append(list1,11);
  printf("%d \n",list_length(list1));
  printf("%d-%d-%d \n", list1->first->value, list1->first->next->value, list1->last->value);
  
  
  return 0;
}

*/
//look at make file and how to effectively integrate all these fucking files.
