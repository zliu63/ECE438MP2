//
//  List.h
//  
//
//  Created by Liuyi Shi on 10/20/17.
//

#ifndef List_h
#define List_h

#include "TCP.h"

//linked list node to store Segments and corespoding status
struct Node_t{
    TCP_Seg seg;
    Seg_Status status;
    struct Node *next;
};

typedef struct Node_t Node;

//add a node to the end of the list
void AddNode(Node **head, TCP_Seg seg, Seg_Status status);

//get total number of nodes in the list
int GetNumber(Node *head);

//find a node using its seq#.
//if the seq does not exist in the list, return a node whost status = NONEXIST
Node GetNode(Node *head, int seq);

int SetStatus(Node *head, );

#endif /* List_h */
