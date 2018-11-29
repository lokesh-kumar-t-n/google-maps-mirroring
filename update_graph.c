#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "graph.h"
NODE* MergeSort(GRAPH* G,NODE* neighbours){//returns pointer to the first node of the sorted list
    if((neighbours==NULL)||(neighbours->next==NULL)){//if it has no element or 1 element
        return neighbours;
    }
    NODE* end,*mid,*prev;
    end=neighbours;
    prev=NULL;
    mid=neighbours;
    while(end!=NULL){
        end=end->next;
        if(end!=NULL){
            prev=mid;
            mid=mid->next;
            end=end->next;
        }
    }
    prev->next=NULL;
    end=MergeSort(G,neighbours);
    mid=MergeSort(G,mid);
    if(mid==NULL){
        neighbours=end;
        end=end->next;
    }
    else if(end==NULL){
        neighbours=mid;
        mid=mid->next;
    }
    else{
        if(city_to_index(G,mid->name)<city_to_index(G,end->name)){
            neighbours=mid;
            mid=mid->next;
        }
        else{
            neighbours=end;
            end=end->next;
        }
    }
    prev=neighbours;
    while((end!=NULL)&&(mid!=NULL)){
        if(city_to_index(G,mid->name)<city_to_index(G,end->name)){
            prev->next=mid;
            mid=mid->next;
            prev=prev->next;
        }
        else{
            prev->next=end;
            end=end->next;
            prev=prev->next;
        }
    }
    while(mid!=NULL){
        prev->next=mid;
        mid=mid->next;
        prev=prev->next;
    }
    while(end!=NULL){
        prev->next=end;
        end=end->next;
        prev=prev->next;
    }
    prev->next=NULL;
    //printf("returned");
    return neighbours;
}
NODE* insert(GRAPH* G,char *city,NODE* neighbours){
    NODE* tmp1,*tmp2,*new,*new_tmp;
    CITY* cur_city,*city_tmp,*city_prev;
    NODE* store;
    int i,j;
    int cur_idx;
    //printf("so ");
    cur_idx=city_to_index(G,city);
    if(cur_idx==-1){
        //printf("adding");
        add_city(G,city);
        cur_idx=city_to_index(G,city);
    }
    //printf("lets ");
    cur_city=G->croot;
    i=0;
    //printf("cur_idx =%d",cur_idx);
    while(i<cur_idx){
        //printf("go ");
        if(cur_city==NULL){
            //printf("invalid index to city: %s",city);
            break;
            //return neighbours;
        }
        city_prev=cur_city;
        cur_city=cur_city->next_city;
        i+=1;
    }
    //printf("entering\n");
    if(G->croot==NULL){
        city_tmp=(CITY*)malloc(sizeof(CITY));
        city_tmp->prev_city=NULL;
        city_tmp->next_city=NULL;
        city_tmp->neighbours=NULL;
        city_tmp->name=(char*)malloc(sizeof(char)*strlen(city));
        strcpy(city_tmp->name,city);
        G->croot=city_tmp;
    }
    else if(cur_city==NULL){
        //printf("\ncur_city is null here\n");
        city_tmp=(CITY*)malloc(sizeof(CITY));
        city_tmp->prev_city=city_prev;
        city_tmp->next_city=NULL;
        city_tmp->neighbours=NULL;
        if(city_prev!=NULL){
            city_prev->next_city=city_tmp;
        }
        city_tmp->name=(char*)malloc(sizeof(char)*strlen(city));
        strcpy(city_tmp->name,city);
    }
    else{
        if(strcmp(cur_city->name,city)==0){//case where the city is already present, but adding new neighbours
            city_tmp=cur_city;
        }
        else{//case where a city is being inserted in the middle
            //printf("\nunbelievable %s!=%s\n",cur_city->name,city);
            city_tmp=(CITY*)malloc(sizeof(CITY));
            city_tmp->prev_city=cur_city;
            city_tmp->next_city=cur_city->next_city;
            cur_city->next_city=city_tmp;
            if(city_tmp->next_city!=NULL){//this condition is redundant, it will always be satisfied, but to be on safer side
                city_tmp->next_city->prev_city=city_tmp;
            }
            city_tmp->neighbours=NULL;
            city_tmp->name=(char*)malloc(sizeof(char)*(strlen(city)));
            strcpy(city_tmp->name,city);
        }
    }
    //printf("calling merge");
    neighbours=MergeSort(G,neighbours);
    //printf("\n came out\n");
    store=neighbours;
    //printf("\nafter sorted\n");
    /*
    while(neighbours!=NULL){
        printf("%s->",neighbours->name);
        neighbours=neighbours->next;
    }
    printf("\n");
    */
    neighbours=store;
    tmp1=city_tmp->neighbours;
    tmp2=NULL;
    cur_city=G->croot;
    city_prev=NULL;
    while(neighbours!=NULL){
        while((tmp1!=NULL)&&(city_to_index(G,tmp1->name)<city_to_index(G,neighbours->name))){
            tmp2=tmp1;
            tmp1=tmp1->next;
        }
        if(tmp1==NULL){
            //printf("ok");
            tmp1=(NODE*)malloc(sizeof(NODE));
            if(tmp2==NULL){
                city_tmp->neighbours=tmp1;
            }
            else{
                tmp2->next=tmp1;
            }
            tmp1->next=NULL;
            tmp1->traffic = neighbours->traffic;
            tmp1->distance= neighbours->distance;
            tmp1->name=(char*)malloc(sizeof(char)*100);
            //printf("ok ");
            strcpy(tmp1->name,neighbours->name);
            //printf("hmm");
        }
        else if(city_to_index(G,tmp1->name)==city_to_index(G,neighbours->name)){
            tmp1->traffic=neighbours->traffic;
            tmp1->distance=neighbours->distance;
        }
        else{
            new=(NODE*)malloc(sizeof(NODE));
            new->traffic=1;
            new->name=(char*)malloc(sizeof(char)*(strlen(neighbours->name)));
            strcpy(new->name,neighbours->name);
            new->distance=neighbours->distance;
            if(tmp2==NULL){
                city_tmp->neighbours=new;
            }
            else{
                tmp2->next=new;
            }
            new->next=tmp1;
        }
        /* fine till here and there is probably a bug and we can reduce the distance */
        //printf("As expected\n");

        while((cur_city!=NULL)&&(city_to_index(G,cur_city->name)<city_to_index(G,neighbours->name))) {
            city_prev=cur_city;
            cur_city=cur_city->next_city;
        }
        ////printf("hello");
        if((city_prev==NULL)&&(city_to_index(G,cur_city->name)<city_to_index(G,neighbours->name))){
            city_tmp=(CITY*)malloc(sizeof(CITY));
            city_tmp->next_city=G->croot;
            G->croot = city_tmp;
            city_tmp->prev_city=NULL;
            city_tmp->neighbours=NULL;
            city_tmp->name=(char*)malloc(sizeof(char)*30);
            strcpy(city_tmp->name,neighbours->name);
            cur_city=city_tmp;
        }
        else if(cur_city==NULL){
            ////printf("\nok\n");
            city_tmp=(CITY*)malloc(sizeof(CITY));
            city_tmp->next_city=NULL;
            city_tmp->prev_city=city_prev;
            city_prev->next_city=city_tmp;
            city_tmp->name=(char*)malloc(sizeof(char)*30);
            strcpy(city_tmp->name,neighbours->name);
            city_tmp->neighbours=NULL;
            cur_city=city_tmp;
        }

        else if(city_to_index(G,cur_city->name)>city_to_index(G,neighbours->name)){
            //printf("yes you are right\n");
            //cur_city=cur_city->prev_city;
            city_tmp=(CITY*)malloc(sizeof(CITY));
            city_tmp->next_city=cur_city;
            city_tmp->prev_city=cur_city->prev_city;
            cur_city->prev_city=city_tmp;
            if(city_tmp->prev_city!=NULL){
                city_tmp->prev_city->next_city=city_tmp;
            }
            else{
                G->croot=city_tmp;
            }
            /*
            if(city_tmp->next_city!=NULL){
                city_tmp->next_city->prev_city=city_tmp;
            }
            */
            city_tmp->name=(char*)malloc(sizeof(char)*30);
            strcpy(city_tmp->name,neighbours->name);
            city_tmp->neighbours=NULL;
            cur_city=city_tmp;
        }
        new=cur_city->neighbours;
        new_tmp=NULL;
        while((new!=NULL)&&(city_to_index(G,new->name)<cur_idx)){
            new_tmp=new;
            new=new->next;
        }
        if(new==NULL){
            if(new_tmp==NULL){
                //cur_city->neighbours=neighbours;//possible bug here
                ////printf("fine");
                cur_city->neighbours=(NODE*)malloc(sizeof(NODE));
                cur_city->neighbours->distance=neighbours->distance;
                cur_city->neighbours->traffic=neighbours->traffic;
                cur_city->neighbours->name=(char*)malloc(sizeof(char)*14);
                strcpy(cur_city->neighbours->name,city);
                cur_city->neighbours->next=NULL;
            }
            else{
                ////printf("\nYou are fine\n");
                //new_tmp->next=neighbours;here
                new_tmp->next=(NODE*)malloc(sizeof(NODE));
                new_tmp->next->distance=neighbours->distance;
                new_tmp->next->traffic=neighbours->traffic;
                new_tmp->next->name=(char*)malloc(sizeof(char)*14);
                strcpy(new_tmp->next->name,city);
                new_tmp->next->next=NULL;
                ////printf("After addition\n");
                new_tmp=cur_city->neighbours;
                while(new_tmp!=NULL){
                    ////printf("%s,%d ",new_tmp->name,new_tmp->distance);
                    new_tmp=new_tmp->next;
                }
                ////printf("\n");
            }
        }
        else if(city_to_index(G,new->name)==cur_idx){
            new->traffic=neighbours->traffic;
            new->distance=neighbours->distance;
        }
        else{
            new_tmp->next=(NODE*)malloc(sizeof(NODE));
            new_tmp->next->distance=neighbours->distance;
            new_tmp->next->traffic=neighbours->traffic;
            new_tmp->next->name=(char*)malloc(sizeof(char)*14);
            strcpy(new_tmp->next->name,city);
            new_tmp->next->next=new;
        }
        neighbours=neighbours->next;
    }
    city_tmp=G->croot;
    ////printf("after %s\n",city);
    while(city_tmp!=NULL){
        new_tmp=city_tmp->neighbours;
        ////printf("%s:",city_tmp->name);
        while(new_tmp!=NULL){
            ////printf(" (%s,%d),",new_tmp->name,new_tmp->distance);
            new_tmp=new_tmp->next;
        }
        city_tmp=city_tmp->next_city;
        ////printf("\n");
    }
    return store;
}
