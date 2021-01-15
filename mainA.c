#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>

typedef struct graph Graph;
typedef struct node Node;
typedef struct heap Heap;
const double INF = 99999999;

struct node
{
    Node *next;
    int vertex_number;
    double distance;
    double weight;
};

//Structs to represent Graph and 
struct graph {
    Node entries[10000000];
    int nodeCount;
};

struct heap{
    Node entries[10000000];
    int heapSize;
    int *mapping;
};

//HEAP FUNCTIONS START ***********************************************************

/****************************************************************/
/* MIN HEAP FUNCTIONS */
/****************************************************************/

int is_heap_empty(Heap* heap)
{
    if(heap->heapSize == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

//ensure that mapping is true
int mapping_control(Heap * heap)
{
    int number_of_true = 0;
    for(int i = 1; i < heap->heapSize + 1; i++)
    {
        //printf("Vertex %d is placed in heap location %d and mapped to %d\n", heap->entries[i].vertex_number, i, heap->mapping[heap->entries[i].vertex_number]);
        if(i != heap->mapping[heap->entries[i].vertex_number])
        {
            printf("--------------------Wrong mapping found-------------\n");
            number_of_true++;
        }
    }
    return 1;
}

int decrease_key_min_heap(Heap *heap, int which_vertex , double key)
{
    int i = heap->mapping[which_vertex];
    //printf("Vertex %d is placed in heap location: %d\n", which_vertex, i);
    int save = i;
    int save_vertex_number = which_vertex;
    //printf("i is: %d\n", i);
    //printf("Inside Decrease Key**************\n");
    //printf("++++++++++In decrease key: Vertex %d, placed in Heap Location: %d\n", which_vertex, i);
    
    if(key > heap->entries[i].distance)
    {
        //printf("can be problem *-*-*-*-*-*-*-\n");
        return 0;
    }

    while((i > 1) && heap->entries[i / 2].distance > key)
    {
        //update mapping
        //where is that vertex
        //printf(",,,,,,,,,Map: %d u %d ye yani place %d ü place %d e maple\n", heap->entries[i / 2].vertex_number, heap->entries[i].vertex_number, i/2, i);
        heap->mapping[heap->entries[i/2].vertex_number] = i; //sequential mapping

        heap->entries[i].distance = heap->entries[i / 2].distance;
        heap->entries[i].vertex_number = heap->entries[i / 2].vertex_number;
        i = i / 2;
    }
    //only one mapping while leaving

    //printf("i is: %d\n", i);
    //printf("Exits What am ı fucking doing in mapping\n");
    //printf(",,,,,,,,,,,Map %d u %d ye\n", which_vertex, i);
    //printf("put %d to %d\n", heap->entries[i].vertex_number, save);
    heap->mapping[which_vertex] = i;

    heap->entries[i].distance = key;
    heap->entries[i].vertex_number = which_vertex;
}

void min_heapify(Heap* heap, int i, int n)
{
    int min = i;
    if (((2 * i) <= n) && (heap->entries[2 * i].distance < heap->entries[i].distance))
    {
        min = 2 * i;
    }
    if (((2*i + 1) <= n) && (heap->entries[2 * i + 1].distance < heap->entries[min].distance))
    {
        min = 2 * i + 1;
    }
    if (min != i)
    {
        //change place of min and i
        //SWAP
        //printf("*******MIN_HEAPIFY*******\n");
        //printf("swap %d and %d\n", heap->entries[i].vertex_number, heap->entries[min].vertex_number);
        int temp_index = heap->entries[i].vertex_number;
        double temp_distance = heap->entries[i].distance;

        heap->entries[i].vertex_number = heap->entries[min].vertex_number;
        heap->entries[i].distance = heap->entries[min].distance;

        heap->entries[min].vertex_number = temp_index;
        heap->entries[min].distance = temp_distance;

        //elements are swapped

        //update mapping
        heap->mapping[heap->entries[i].vertex_number] = i;
        heap->mapping[heap->entries[min].vertex_number] = min;

        min_heapify(heap, min, n);
    }
}

void printHeap(Heap * heap)
{
    int heapSize = heap->heapSize;
    printf("Heap Size: %d\n", heap->heapSize);
    for(int i = 1; i < heap->heapSize + 1; i++)
    {
        printf("Heap entry %d, vertex %d, node distance %f\n", i , heap->entries[i].vertex_number, heap->entries[i].distance);
    }
}

void printMapping(Heap *heap, int vertex_count)
{
    printf("Mapping:\n");
    for(int i = 1; i < vertex_count + 1; i++)
    {
        printf("vertex=%d is placed in %d in heap\n", i, heap->mapping[i]);
    }
}


Node * extract_min_heap(Heap *heap)
{
    Node* min_node = (Node*)malloc(sizeof(Node));
    
    //pass the data of the root
    min_node->vertex_number = heap->entries[1].vertex_number;
    min_node->distance = heap->entries[1].distance;

    //move heap[n] to the root
    heap->entries[1].vertex_number = heap->entries[heap->heapSize].vertex_number;
    heap->entries[1].distance = heap->entries[heap->heapSize].distance;

    heap->entries[heap->heapSize].distance = INF;

    //update needed for mapping
    heap->mapping[min_node->vertex_number] = INF; //returned vertex number is no longer valid
    heap->mapping[heap->entries[1].vertex_number] = 1; // first element is mapped to 1
    heap->heapSize = heap->heapSize - 1;
    min_heapify(heap, 1, heap->heapSize);

    return min_node;
}


/*******************
 * Weight or distance
 * 
 */



Heap * create_empty_heap(int vertex_count)
{
    Heap* heap = (Heap*) malloc(sizeof(Heap));
    heap->mapping = (int*) malloc((vertex_count + 1) * sizeof(int));
    heap->heapSize = vertex_count;
    for(int i = 1; i < heap->heapSize + 1; i++)
    {
        heap->entries[i].vertex_number = i;
        heap->entries[i].distance = 0;
        heap->mapping[i] = i;
    }
    return heap;
}

Heap * update_min_priority_queue(Heap * heap, double *distance)
{
    for(int i = 1; i < heap->heapSize + 1; i++)
    {
        heap->entries[i].distance = distance[i];
    }
    return heap;
}

void remove_heap(Heap *heap)
{
    int* temp = heap->mapping;
    free(temp);
    free(heap);
}

//HEAP FUNCTIONS END ***********************************************************

//GRAPH FUNCTIONS START ***********************************************************

// A utility function to print the adjacency list  
// representation of graph 

void printGraph(Graph *graph) 
{ 
    int totalNode = 0;
    for (int i = 0; i < graph->nodeCount; i++) 
    {
        int edge_count = 0;
        Node *temp = &graph->entries[i];
        printf("Head: ");
        while (temp != NULL) 
        { 
            printf("%d -> ", temp->vertex_number);
            //printf("%d -> (%f) ", temp->vertex_number, temp->weight);
            temp = temp->next; 
            totalNode = totalNode + 1;
            edge_count++;
        } 
        printf("\n");
    } 
    printf("Total #of nodes in graph: %d\n", totalNode);
} 

void removeGraph(Graph* graph) 
{ 
    int removedNodeCount = 0;
    for (int i = 0; i < graph->nodeCount; i++) 
    { 
        Node* temp = &graph->entries[i];
        temp = temp->next; 
        //printf("\n Adjacency list of vertex %d\n head ", i); 
        while (temp != NULL) 
        { 
            Node* del = temp;
            temp = temp->next; 
            removedNodeCount++;
            free(del);
        }
        //printf("\n"); 
    }
    printf("Removed number of nodes are: %d\n", removedNodeCount + (graph->nodeCount));
    free(graph); 
} 

Graph* initializeGraph(Graph *graph)
{
    for(int i = 0; i < graph->nodeCount; i++)
    {
            graph->entries[i].next = NULL;
            graph->entries[i].vertex_number = i + 1;
            graph->entries[i].weight = 0;
    }
    return graph;
}

void addEdge2Graph(Graph *graph, int src_index, int dest_index, double weight)
{
    //Create new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->vertex_number = dest_index;
    newNode->weight = weight;

    //take the source node
    Node * temp = &graph->entries[src_index - 1];

    if(temp == NULL)
    {
        printf("Head is Null\n");
        return;
    }

    //move until next is NULL
    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    //temp->next = NULL
    temp->next = newNode;
}

Graph* obtainGraph(int * values, double *weights, int vertexsize, int edgesize)
{
    Graph* graph = malloc(sizeof(Graph));
    graph->nodeCount = vertexsize;
    graph = initializeGraph(graph);
    int weight_count = 0;
    for(int i = 0;  i < edgesize * 2; i = i + 2)
    {
        addEdge2Graph(graph, values[i], values[i + 1], weights[weight_count]);
        weight_count++;
    }
    return graph;
}

void control_current_situation(Heap *heap, int vertex_count)
{
    printHeap(heap);
}

void printDistances(double * distance, int iterationCount, int vertex_count)
{
    printf("Distances are: \n");
    for(int i = 1; i < vertex_count + 1; i++)
    {
        printf("v: %d d: %f \n", i, distance[i]);
    }
}


/***********
    main function for dijkstra
 ***********/

double * dijkstra(Graph* graph, int src_node_index)
{
    int vertex_count = 0;
    vertex_count = graph->nodeCount;

    //create distance values for
    double *distance = (double *) malloc((vertex_count + 1) * sizeof(double));
    for(int i = 1; i < vertex_count + 1; i++)
    {
        distance[i] = INF;
    }

    //make source node distance = 0
    distance[src_node_index] = 0;
    
    //create empty heap in size of vertex count
    Heap* heap = create_empty_heap(vertex_count);
    Heap* min_priority_queue= update_min_priority_queue(heap, distance); // initialize heap
    int totalNodeCount = 0;
    int iterationCount = 0;
    int number_of_pass = 0;
    //printHeap(min_priority_queue);
    //Node* min_node = extract_min_heap(heap);
    //printf("********Start Heap*********\n");
    //printHeap(min_priority_queue);
    printf("----------LOOP STARTS-----------\n");
    while(!is_heap_empty(min_priority_queue))
    {
        //printf("While loop starts\n");
        //control_current_situation(min_priority_queue, vertex_count);
        //printMapping(heap, vertex_count);
        //printf("Extract Min is called\n");
        Node* u = extract_min_heap(min_priority_queue); //has heap index
        //printHeap(min_priority_queue);
        Node *v = &graph->entries[u->vertex_number - 1];
        totalNodeCount++;
        //printf("ITERATION: %d START\n", iterationCount);
        //move to first edge
        v = v->next; 
        //traverse edges of the vertex
        //printf("Extracted Vertex is: %d\n", u->vertex_number);
        //control_current_situation(min_priority_queue, vertex_count);
        //printMapping(heap, vertex_count);
        while (v != NULL) 
        {
            //printf("Target Edge of the vertex: %d and %d\n", u->vertex_number, v->vertex_number);
            //control_current_situation(min_priority_queue, vertex_count);
            //printMapping(heap, vertex_count);
            if(min_priority_queue->mapping[v->vertex_number] <= min_priority_queue->heapSize) //need to be in the queue
            {
                if(v->weight + distance[u->vertex_number] < distance[v->vertex_number] && distance[u->vertex_number] != INF)
                {
                    distance[v->vertex_number] = v->weight + distance[u->vertex_number];
                    //printf("Decrease Key Called for decrease_key_min_heap(%d, %f)\n", v->vertex_number, distance[v->vertex_number]);
                    decrease_key_min_heap(min_priority_queue, v->vertex_number, distance[v->vertex_number]);
                    /*
                    if(!mapping_control(min_priority_queue))
                    {
                        //printf("Mapping Error found terminated\n");
                        break;
                    }  
                    */
                }
            }
            //control_current_situation(heap, vertex_count);
            //printf("%d -> ", temp->index);
            v = v->next; 
            totalNodeCount++;
        }
        //printf("After last v: ***************************\n");
        //control_current_situation(min_priority_queue, vertex_count);
        //printMapping(heap, vertex_count);
        //printDistances(distance, iterationCount, vertex_count); 
        iterationCount++;
        free(u);
    }
    printf("Total Number iterations: %d\n", iterationCount);
    printf("Total Number of Traverse operations: %d\n", totalNodeCount);
    printf("Dijkstra is terminated\n");
    //remove_heap(min_priority_queue);
    return distance;
}

int main(int argc, char **argv)
{
    int fd;
    int vertexCount;
    int edgeCount;

    if(argc == 2)
    {
        //Read Data
        char *buffer = (char *) malloc(100000000 * sizeof(char));
        int *meta_data = (int *) malloc(10 * sizeof(int));
        int *values = (int *) malloc(100000000 * sizeof(int));
        double *weights = (double *) malloc(100000000 * sizeof(double));
        int size;
        printf("Arguments are valid\n");
        fd = open(argv[1], O_RDONLY);
        /*
        char *c = argv[2];
        printf("Option : %c\n", *c);
        */
        

        if(fd < 0)
        {
            printf("Error while readin the file, Error is: %d\n", fd);
        }
        size = read(fd, buffer, 100000000);
        int valueCount = 0;
        int charOrder = 0;
        char *charCarrier = (char *) malloc(2000 * sizeof(char));
        //Create integer array with these data
        int passTheLine = 0;
        int line_count = 0;
        int meta_data_count = 0;
        int weight_index = 0;
        int which_one = 1;
        for (int i = 0; i < size; ++i)
        {
            if(buffer[i] == '%')
            {
                passTheLine = 1;
            }
            if(passTheLine == 0)
            {
                line_count = line_count + 1;
                if (buffer[i] == '\n' || buffer[i] == ' ')
                {
                    if(meta_data_count <= 2)
                    {
                        meta_data[meta_data_count] = atoi(charCarrier);
                        memset(charCarrier, ' ', 10 * sizeof(char)); //make it empty the filled areas of the array
                        meta_data_count++;
                        charOrder = 0;
                    }
                    else
                    { 
                        if(which_one != 3)
                        {
                            values[valueCount] = atoi(charCarrier);
                            memset(charCarrier, ' ', 10 * sizeof(char)); //make it empty the filled areas of the array
                            valueCount++;
                            charOrder = 0;
                            which_one++;
                        }
                        else
                        {
                            weights[weight_index] = atof(charCarrier);
                            memset(charCarrier, ' ', 10 * sizeof(char)); //make it empty the filled areas of the array
                            weight_index++;
                            charOrder = 0;
                            which_one = 1;
                        }
                    }
                }
                else
                {
                    charCarrier[charOrder] = buffer[i];
                    charOrder++;
                }
            }
            if (passTheLine == 1 && buffer[i] == '\n')
            {
                passTheLine = 0;
            }
        }
        vertexCount = meta_data[0];
        edgeCount = meta_data[2];

        printf("Value Count %d\n", valueCount);
        printf("Weight Count %d\n", weight_index);
        printf("Vertex Count is: %d\n", vertexCount);
        printf("Edge Count is: %d\n", edgeCount);
     
        /*
        for(int i = 0; i < weight_index; i++)
        {
            printf("Weight %d is: %f\n", i + 1, weights[i]);
        }
        */
      

        FILE *fp;

        fp = fopen("a.txt", "w+");
        

        Graph *graph = obtainGraph(values, weights, vertexCount, edgeCount);
        //graph is obtained then free the reading arrays
        free(weights);
        free(meta_data);
        free(charCarrier);
        free(values);
        free(buffer);
        //printGraph(graph);
        int src_node_index = 1;
        double * distance = dijkstra(graph, src_node_index);
        for(int i = 1; i < vertexCount + 1; i++)
        {
            if(distance[i] == INF)
            {
                fprintf(fp, "%d\n", -1);
            }
            else
            {
                fprintf(fp, "%0.8f\n", distance[i]);
            }
        }

        fclose(fp);
        removeGraph(graph);
        free(distance);
        printf("Results written and program is terminated\n");

    }
    else
    {
        printf("Arguments are invalid. Terminated.");
        return 0;
    }
}