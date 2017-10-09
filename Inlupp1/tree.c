

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "list.h"
/// Change this definition and replace int with the appropriate type
/// in your program. Note, however, that your implementation of tree.c
/// should treat T as if it did not know the type.

struct shelf
 {
  char* hylla; //"a12"
  int amount;

};




struct node
{
  node_t *left;
  node_t *right;
  K key;
  T value;
};

/*
struct ware
{
  char *name;
  char *desc;
  int price;
  list_t *list;
};*/

 //typedef struct ware ware_t;
/// Define struct tree in your .c file not here! (why?) 
struct tree
{
  node_t *rootnode;  
};

typedef void(*tree_action)(K key, T elem);


tree_t *tree_new()
{
  tree_t *t = calloc(1, sizeof(tree_t));
  
  return t;
}

/// Remove a tree along with all T elements.

/// \param tree the tree
/// \param cleanup a function that takes a key and element as
///        argument, to be used to free memory. If this param is 
///        NULL, no cleanup of keys or elements will happen.

void node_delete(node_t *node, tree_action cleanup)
{
  if (node->left != NULL)
    {
      node_delete(node->left, cleanup);
      cleanup(node->key, node->value);
    }
  if(node->right != NULL)
    {
      node_delete(node->right, cleanup);
      cleanup(node->key, node->value);
      free(node);
    }
  else
    {
      cleanup(node->key, node->value);
      free(node);
    }
 
  //måste desta detta på något sätt
}


void tree_delete(tree_t *tree, tree_action cleanup)
{

  node_delete(tree->rootnode, cleanup);
  free(tree);
 
}
void cleanup_func(K key, T elem) //behöver hjälp med detta
{
 //free(key);  //någonting inte stämmer här så att det inte funkar med free().
 //free(elem);
}


//behöver någon form jämnföreälse?
int node_size(node_t *node)
{
  if(node == NULL)
    {
      return 0;
    }
  else
    {
  return 1 +node_size(node->left) +node_size(node->right);
    }
}

/// \returns: the number of nodes in the tree
int tree_size(tree_t *tree)
{
  return node_size(tree->rootnode);
}

/// Get the depth of the tree 
/// \returns: the depth of the deepest subtree
int node_depth(node_t *node)
{
  if (node == NULL)
    {
      return 0;
    }
  else
    {
      int leftdepth = node_depth(node->left);
      int rightdepth= node_depth(node->right);
      if (leftdepth > rightdepth)
        return leftdepth +1;
      else
        {
          return rightdepth +1;
        }
    }  
}



int tree_depth(tree_t *tree)
{
  return node_depth(tree->rootnode);
}



node_t *make_node(node_t *node, K Key, T elem)
{
  node_t *new_node = calloc(1, sizeof(node_t));
  
  
  new_node->value = elem;
  new_node->key = Key;
  new_node->right = NULL;
  new_node->left = NULL;
  node = new_node;
  return node;
}

void  node_insert(node_t *node, K key, T elem)
{
  
  int c = strcmp(key, node->key);

   if ( c < 0)    
    {
      
      if (node->left != NULL)
        {
          node_insert(node->left, key, elem);
        }
      else
        {
          node->left = make_node(node, key, elem);
        }
      
    }
  else if( c > 0 )
    {
      if(node->right != NULL)
        {
          node_insert(node->right, key, elem);
        }
      else
        {
          node->right = make_node(node, key, elem);
        }
    }
   
}

/// Insert element(read value) into the tree. Returns false if the key is already used.
///
/// \param tree pointer to the tree
/// \param key the key of element to be appended
/// \param elem the element 
/// \returns: true if successful, else false
bool tree_insert(tree_t *tree, K key, T elem)
{
   if (tree_has_key(tree, key))
    {
      return false;
    }
    
  if (tree->rootnode == NULL)
    {
      tree->rootnode= make_node(tree->rootnode, key, elem);
      return true;
    }
  else 
    {
      node_insert(tree->rootnode, key, elem);
      return true;
    }
}

/// Checks whether a key is used in a tree
///
/// \param tree pointer) to the tree
/// \param key the key to check for inclusion in the tree
/// \returns: true if key is a key in the tree

bool node_has_key(node_t *node, K key)
{
  if(!(node==NULL))
    {
      if(strcmp(key, node->key) < 0)
        {
          return node_has_key(node->left, key);
        }
      if(strcmp(key, node->key) > 0)
        {
          return node_has_key(node->right, key);
        }
            
        
      return true;
      
    }
  else
    {
      return false;
    }
}

bool tree_has_key(tree_t *tree, K key)
{
  return node_has_key(tree->rootnode, key); 
}
/// Returns the element for a given key in tree.
/// (The implementation may assume that the key exists, or reserve
/// e.g. NULL as a value to indicate failure)
/// 
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \returns: the element associated wity key key
T node_get(node_t *node, K key) //pointer to pointer?
{
  if(strcmp(key, node->key)<0)
    {
      
      return node_get(node->left, key);
    }
  if(strcmp(key, node->key)>0)
    {
      
      return node_get(node->right, key);
    }
      return (node->value);
    
}

T tree_get(tree_t *tree, K key)
{
  if(strcmp(key, tree->rootnode->key)==0)
    {
      
      return tree->rootnode->value;
    }
  else
    {
      return node_get(tree->rootnode, key);
    }
}

