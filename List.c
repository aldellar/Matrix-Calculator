/****************************************************************************************
*  Andrew Dell'Aringa 2082835
*  PA4 02/07/24
*  List.c conatins the function definitions and listOBJ and nodeOBJ ADT's
*****************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "List.h"

//private types
typedef struct NodeObj{
    void* data;
    struct NodeObj* next;
    struct NodeObj* prev;
}NodeObj;

typedef NodeObj* Node;

typedef struct ListObj{
    Node front;
    Node back;
    Node cursor;
    int index;
    int length;
} ListObj;

// Constructors-Destructors ---------------------------------------------------

//constructor for Node object
//Initializes next, prev, and data fields.
Node newNode(void* data){
    Node N = malloc(sizeof(NodeObj));
    assert(N!=NULL);
    N->next = N->prev = NULL;
    N->data = data;
    return(N);
}

//frees memory pointed to by *pN and sets *pN to NULL.
void freeNode(Node* pN) {
    if(pN!=NULL && *pN!=NULL){
        free(*pN);
        *pN=NULL;
    }
}

//constructor for newList object
//Initializes front, back, cursor, index, and length fields.
List newList(void) {
    List L = malloc(sizeof(ListObj));
    assert(L!=NULL);
    L->front = L->back = L->cursor = NULL;
    L->index = -1;
    L->length = 0;
    return(L);
}

//empties list, frees memory pointed to by *pL and sets *pL to NULL.
void freeList(List* pL) {
    if(pL!=NULL && *pL!=NULL){
        while(length(*pL) != 0){
            deleteFront(*pL);
        }
    }
    free(*pL);
    *pL = NULL;
}

// Access functions -----------------------------------------------------------

//returns length of L.
int length(List L) {
    if (L==NULL){
        printf("List Error: calling length() on NULL list reference.\n");
       	return -1;
    }
    return(L->length);
}

//returns index of cursor.
int index(List L) {
    if (L==NULL){
        printf("List Error: calling index() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    return(L->index);
}

//returns front element of L.
void* front(List L) {
    if (L==NULL){
        printf("List Error: calling front() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    return(L->front->data);
}

//returns back element of L.
void* back(List L) {
    if (L==NULL){
        printf("List Error: calling back() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    return(L->back->data);
}

//returns cursor element of L.
void* get(List L) {
    if (L==NULL){
        printf("List Error: calling get() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    if(L->cursor == NULL){
	return NULL;
    }

    return(L->cursor->data);
}



// Manipulation procedures ----------------------------------------------------

//resets L to its original empty state.
void clear(List L) {
    if (L==NULL){
        printf("List Error: calling clear() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    while(L->length > 0){
        deleteFront(L);
    }
}

//overwrites the cursor element's data with x.
void set(List L, void* x) {
    if (L==NULL){
        printf("List Error: calling set() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    L->cursor->data = x;
}

//moves cursor to the front of the list.
void moveFront(List L) {
    if (L==NULL){
        printf("List Error: calling moveFront() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    L->cursor = L->front;
    if(L->length==0){
        L->index = -1;
    } else {
        L->index = 0;
    }
}

//moves cursor to the back of the list.
void moveBack(List L) {
    if (L==NULL){
        printf("List Error: calling moveBack() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    L->cursor = L->back;
    if(L->length==0){
        L->index = -1;
    } 
    else {
        L->index = L->length-1;
    }
}

//moves cursor to the prev element in the list.
void movePrev(List L) {
    if (L==NULL){
        printf("List Error: calling movePrev() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL){
        if (L->cursor == L->front){ //cursor at front so cursor falls off
            L->cursor = NULL;
            L->index = -1;
        } else {
            L->cursor = L->cursor->prev;
            L->index--;
        }
    } //else do nothing
}

//moves cursor to the next element in the list.
void moveNext(List L) {
    if (L==NULL){
        printf("List Error: calling moveNext() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL){
        if (L->cursor == L->back){ //cursor at back, so cursor falls off
            L->cursor = NULL;
            L->index = -1;
        } else {
            L->cursor = L->cursor->next;
            L->index++;
        }
    } //else do nothing
}

//adds element containing x to the front of the list.
void prepend(List L, void* x) { //NEEDS
    if (L==NULL){
        printf("List Error: calling prepend() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    Node newElement = newNode(x);
    if (L->length == 0){ //empty list
        L->front = newElement;
        L->back = newElement;
        L->length = 1;
    } else { //replace front element with new, front points to new
        L->front->prev = newElement;
        newElement->next = L->front;
        L->front = newElement;
        L->length++;
        if (L->index > -1){
            L->index++;
        }
    }
}

//adds element containing x to the end of the list.
void append(List L, void* x)  {
    if (L==NULL){
        printf("List Error: calling append() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    Node newElement = newNode(x);
    if (L->length == 0){ //empty list
        L->front = newElement;
        L->back = newElement;
        L->length = 1;
    } else { //replace back element with new, back points to new
        L->back->next = newElement;
        newElement->prev = L->back;
        L->back = newElement;
        L->length++;
    }

}

//inserts a element containing x before the cursor.
void insertBefore(List L, void* x){
    if (L==NULL){
        printf("List Error: calling insertBefore() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0 || L->index == -1){
        printf("List Error: calling insertBefore() from out of cursor's range.\n");
        exit(EXIT_FAILURE);
    }
    if (L->index==0) {
        prepend(L, x);
    } else {
        Node newElement = newNode(x);
        newElement->prev = L->cursor->prev;
        L->cursor->prev->next = newElement;
        L->cursor->prev = newElement;
        newElement->next = L->cursor;
        L->length++;
        L->index++;
    }
    
}

//inserts element containing x after the cursor.
void insertAfter(List L, void* x){
    if (L==NULL){
        printf("List Error: calling insertAfter() on NULL list reference.\n");
        exit(EXIT_FAILURE);
    }
    if (L->length == 0 || L->index == -1){
        printf("List Error: calling insertAfter() from out of cursor's range.\n");
        exit(EXIT_FAILURE);
    }
    if (L->index==(L->length)-1) {
        append(L, x);
    } else {
        Node newElement = newNode(x);
        newElement->next = L->cursor->next;
        L->cursor->next->prev = newElement;
        L->cursor->next = newElement;
        newElement->prev = L->cursor;
        L->length++;
    }
}

//deletes the front element of the list.
void deleteFront(List L) {
	if(L->length == 0){
    	fprintf(stderr, "trying to delete from an empty list");
    }
    if(L->length > 0){
		if(L->length == 1){
			free(L->front);
			L->front = NULL;
			L->back = NULL;
			L->length--;
			return;
		}
		
		//Get the address of the node after the front;
		Node next = L->front->next;
		//Delete the front
		free(L->front);
		//Make the next the new front
		L->front = next;
		//Clear the next's prev pointer
		next->prev = NULL;
		//Decrement the List length
		L->length--;
		
		if(index(L) == 0){
			L->index = -1;
			L->cursor = NULL;
			return;
		}
		//Decrement the index 
		L->index--;
    }

}

//deletes the back element of the list.
void deleteBack(List L) {
   if(L->length == 0){
    	fprintf(stderr, "trying to delete from an empty list");
    }
   if(L->length > 0){
   		
		if(L->length == 1){
			
			free(L->front);
			L->front = NULL;
			L->back = NULL;
			L->length--;
			return;
		}
		if( get(L) == L->back->data){
			L->index = -1;
			L->cursor = NULL;
			//set the node that was pointing to the back to null;
			Node prev = L->back->prev;
			prev->next = NULL;
			//delete the back;
			free(L->back);
			//make the prev node the new back
			L->back = prev;
			//Decrement the List Length 
			L->length--;
			return;
		}
		
		//set the node that was pointing to the back to null;
		Node prev = L->back->prev;
		prev->next = NULL;
		//delete the back;
		free(L->back);
		//make the prev node the new back
		L->back = prev;
		//Decrement the List Length 
		L->length--;
		
		
		
	}

}

//delete cursor element, making cursor undefined
void delete(List L) {
    if(L->length == 0){
    	
    	fprintf(stderr, "trying to delete from an empty list");
    }
    if(index(L) == 0){
    		
 		//set the front pointer to the cursors next
 		L->front = L->cursor->next;
 		
 		//set the cursors next's prev pointer to null
 		L->front->prev = NULL;
 		
 		//free the cursor data
 	
 		free(L->cursor);
 		
 		//make the cursor undefined
 		L->cursor = NULL;
 		
 		//change the index to -1
 		L->index = -1;
 		
 		//decrement the length of the list
 		L->length--;
 		
 		
 		
 		return;
 	}
 	if(index(L) == L->length -1){
 		
 		//set cursor's prev's next pointer to null
 		L->cursor->prev->next = NULL;
 		
 		//set the back to cursor's prev
 		L->back = L->cursor->prev;
 		//free the cursor data
 		free(L->cursor);
 		
 		//make the cursor undefined
 		L->cursor = NULL;
 		
 		//change the index to -1
 		L->index = -1;
 		
 		//decrement the length of the list
 		L->length--;
 		
 		return;
 	}
 	
 	//get the node after the cursor
 	Node next = L->cursor->next;
 	
 	//get the node before the cursor
 	Node prev = L->cursor->prev;
 	
 	//point the next's prev to the node prev to the cursor
 	next->prev = prev;
 	
 	//point the prev's next to the node after the cursor
 	prev->next = next;
 	
 	//delete the cursor
 	free(L->cursor);
 	
 	//set the cursor to NULL as it is now undefined
 	L->cursor = NULL;
 	
 	//change the index to -1
 	L->index = -1;
 		
 	//decrement the length of the list
 	L->length--;

}


// Other operations -----------------------------------------------------------


