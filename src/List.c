//
//  List.c
//  
//
//  Created by Liuyi Shi on 10/20/17.
//

#include "List.h"

void AddNode(Node **head, TCP_Seg seg, Seg_Status status){
    Node *node;
    node = malloc(sizeof(Node));
    node->seg = seg;
    node->status = status;
    node->next = NULL;
    
    if( (*head) == NULL){
        (*head) = node;
    }
    else{
        Node *p;
        p = (*head) ;
        while(p->next != NULL)
            p = p->next;
        p->next = node;
    }
}

int GetNumber(Node *head){
    if(head == NULL){
        return 0;
    }
    else{
        int num = 0;
        Node *p;
        p = head;
        while(p != NULL){
            num++;
            p = p->next;
        }
        return num;
    }
}

Node GetNode(Node * head, int seq){
    Node node;
    if(head == NULL){
        node.status = NOTEXIST;
        return node;
    }
    else{
        Node *p = head;
        while(p != NULL){
            if(p->seg.SEQ == seq){
                node = *p;
                return node;
            }
            p = p->next;
        }
        node.status = NOTEXIST;
        return node;
    }
        
}
