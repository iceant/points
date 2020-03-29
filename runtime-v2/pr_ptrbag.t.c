//
// Created by pchen on 12/30/18.
//

#include "pr_ptrbag.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct Graph Graph;

typedef struct Node{
    char* d_name_p;
    pr_ptrbag_t* d_edges_p;
}Node;

typedef struct Edge{
    struct Node * d_from_p;
    struct Node * d_to_p;
    double d_weight;
}Edge;

struct Graph{
    pr_ptrbag_t* d_nodes_p;
    pr_ptrbag_t* d_edges_p;
};

//////////////////////////////////////////////////////////////////////////////
Node* Node_new(const char* name){
    Node* node = malloc(sizeof(*node));
    assert(node);
    node->d_edges_p = pr_ptrbag_new();
    if(name){
        node->d_name_p = malloc(strlen(name)+1);
        strcpy(node->d_name_p, name);
    }else{
        node->d_name_p = NULL;
    }
    return node;
}

void Node_delete(Node* node){
    assert(node);
    assert(node->d_edges_p);
    printf("delete node(%s)\n", node->d_name_p);
    pr_ptrbag_delete(&node->d_edges_p);
    assert(node->d_edges_p==NULL);
    free(node->d_name_p);
    free(node);
}

#define Node_SETEDGE(node, edge)\
    pr_ptrbag_add((node)->d_edges_p, (edge), NULL);
//////////////////////////////////////////////////////////////////////////////
void Edge_delete(Edge* edge){
    assert(edge);
    printf("delete edge[%s --(%f)-->%s]\n", edge->d_from_p->d_name_p, edge->d_weight, edge->d_to_p->d_name_p);
    free(edge);
}
//////////////////////////////////////////////////////////////////////////////

Graph* Graph_new(){
    Graph* p = malloc(sizeof(*p));
    assert(p);
    p->d_edges_p = pr_ptrbag_new();
    p->d_nodes_p = pr_ptrbag_new();
    return p;
}

void Graph_delete(Graph** graph){
    assert(graph);
    assert(*graph);
    Graph* graph_p = * graph;
    pr_ptrbag_delete(&(graph_p)->d_edges_p);
    pr_ptrbag_delete(&(graph_p)->d_nodes_p);
    assert((graph_p)->d_edges_p==NULL);
    assert((graph_p)->d_nodes_p==NULL);
    free(*graph);
    *graph = NULL;
}

Node* Graph_addNode(Graph * graph, const char* nodeName){
    assert(graph);
    Node * node = Node_new(nodeName);
    pr_ptrbag_add(graph->d_nodes_p, node, (pr_ptrbag_release_proc)Node_delete);
    printf("Graph_addNode(%s)\n", nodeName);
    return node;
}

Node* Graph_findNode(Graph* graph, const char* nodeName){
    assert(graph);
    assert(nodeName);
    pr_ptrbag_iter_t * iter = pr_ptrbag_iter_new(graph->d_nodes_p);
    Node* node_p = NULL;
    while(pr_ptrbag_iter_hasnext(iter)){
        Node * node = (Node*)pr_ptrbag_iter_get(iter);
        if(strcmp(node->d_name_p, nodeName)==0){
            node_p =  node;
            break;
        }
        pr_ptrbag_iter_next(iter);
    }
    pr_ptrbag_iter_delete(&iter);

    return node_p;
}

void Graph_removeNode(Graph* graph, Node* node){
    assert(graph);
    assert(node);
    pr_ptrbag_manip_t * manip = pr_ptrbag_manip_new(node->d_edges_p);
    while(pr_ptrbag_manip_hasnext(manip)){
        Edge* edge = pr_ptrbag_manip_get(manip);
        pr_ptrbag_remove(graph->d_edges_p, edge);
        pr_ptrbag_manip_remove(manip);
    }
    pr_ptrbag_manip_delete(&manip);
    pr_ptrbag_remove(graph->d_nodes_p, node);
}

