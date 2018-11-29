typedef struct city_db{
    //this structure holds individual city's name and it's robinkarp value,
    //so that it can be used in future.
    //and the length of the city name cannot be more than
    char name[100];
}CITY_DB;
typedef struct index{
    CITY_DB *array;//A 1D array to keep track of index to city mappings
}INDEX_ARRAY;
typedef struct tree_node{
    struct tree_node *next[26];
    //pointer to the next node for the range 'a'-'z' or 'A'-'Z'.
    //note that there are only 26 pointers. the tree is case insensitive.
    //bangalore, BANGALORE and BAngAloRe ---> all mean the same.
    //NULL terminated
    int idx;
    //if this is a leaf to any of the city, it's index will be stored in idx. else it will be -1.
    //this uses implicit suffix tree.
}TREE_NODE;
typedef struct trie{
    TREE_NODE *root;
}TRIE;
typedef struct node{
    int traffic;//the density of traffic at this point
    char *name;//name of the city under consideration
    int distance;//to reach this city from the city in prospective
    struct node* next;
}NODE;
typedef struct city{
    NODE* neighbours; //pointer to the first node of it's neighbours. NULL terminated.
    char *name;//name of the current city
    struct city* next_city;
    struct city* prev_city;
}CITY;
typedef struct graph{
    CITY *croot;//pointer to the first city in the graph. NULL terminated.Used for dijkstra algo.
    TRIE *t;//pointer to TRIE. used for city_to_index mapping
    INDEX_ARRAY *idx_arr;//pointer to the only node of INDEX_ARRAY. used for index_to_city mapping
    int no_of_cities;//count of total cities in the graph.
}GRAPH;

char* index_to_city(GRAPH* G,int idx);
//returns the corresponding city pointed to by the index.

int city_to_index(GRAPH* G,char* city);
//returns the corresponding index for the given cities. This will make use of TRIE.

int add_city(GRAPH* G,char* city);
//this function should add the 'city' to the TRIE. so it has to update both TRIE and INDEX_ARRAY.
//and return the index of the city in the INDEX_ARRAY. If the city name already exists, it should
//just return the index of the city, need not add to TRIE

NODE* insert(GRAPH* G,char *city,NODE* neighbours);
/*insert function will insert the city to the graph. it has to add the
neighbours of the city to the graph, neighbours will contain NULL terminated linked list
of it's neighbours. Also it has to update the neighbours city to point to this city as well*/

CITY* shortest_path(GRAPH* G,char* src,char* dst,NODE* remove);
/*shortest_distance will return the path of the current shortest path between the cities src and dst.
it should be NULL terminated.the last node dst in the returned list.
This function will make use of city_to_index and index_to_city function to map between cities and index.
As decided this function has to use dijkstra algo.*/
