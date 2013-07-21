#ifndef __LIST__H
#define __LIST__H

typedef void (*t_nodeFree)( void * );

typedef struct s_node
{
  struct s_node *prev;
  struct s_node *next;
}              t_node;

typedef struct s_list
{
  struct s_node *tail;
  struct s_node *head;

  unsigned int size;
  unsigned int elem_size;
  t_nodeFree   node_free;
} t_list;

t_list * create_list( unsigned int elem_size, t_nodeFree free_func ){
  t_list * ret;
  ret = NULL;
  ret = (t_list*) malloc( sizeof(t_list) );
  ret->elem_size = elem_size;
  ret->node_free = free_func;
  ret->tail = ret->head = NULL;
  ret->size = 0;
  return ret;
}

void clear_list( t_list * lst )
{
  t_node * cur;
  t_node * nxt;
  if( !lst )
    return;
  cur = lst->tail;
  nxt = NULL;
  while( cur ){
    if( cur->free_func )
      lst->free_func( cur );
    nxt = cur->next;
    free(cur);
    cur = NULL;
    cur = nxt;
  }
  lst->tail = lst->head = NULL;
  lst->size = 0;
}

void destroy_list( t_list * lst )
{
  if( !lst )
    return;
  clear_list(lst);
  free (lst);
  lst = NULL;
}

#endif
