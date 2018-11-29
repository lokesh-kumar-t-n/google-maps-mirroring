#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "graph.h"

int c = 10;
static int gi(char ch)
{
	return ch-97;
}


static TREE_NODE *create_node()
{
	TREE_NODE *new_node = (TREE_NODE *)malloc(sizeof(TREE_NODE));
	int i;
	for(i=0;i<26;i++)
	{
		new_node->next[i] = NULL;
	}
	new_node->idx = -1;
	return new_node;
}

static void make_trie(TREE_NODE *root,char *city,int idx)
{
	int i;
	int l = strlen(city);
	TREE_NODE *temp;
	temp = root;
	for(i=0;i<l;i++)
	{
		//printf("tree traversal\n");
		if((temp->next[gi(city[i])]) == NULL)
		{
			//printf("tree if\n");
			temp->next[gi(city[i])] = create_node();//alphabet not there
			temp = temp->next[gi(city[i])];

		}
		else
		{
			//printf("tree else\n");
			temp = temp->next[gi(city[i])];//alphabet there
		}
	}
	temp->idx = idx;//end place the correct index;
	//printf("temp idx %d\n",temp->idx);
	//return 1;
}

char* index_to_city(GRAPH* G,int idx)
{
	INDEX_ARRAY *t1 = G->idx_arr;
	CITY_DB *arr = t1->array;
	char *incity = (char *)malloc(sizeof(int)*100);
	incity = arr[idx].name;
	//strlwr(incity);
	return incity;
}

static void Strlwr(char *city)
{
	int i = 0;
	int len=strlen(city);
	while(i<len)
	{
		if(city[i]>='A' && city[i]<='Z')
		{
			city[i] = city[i] - 'A' + 'a';
		}
		i++;
	}
}

int city_to_index(GRAPH* G,char* city)
{
	int i,len;
	len = strlen(city);
	Strlwr(city);//to lower case
	if(G->t == NULL){
		return -1;
	}
	TREE_NODE *head = G->t->root;
	TREE_NODE *p,*c;
	if(head==NULL)
	{
		return -1;//no cities in db
	}
	else
	{
		i=0;
		c = head;
		p = c;
		while(i<len)
		{
			if(c->next[gi(city[i])]!=NULL)
			{
				c = c->next[gi(city[i])];//go to next node by traversing the edge
				i++;
			}
			else
			{
				break;//fail to find fell of the tree
			}
		}
		if(i==len)
		{
			return c->idx;
		}
		else
		{
			return -1;//city not in db
		}
	}
}


int add_city(GRAPH *G,char *city)
{
	int n = G->no_of_cities;

	Strlwr(city);
	if(n==0)
	{
		CITY_DB *t1 = (CITY_DB *)malloc(sizeof(CITY_DB)*10);
		G->idx_arr = (INDEX_ARRAY *)malloc(sizeof(INDEX_ARRAY));
		G->idx_arr->array = t1;
		strcpy(t1[0].name,city);
		G->t = (TRIE *)malloc(sizeof(TRIE));
		G->t->root = create_node();
		make_trie(G->t->root,city,n);
		G->no_of_cities++;
		return 0;
	}
	//int c = 10;
	int check = (city_to_index(G,city));
	if(check!=-1)
	{
		return check;
	}
	else
	{
		if(n>c-1)
		{
			//printf("realloc\n");
			CITY_DB *t2 = (CITY_DB *)malloc(sizeof(CITY_DB)*(n+c));
			CITY_DB *t1 = G->idx_arr->array;
			c = c + n;
			int i;
			for(i=0;i<n;i++)
			{
				t2[i] = t1[i];
			}
			G->idx_arr->array = t2;
			strcpy(t2[n].name,city);
			free(t1);
			make_trie(G->t->root,city,n);
			G->no_of_cities++;
			return n;
		}
		else
		{
			//printf("tis else\n");
			CITY_DB *t1;
			t1 = G->idx_arr->array;
			strcpy(t1[n].name,city);
			make_trie(G->t->root,city,n);
			G->no_of_cities++;
		}
	}
	return n;
}
/*
GRAPH *init_graph(){
    GRAPH *G=(GRAPH*)malloc(sizeof(GRAPH));
    G->croot=NULL;
    G->t=NULL;
    G->idx_arr=NULL;
    G->no_of_cities=0;
    return G;
}
int main(){
    GRAPH * G;
    int i,j;
    G = init_graph();
    //char *city  = "abc";
    int idx;
    idx = add_city(G,"dab");
    idx = add_city(G,"abc");
    printf("return index %d\n",idx);
    //printf("return index %d\n",idx);

    i=city_to_index(G,"abc");
    idx = add_city(G,"bab");
    printf("return idx %d\n",idx);
    printf("Index of abc is %d\n",i);
    j=city_to_index(G,"dab");
    printf("Index of dab is %d\n",j);
    printf("city at index %d is %s(expected abc)\n",i,index_to_city(G,i));
    printf("city at index %d is %s(expected dab)\n",j,index_to_city(G,j));
    printf("city at index %d is %s(expected bab)\n",idx,index_to_city(G,idx));

    return 0;
}
*/