Edge* Graph_addEdge(Graph * graph, Node * from, Node * to, double weight){
    assert(graph);
    assert(from);
    assert(to);
    Edge * p = malloc(sizeof(*p));
    assert(p);
    p->d_from_p = from;
    p->d_to_p = to;
    p->d_weight = weight;
    pr_ptrbag_add(graph->d_edges_p, p, (pr_ptrbag_release_proc)Edge_delete);
    Node_SETEDGE(from, p);
    Node_SETEDGE(to, p);
    printf("Graph_addEdge(%s, %s, %f)\n", from->d_name_p, to->d_name_p, weight);
    return p;
}

Edge* Graph_findEdge(Graph * graph, Node* from, Node* to){
    pr_ptrbag_iter_t *iter = pr_ptrbag_iter_new(graph->d_edges_p);
    Edge * ptr = NULL;
    while(pr_ptrbag_iter_hasnext(iter)){
        Edge* edge = (Edge*)pr_ptrbag_iter_get(iter);
        if(edge->d_from_p==from && edge->d_to_p == to){
            ptr =  edge;
            break;
        }
        pr_ptrbag_iter_next(iter);
    }
    pr_ptrbag_iter_delete(&iter);
    return ptr;
}

void Graph_removeEdge(Graph * graph, Edge* edge){
    pr_ptrbag_remove(graph->d_edges_p, edge);
}
//////////////////////////////////////////////////////////////////////////////
void Graph_print(Graph* graph){
    printf("Graph:\n");
    pr_ptrbag_iter_t *nit = pr_ptrbag_iter_new(graph->d_nodes_p);
    if(pr_ptrbag_iter_hasnext(nit)){
        printf("  Nodes:\n");
    }
    for(;pr_ptrbag_iter_hasnext(nit);pr_ptrbag_iter_next(nit)){
        Node* node = (Node*)pr_ptrbag_iter_get(nit);
        printf("  [%s]", node->d_name_p);
    }
    pr_ptrbag_iter_delete(&nit);

    pr_ptrbag_iter_t *eit = pr_ptrbag_iter_new(graph->d_edges_p);
    const char* p = "\n  Edges:  %s --(%f)-->%s";
    const char* q = "\n          %s --(%f)-->%s";
    for(;pr_ptrbag_iter_hasnext(eit); pr_ptrbag_iter_next(eit)){
        Edge * edge = (Edge*)pr_ptrbag_iter_get(eit);
        printf(p, edge->d_from_p->d_name_p, edge->d_weight, edge->d_to_p->d_name_p);
        p = q;
    }
    pr_ptrbag_iter_delete(&eit);

    printf("\nEnd Graph\n");
}

//////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv){
    Graph * graph_p = Graph_new();

    Node* n1 = Graph_addNode(graph_p, "Mindy");
    Node* n2 = Graph_addNode(graph_p, "Susan");
    Node* n3 = Graph_addNode(graph_p, "Rick");

    Graph_addEdge(graph_p, n2, n1, 4);
    Graph_addEdge(graph_p, n1, n3, 5);
    Graph_addEdge(graph_p, n3, n2, 1);

    Graph_addNode(graph_p, "Franklin");
    Graph_addNode(graph_p, "Cathy");

    Graph_addEdge(graph_p, Graph_findNode(graph_p, "Susan"), Graph_findNode(graph_p, "Franklin"), 6);
    Graph_addEdge(graph_p, Graph_findNode(graph_p, "Rick"), Graph_findNode(graph_p, "Franklin"), 2);

    Graph_addEdge(graph_p, Graph_findNode(graph_p, "Rick"), Graph_findNode(graph_p, "Cathy"), 3);

    Graph_print(graph_p);

    Graph_removeNode(graph_p, n3);
//    Graph_removeEdge(graph_p, Graph_findEdge(graph_p, n3, n2));
    printf("After remove node 'Rick'\n");
    Graph_print(graph_p);

    Graph_delete(&graph_p);
}