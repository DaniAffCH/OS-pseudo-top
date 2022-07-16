#include "linked_list.h"
#include <assert.h>
#include <stdio.h>
#include "types.h"

void List_init(ListHead* head) {
  head->first=0;
  head->last=0;
  head->size=0;
}

ListItem* List_find(ListHead* head, ListItem* item) {
  // linear scanning of list
  ListItem* aux=head->first;
  while(aux){
    if (aux==item)
      return item;
    aux=aux->next;
  }
  return 0;
}

ListItem* List_insert(ListHead* head, ListItem* prev, ListItem* item) {
  if (item->next || item->prev)
    return 0;
  
#ifdef _LIST_DEBUG_
  // we check that the element is not in the list
  ListItem* instance=List_find(head, item);
  assert(!instance);

  // we check that the previous is inthe list

  if (prev) {
    ListItem* prev_instance=List_find(head, prev);
    assert(prev_instance);
  }
  // we check that the previous is inthe list
#endif

  ListItem* next= prev ? prev->next : head->first;
  if (prev) {
    item->prev=prev;
    prev->next=item;
  }
  if (next) {
    item->next=next;
    next->prev=item;
  }
  if (!prev)
    head->first=item;
  if(!next)
    head->last=item;
  ++head->size;
  return item;
}

ListItem* List_detach(ListHead* head, ListItem* item) {

#ifdef _LIST_DEBUG_
  // we check that the element is in the list
  ListItem* instance=List_find(head, item);
  assert(instance);
#endif

  ListItem* prev=item->prev;
  ListItem* next=item->next;
  if (prev){
    prev->next=next;
  }
  if(next){
    next->prev=prev;
  }
  if (item==head->first)
    head->first=next;
  if (item==head->last)
    head->last=prev;
  head->size--;
  item->next=item->prev=0;
  return item;
}

void List_print(ListHead* head){
  procListItem* it = (procListItem*) head->first;

  while(it){
    printf("[%u] -> ", it->info->pid);

    it = (procListItem*) it->list.next;
  }
  printf("\n\n");
}

procListItem* List_find_by_pid(ListHead* head, pid_t pid){
  procListItem* it = (procListItem*) head->first;

  while(it){
    if(it->info->pid == pid)
      return it;
    it = (procListItem*) it->list.next;
  }
  return NULL;
}
