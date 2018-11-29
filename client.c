#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "graph.h"
#define FREQ 2.60e9
unsigned long long rdtsc(){
  unsigned long long int x;
  unsigned a, d;

  __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

  return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}
int build(GRAPH* G){
    FILE* fp;
    char city[100],city1[100];
    NODE* neighbours,*tmp;
    int distance,len;
    char buffer[1000];
    int i,j;
    int cur_len;
    fp=fopen("needed.txt","r");
    while(fgets(city,100,fp)!=NULL){
        city[strlen(city)-1]='\0';
        //printf("%s\n",city);
        add_city(G,city);
    }
    //printf("done\n");
    //return 1;
    fclose(fp);
    cur_len=0;
    neighbours=NULL;
    strcpy(city,"none");
    fp=fopen("distances.txt","r");
    while(fgets(buffer,1000,fp)!=NULL){
        //printf("Okay");
        //printf("buffer is %s\n",buffer);
        i=0;
        len=strlen(buffer);
        while((i<len)&&(buffer[i]!=',')){
            i+=1;
        }
        if(i==len){
            if(buffer[len-1]=='\n'){
                buffer[len-1]='\0';
            }
            if(strcmp(city,"none")!=0){
                //printf("called");
                insert(G,city,neighbours);
                //printf("returned");
            }
            while(neighbours!=NULL){
                tmp=neighbours;
                neighbours=neighbours->next;
                free(tmp);
            }
            strcpy(city,buffer);
        }
        else{
            while((i<len)&&(buffer[i]!='\n')){
                strncpy(city1,buffer,i);
                city1[i]='\0';
                distance=0;
                i+=1;
                while((i<len)&&(buffer[i]!=',')){
                    distance=distance*10 +(buffer[i]-'0');
                    i+=1;
                }
                //printf("here");
                //printf("so");
                tmp=(NODE*)malloc(sizeof(NODE));
                tmp->name=(char*)malloc(sizeof(char)*100);
                strcpy(tmp->name,city1);
                tmp->distance=distance;
                tmp->next=neighbours;
                neighbours=tmp;
                if(i<len){
                    strcpy(buffer,buffer+i+1);
                    len=strlen(buffer);
                    i=0;
                    //printf("buffer in between is =%s\n",buffer);
                    while(buffer[i]!=','){
                        i+=1;
                    }
                }
            }
        }
    }
    if(strcmp(city,"none")!=0){
        //printf("called");
        insert(G,city,neighbours);
        //printf("returned");
    }
    while(neighbours!=NULL){
        tmp=neighbours;
        neighbours=neighbours->next;
        free(tmp);
    }
    fclose(fp);
    return 0;
}
void init_graph(GRAPH *G){
    //G=(GRAPH*)malloc(sizeof(GRAPH));
    G->croot=NULL;
    G->t=NULL;
    G->idx_arr=NULL;
    G->no_of_cities=0;
}
void main(){
    unsigned long long int t0,t1;
    GRAPH G;
    int t;
    int i,j;
    char src[100],dst[100],cur_city[100];
    NODE* tmp,*start;
    CITY* city,*tmp2;
    int n;
    init_graph(&G);
    if(G.t==NULL){
        //printf("it is null");
    }
    t0=rdtsc();
    build(&G);
    t1=rdtsc();
    printf("time taken to build the graph is %f\n",(t1-t0)/FREQ);
    tmp=NULL;
    printf("Enter the number of test cases\n");
    scanf("%d",&t);

    for(i=0;i<t;i++){
        printf("\n");
        printf("Enter the source\n");
        scanf("%s",src);
        printf("Enter the destination\n");
        scanf("%s",dst);
        printf("Enter the number of cities would not like to go to. If none enter 0\n");
        scanf("%d",&n);
        start=NULL;
        for(j=0;j<n;j++){
            printf("Enter the next city [%d] which you want to avoid\n",j+1);
            scanf("%s",cur_city);
            if(strcmpi(cur_city,src)==0){
                printf("Sorry, you have to move through source\n");
            }
            else if(strcmpi(cur_city,dst)==0){
                printf("Sorry, you have to move through destination\n");
            }
            else{
                tmp=(NODE*)malloc(sizeof(NODE));
                tmp->name=(char*)malloc(sizeof(char)*(strlen(cur_city)));
                strcpy(tmp->name,cur_city);
                tmp->next=start;
                start=tmp;
            }
        }
        if(city_to_index(&G,src)==-1){
            printf("Sorry, source is not found in the database\n");
        }
        else if(city_to_index(&G,dst)==-1){
            printf("Sorry, destination is not found in the database\n");
        }
        else{
            t0=rdtsc();
            city=shortest_path(&G,src,dst,start);
            t1=rdtsc();
            if(city==NULL){
                printf("Sorry, no path is found");
            }
            else{
                while(city->next_city!=NULL){
                    tmp2=city;
                    printf("%s->",city->name);
                    city=city->next_city;
                    free(tmp2);
                }
                printf("%s\n",city->name);
                free(city);
                printf("Search time is %f\n",(t1-t0)/FREQ);
            }
        }
        while(start!=NULL){
            tmp=start;
            start=start->next;
            free(tmp);
        }
    }
    city= G.croot;
    while(city!=NULL){
        start=city->neighbours;
        while(start!=NULL){
            tmp=start;
            start=start->next;
            free(tmp);
        }
        tmp2=city;
        city=city->next_city;
        free(tmp2);
    }
}
