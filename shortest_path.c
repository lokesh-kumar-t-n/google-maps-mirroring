
#include<stdio.h>
#include<limits.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include "graph.h"

typedef struct q{
	int self_idx;
	int distance;		// parameter on which shortest path is determined.
	int idx_parent;  //parent index
	int flag;		// represents if the node is valid in the queue or not.
}Q;

Q* initialize_src(GRAPH *G,Q *queue){
		int n=G->no_of_cities;
		int i;
		for(i=0;i<n;i++){
			queue[i].self_idx = i;
			queue[i].distance=INT_MAX;
			queue[i].idx_parent = -1;
			queue[i].flag = 0;
		}
		return queue;
}

int empty(Q *queue,int size){
	int i;
	i=0;
	while(i < size){				//flag=0 valid...flag=-1 invalid
		if(queue[i].flag == 0)
			return 0;
		i+=1;
	}
	return 1;
}

Q extract_min(Q *queue,int size){
	//int min=INT_MAX;
	int index;
	Q temp;
	int i;
	for(i=0;i<size;i++){
		if(queue[i].flag==0){
			temp.distance=queue[i].distance;
			temp.self_idx=i;
			index=i;
			break;
		}
	}
	//printf("distance now is =%d\n",temp.distance);
	if(i==size){
		temp.distance=INT_MAX;
		//printf("\nwas it here\n");
		return temp;
	}
	for(i=i+1;i<size;i++){
		//printf("%d,%d\n",i,queue[i].distance );
		if((queue[i].flag == 0) && (temp.distance > queue[i].distance)){
			temp=queue[i];
			temp.self_idx=i;
			index=i;
		}
	}
	queue[index].flag = -1;
	//printf("distance at the end is %d\n",temp.distance);
	return temp;
}

Q* update_queue(GRAPH *G , Q *queue ,CITY* root,NODE *adj ,int distance){
	int v,u;
	v=city_to_index(G,root->name);
	u=city_to_index(G,adj->name);

	if((queue[v].distance +distance)<queue[u].distance ){
		queue[u].distance=queue[v].distance + distance;
		queue[u].idx_parent = v;
	}
	return queue;
}

CITY *new_city(GRAPH *G,int index){
	CITY *temp=(CITY *)malloc(sizeof(CITY));
	temp->name=(char*)malloc(sizeof(char)*100);
	strcpy(temp->name,index_to_city(G,index));
	temp->neighbours = NULL;
	temp->next_city = NULL;
	temp->prev_city = NULL;

	return temp;
}

CITY *find_path(GRAPH *G,Q *queue,int src,int dest){
	//printf("find_path queue is ");
	CITY *root=NULL,*head,*temp,*temp1;
	int i,j,n=G->no_of_cities;
	head=root;
	// for(i=0;i<3;i++){
	// 	printf("(%d,%d) ",queue[i].self_idx,queue[i].idx_parent);
	// }
	// printf("\n");
	// printf("source and destination are %d,%d\n",src,dest);
	//return NULL;
	int index=dest;
	//root = new_city(G,index);
	//root->next_city=NULL;
	//temp=root;
	root=NULL;
	//printf("index=%d src=%d ",index,src);
	//return NULL;
	while(index != src){
		//index=queue[dest].idx_parent;
		//printf("%d ",index);
		temp=root;
		//return NULL;
		root=new_city(G,index);
		//printf("not here\n");
		//return NULL;
		if(root==NULL){
			//printf("root is NULL\n");
			return NULL;
		}
		else{
			root->next_city=temp;
		}
		if(temp!=NULL){
			temp->prev_city=root;
		}
		index=queue[index].idx_parent;
		//printf("index is %d",index);
		//return NULL;
	}
	temp=root;
	root= new_city(G,src);
	root->next_city = temp;
	root->prev_city=NULL;
	//root=temp1;
	if(temp!=NULL){
		temp->prev_city = root;
	}
	return root;
}


CITY* shortest_path(GRAPH* G,char* src,char* dst,NODE* remove){
	//printf("shortest path\n");
	//printf("src=%s, dst=%s",src,dst);
	int cost=0;
	int idx_src = city_to_index(G,src);
	int idx_dest = city_to_index(G,dst);
	int i;
	CITY* source;
	NODE* node_tmp;
	source=G->croot;
	int cur;
	cur=0;
	int size=G->no_of_cities;
	//printf("here");
	while(cur<idx_src){
		source=source->next_city;
		cur+=1;
	}
	//printf("ok");
	Q *queue = (Q *)malloc(sizeof(Q)*size);
	queue = initialize_src(G,queue);
	//printf("source index is %d\n",idx_src);
	queue[idx_src].distance=0;  // initialization of sourse distance to ZERO.
	//printf("initialization done\n");
	while(remove!=NULL){
		i=city_to_index(G,remove->name);
		if(i>=0){
			queue[i].flag=-1;
		}
		node_tmp=remove;
		remove=remove->next;
		free(node_tmp);
	}
	// for(i=0;i<3;i++){
	// 	printf("(%d,%d) ",queue[i].self_idx,queue[i].idx_parent);
	// }			//how do you come with size.
	while(!empty(queue,size)){
		// for(i=0;i<3;i++){
		// 	printf("(%d,%d) ",queue[i].self_idx,queue[i].idx_parent);
		// }
		// printf("\n");
		CITY *root=G->croot;
		Q min=extract_min(queue,size);
		//printf("min distance is %d\n",min.distance);
		if(min.distance==INT_MAX){
			//printf("Sorry, no path exists");
			return NULL;
		}
		if(min.self_idx==idx_dest){
			break;
		}
		while(root){
			//printf("%d is %d\n",min.self_idx,city_to_index(G,root->name));
			if(min.self_idx == city_to_index(G,root->name))
				break;
			root=root->next_city;
		}

		NODE *nhood = root->neighbours;

		while(nhood){
			queue = update_queue(G,queue , root,nhood , nhood->distance);
			nhood=nhood->next;
		}
		// for(i=0;i<3;i++){
		// 	printf("(%d,%d) ",queue[i].self_idx,queue[i].idx_parent);
		// 	printf("inside");
		// }
		// printf("\n");

	}
	//printf("almost done\n");
	// for(i=0;i<3;i++){
	// 	printf("(%d,%d) ",queue[i].self_idx,queue[i].idx_parent);
	// }

	CITY *path = find_path(G,queue,idx_src,idx_dest);
	cost = queue[idx_dest].distance;
	printf("The total distance to move from %s to %s is %d\n",src,dst,cost);
	return path;
}
