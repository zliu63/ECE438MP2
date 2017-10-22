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

int GetStatusNumber(Node *head, Seg_Status status){
    if(head == NULL){
        return 0;
    }
    else{
        int num = 0;
        Node *p;
        p = head;
        while(p != NULL){
            if( p->status == status )
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

TCP_Seg GetSeg(Node *head, int seq){
    Node node;
    node = GetNode(head, seq);
    return node.seg;
}

Seg_Status GetStatus(Node *head, int seq){
    Node node;
    node = GetNode(head, seq);
    return node.status;
}

int SetNodeStatus(Node *head, int seq, Seg_Status status){
    Node node;
    if(head == NULL){
        return -1;
    }
    else{
        Node *p = head;
        while(p != NULL){
            if(p->seg.SEQ == seq){
                p->status = status;
                return 0;
            }
            p = p->next;
        }
        return -1;
    }
}

int RemoveNode(Node **head, int seq){
    if((*head) == NULL)
        return -1;
    else{
        if( (*head)->seg.SEQ == seq ){
            Node *p = (*head);
            (*head) = (*head) -> next;
            free(p);
            return 0;
        }
        else{
            Node *p = (*head);
            while( (p->next) != NULL ){
                if(p->next->seg.SEQ == seq){
                    Node *pp;
                    pp = p->next;
                    p->next = p->next->next;
                    free(pp);
                    return 0;
                }
                p = p->next;
            }
            return -1;
        }
    }
}