/// This does not need implementation until Assignment 2
///
/// \param tree pointer to the tree
/// \param key the key of elem to be removed
/// \returns: the removed element
T tree_remove(tree_t *tree, K key);

/// Swap the element for a given key for another.
///
/// \param tree pointer to the tree
/// \param key the key of elem to be changed
/// \param elem the updated element 
/// \returns: the updated element
//////////// ================= Added in version 1.2
///
/// NOTE: Implementing these functions is NOT mandatory
///

/// Returns an array holding all the keys in the tree
/// in ascending order.
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() keys


void node_keys(K *keys, node_t *node, int *i) //bra exempel på rekursion
{
  if(node!= NULL)
    {
      if (node->left != NULL)
        {
          node_keys(keys, node->left, i);
        }
      if (node->right != NULL)
        {
          node_keys(keys, node->right, i);
        }

      keys[*i] = node->key;
      (*i)++;
    }
}

K *tree_keys(tree_t *tree) //exempel på värdeöverföring via pekare?
{
  K *keys = calloc(tree_size(tree),sizeof(K)); 
  int i = 0;

  //node_t *node = tree->rootnode; 
 
  node_keys(keys, tree->rootnode, &i); 

  return keys;
}


void node_elements(T *elements, node_t *node, int *i) //bra exempel på rekursion
{
  if(node!= NULL)
    {
      if (node->left != NULL)
        {
          node_elements(elements, node->left, i);
        }
      if (node->right != NULL)
        {
          node_elements(elements, node->right, i);
        }

      elements[*i] = node->value;
      (*i)++;
    }
}

T *tree_elements(tree_t *tree) //exempel på värdeöverföring via pekare?
{
  T *elements = calloc(tree_size(tree),sizeof(T)); 
  int i = 0;

  //node_t *node = tree->rootnode; 
 
  node_elements(elements, tree->rootnode, &i); 

  return elements;
}

/// Returns an array holding all the elements in the tree
/// in ascending order of their keys (which are not part
/// of the value).
///
/// \param tree pointer to the tree
/// \returns: array of tree_size() elements
//T *tree_elements(tree_t *tree);

/// This function is used in tree_apply() to allow applying a function
/// to all elements in a tree. 
//typedef void(*tree_action2)(K key, T elem, void *data);

//enum tree_order { inorder = 0, preorder = -1, postorder = 1 };

void in_order_tree_apply(node_t *node,tree_action2 fun, void *data)
{
  if(node != NULL)
    {
      if(node->left != NULL)
	{
	  in_order_tree_apply(node->left, fun, data);
	}
      if(node->right != NULL)
	{
	  in_order_tree_apply(node->right, fun, data);
	}
      fun(node->key, node->value, data);
    }
}

/// Applies a function to all elements in the tree in a specified order.
/// Example (using shelf as key):
///
///     tree_t *t = tree_new();
///     tree_insert(t, "A25", some_item);
///     int number = 0;
///     tree_apply(t, inorder, print_item, &number);
///
/// where print_item is a function that prints the number and increments it,
/// and prints the item passed to it. 
///
/// \param tree the tree
/// \param order the order in which the elements will be visited
/// \param fun the function to apply to all elements
/// \param data an extra argument passed to each call to fun (may be NULL)
void tree_apply(tree_t *tree, enum tree_order order, tree_action2 fun, void *data)
{
  if(order == 0)
    {
      in_order_tree_apply(tree->rootnode, fun, data); 
    }
  if (order == -1)
    {
      return;
    }
  
  else if(order == 1)
    {
      return;
    }
}


/*void print_node( node_t *node, int i)
{
  
  if(node != NULL)
    {
      if(node->left != NULL)
	{
	  print_node(node -> left, i + 1);
	  printf("%d: %s \n", i ,node -> key);
	}
      else if(node->right != NULL)
	{
	  printf("%d: %s \n", i ,node -> key);
	  print_node(node ->right, i + 1);
	}
    }
} */
 /*
void print_tree(tree_t *tree, int i)
{
  print_node(tree->rootnode, i);
}
*/
/*
int main()
{
  int *value_1 = malloc(sizeof(int));
   *value_1= 111;

   int *value_2 = malloc(sizeof(int));
   *value_2= 222;
   
   int *value_3 = malloc(sizeof(int));
  *value_3 = 333;


 
  tree_t *tree1;
  tree1 = tree_new();
  tree_insert(tree1, "hej", value_1);
  tree_insert(tree1, "då", value_2);
  tree_insert(tree1, "mama", value_3);
  //print_tree(tree1);
  //T f = tree_get(tree1, "hej");
  //tree_delete(tree1, cleanup_func);
  int s = tree_size(tree1);
  int d = tree_depth(tree1);
  printf("size is %d\n and depth is %d\n",s, d);
  K *c = tree_keys(tree1);
  printf("Key = %s\n",c[2]);
  //tree_has_key(tree1, "då");
  print_tree(tree1);
  //printf("Value: %d", *f); 
  return 0;
}


  node_get

{
  if(node)
    {
      if(node->key == key)
        {
          return *node->value;
        }
      else
        {
          if (node->left != NULL)
            {
              T left = node_get(node->left, key);
              return left;                     
            }
          if (node->right != NULL)
            {
          
              return node_get(node->right, key);
            }
        }
    }
} 
*/
