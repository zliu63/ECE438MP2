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
    struct Node_t *next;
};

typedef struct Node_t Node;

//add a node to the end of the list
void AddNode(Node **head, TCP_Seg seg, Seg_Status status);

//get total number of nodes in the list
int GetNumber(Node *head);

//get number of nodes in a specific status
int GetStatusNumber(Node *head, Seg_Status status);

//find a node using its seq#.
//if the seq does not exist in the list, return a node whost status = NONEXIST
Node GetNode(Node *head, int seq);

//find a segment using seq#
//if the seq does not exist in the list, return a seg whost status = NONEXIST
TCP_Seg GetSeg(Node *head, int seq);

//fine the status of a node using seq#
//if the seq does not exist in the list, return status = NONEXIST
Seg_Status GetStatus(Node *head, int seq);

//set the status of a node with certain seq#
//return -1 if node not exist; return 0 if success
int SetNodeStatus(Node *head, int seq, Seg_Status status);

//remove a node with certain seq#
int RemoveNode(Node **head, int seq);

#endif /* List_h */
