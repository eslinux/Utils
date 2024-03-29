#include <stdio.h>
#include <stdbool.h>
#include "Slist.h"


slist * slist_append(slist * list, void * data)
{
	slist * walk=list;
	slist * add_item= (slist*)malloc(sizeof(slist));
	if(!add_item)
	{
		perror("Out of memory !!!\n");
		return list;
	}
	add_item->data = data;
	add_item->next=NULL;
	do{
		if(walk){
			if(walk->next == NULL){
				walk->next=add_item;
				break;
			}
		}else{
			return add_item;
		}
		walk=walk->next;
	}while(walk);
	return list;
}

slist * slist_remove_by_data(slist * list, void * data)
{
	return slist_remove_by_data_x(list,data,NULL);
}

slist * slist_remove_by_data_x(slist * list, void * data, void (* release_data) (void * data))
{
	if(!list) return NULL;
	slist * walk=list, *tmp=list, *out_list=list;
	while(walk){
		if(walk->data == data){
			if(tmp==walk){ //first item
//				printf("Found ========= \n");
				if(release_data){
					release_data(walk->data);
				}
				out_list = walk->next;
				tmp=walk;
				walk=walk->next;
				free(tmp);
				tmp=walk;
				continue;
			}else{ //not first item
				if(release_data){
					release_data(walk->data);
				}
				tmp->next = walk->next;
				free(walk);
				walk=tmp->next;
				continue;
			}
		}else{
			tmp=walk;
			walk=walk->next;
		}
	}
	return out_list;
}

slist * slist_remove_by_index(slist * list, int index)
{
	//index begin by 0
	return slist_remove_by_index_x(list,index, NULL);
}

slist * slist_remove_by_index_x(slist * list, int index, void (* release_data) (void * data))
{
	if(!list) return NULL;
	//index begin by 0
	slist * walk=list,*tmp=list, *out_list = list;
	int i=0;
	while(i<index){
		if(walk && walk->next){
			tmp=walk;
			walk=walk->next;
		}else{
			return out_list;
		}
		i++;
	}
	if(index == 0 ) { //first
		out_list = walk->next;
		if(release_data) {
			release_data(walk->data);
		}
		free(walk);
	}else if(i==index ){ //normal and not first
		tmp->next = walk->next;
		if(release_data) {
			release_data(walk->data);
		}
		free(walk);
	}
	return out_list;
}

slist * slist_delete_all(slist * list)
{
	return slist_delete_all_x(list,NULL);
}

slist * slist_delete_all_x(slist * list, void (*release_data)(void *data))
{
	if(!list) return NULL;
		//index begin by 0
	slist * walk=list;
	while(walk){
		list=walk;
		walk=walk->next;
		if(release_data) release_data(list->data);
		free(list);
	}
	return NULL;
}

slist * slist_concat(slist * list, slist * added_list)
{
	if(!list && !added_list ) return NULL;
	slist * walk=list;
	while(walk){
		if(! walk->next) break;
		walk=walk->next;
	}
	if(walk){
		walk->next=added_list;
	}else{
		return added_list;
	}
	return list;
}

slist * slist_insert(slist * list,void * data,void * data_cond, bool (*condition)(void * data1,void * data2))
{
	if(!list || !condition) return list;
	slist * walk=list;
	while(walk){
		if(condition(walk->data,data_cond)==true){
			slist * inst= (slist*)malloc(sizeof(slist));
			if(!inst){
				perror("Out of memory !!!\n");
				return list;
			}
			inst->data=data;
			inst->next=walk->next;
			walk->next=inst;
			return list;
		}
		walk=walk->next;
	}
	return list;
}

slist * slist_sort(slist * list, bool (*compare_func)(void * data1, void * data2) )
{
	if(!list || !compare_func) return list;
	slist *walk=list, * tmp=list;
	void * tmp_data;
	while(tmp){
		walk=tmp->next;
		while(walk){
			if(compare_func(tmp->data,walk->data) == true){
				tmp_data = tmp->data;
				tmp->data = walk->data;
				walk->data=tmp_data;
			}
			walk=walk->next;
		}
		tmp=tmp->next;
	}
	return list;
}

slist * slist_find(slist * list, void * data, bool (*compare_func)(void * data1, void * data2))
{
	if(!list || !compare_func) return list;
	slist *walk=list;
	while(walk){
		if(compare_func(data,walk->data) != false) return walk;
		walk=walk->next;
	}
	return NULL;
}

size_t  slist_size(slist * list)
{
	if(!list) return 0;
	slist *walk=list;
	size_t sz=0;
	while(walk)
	{
		sz++;
		walk=walk->next;
	}
	return sz;
}

inline void slist_trace(slist *list, void * data, void (* trace_func)(int index, void * data,void * itemData))
{
	slist_work(list,data,trace_func);
}

void slist_work(slist *list, void * data,void (* work_func)(int index,void * data,void * itemData))
{
	if(!work_func || !list) return;
	volatile int i=0;
	while(list){
		work_func(i++,data,list->data);
		list=list->next;
	}
}


slist*  slist_prepend(slist *list, void* data)
{
    slist *new_list;

      new_list = (slist*) malloc (sizeof (slist));
      new_list->data = data;
      new_list->next = list;

      return new_list;
}

slist* slist_nth(slist  *list, int n)
{
    while (n-- > 0 && list)
        list = list->next;

      return list;
}

void * slist_nth_data(slist *list, int  n)
{
    while (n-- > 0 && list)
        list = list->next;

      return list ? list->data : NULL;
}

int slist_index(slist *list, void *data)
{
    int i;

    i = 0;
    while (list)
    {
        if (list->data == data)
            return i;
        i++;
        list = list->next;
    }

    return -1;
}

slist*  slist_last(slist *list)
{
    if (list)
    {
        while (list->next)
        list = list->next;
    }

      return list;
}


slist* slist_insert_index (slist   *list, void *data, int position)
{
    slist *prev_list = NULL;
    slist *tmp_list = NULL;
    slist *new_list = NULL;

    if (position < 0)
        return slist_append (list, data);
    else if (position == 0)
        return slist_prepend (list, data);

    new_list = (slist *)malloc(sizeof(slist));
    if(!new_list)
    {
        perror("Out of memory !!!\n");
        return list;
    }
    new_list->data = data;

    if (!list)
    {
        new_list->next = NULL;
        return new_list;
    }

    //prev_list = NULL;
    tmp_list = list;

    while ((position-- > 0) && tmp_list)
    {
        prev_list = tmp_list;
        tmp_list = tmp_list->next;
    }

    if (prev_list)
    {
        new_list->next = prev_list->next;
        prev_list->next = new_list;
    }
    else
    {
        new_list->next = list;
        list = new_list;
    }

    return list;
}

