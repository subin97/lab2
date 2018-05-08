/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32164640 /32162045 /32161391
*	    Student name : 최수빈 /백예은   /노유진
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

pthread_mutex_t lock;

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree) {
    // You need to implement lab2_node_print_inorder function.
	if (tree->root == NULL) {          //트리가 비었을 때
		printf("tree is empty");
		return 0;                   //실패
	}	 

	return inorder(tree->root);//중위순회 성공시 1 return
}

int inorder(lab2_node *now) {

	if (now == NULL)
		return 0;

	inorder(now->left);
	printf("%d ", now->key);
	inorder(now->right);
	return 1;//순회 마치면 1 return
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // You need to implement lab2_tree_create function.
    lab2_tree *tree=(lab2_tree*)malloc(sizeof(lab2_tree));
    tree->root=NULL;
    return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
    lab2_node *now=(lab2_node *)malloc(sizeof(lab2_node));
    now->key=key;
    now->left=now->right=NULL;
    return now;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.
    int gap=0;
    if(tree->root==NULL){
        tree->root=new_node;
        return 1;
    }
    lab2_node *now=tree->root;
    while(1){
        gap=now->key-new_node->key;
        if(gap==0){
            return 0;
        }
        if(gap>0){
            if(now->left){
                now=now->left;
            }
            else{
                now->left=new_node;
                return 1;
            }
        }
        else{
            if(now->right){
                now=now->right;
            }
            else{
                now->right=new_node;
                return 1;
            }
        }
    }
    return 1;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
      // You need to implement lab2_node_insert_fg function.
    int gap=0;
    if(tree->root==NULL){
        tree->root=new_node;
        return 1;
     }
    lab2_node *now=tree->root;
    
    while(1){
        gap=now->key-new_node->key;
        if(gap==0) return 0;
        if(gap>0){
            if(now->left) now=now->left;
            else{    
                now->left=new_node;
                return 1;  
            }
        }
        else{              
            if(now->right){now=now->right;} 
            else{  
                now->right=new_node;  
                return 1; 
            }
        }
     }
      return 1;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function.
    pthread_mutex_lock(&lock);
    int gap=0;
    if(tree->root==NULL){
        tree->root=new_node;
        pthread_mutex_unlock(&lock);
        return 1;
    }
    lab2_node *now=tree->root;

    while(1){
        gap=now->key-new_node->key;
        if(gap==0){
            pthread_mutex_unlock(&lock);
            return 0;}
        if(gap>0){
            if(now->left){
                now=now->left;
            }
            else{
                now->left=new_node;
                pthread_mutex_unlock(&lock);
                return 1;
            }
        }
        else{
            if(now->right){
                now=now->right;
            }
            else{
                now->right=new_node;
                pthread_mutex_unlock(&lock);
                return 1;
            }
        }
    }
    pthread_mutex_unlock(&lock);
    return 1;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove function.
	lab2_node *p = tree->root;//
	lab2_node *q = p;

	while (p) {
		if (key == p->key)//삭제할 key가 존재하면 빠져나옴
			break;
		else if (key < p->key) {
			q = p;
			p = p->left;
		}
		else {
			q = p;
			p = p->right;
		}
	}

	if (!p) {
		printf("no vlaue for removing\n");
		return 0;
	}

	if (p->left || p->right) {//하나이상의 자식을 가진 경우

		if (p->left && p->right) {
			lab2_node *t = p;
			q = p;
			p = p->right;
			for (; p->left; p = p->left) {
				q = p;

				if (p->right) {
					if (t == q) {
						t->key = p->key;
						q->right = p->right;
					}

					else {
						t->key = p->key;
						q->left = p->right;
					}
				}

				else {
					t->key = p->key;
					t->left = p->left;
				}
			}
		}

		else {//자식이 하나일 경우
			if (p->key < q->key) {
				if (p->left)
					q->left = p->left;
				else
					q->left = p->right;
			}

			else {
				if (p->left)
					q->right = p->left;
				else
					q->right = p->right;
			}
		}
	}

	else {
		if (p == tree->root)//root노드를 삭제할 경우
			tree->root = 0;

		else if (p->key < q->key)
			q->left = 0;

		else
			q->right = 0;
	}

	lab2_node_delete(p);
	printf("deleting complete\n");
	return 1;

}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_fg function.
        // You need to implement lab2_node_remove function.
	lab2_node *p = tree->root;//
	lab2_node *q = p;

	while (p) {
		if (key == p->key)//삭제할 key가 존재하면 빠져나옴
			break;
		else if (key < p->key) {
			q = p;
			p = p->left;
		}
		else {
			q = p;
			p = p->right;
		}
	}

	if (!p) {
		printf("no vlaue for removing\n");
		return 0;
	}

	if (p->left || p->right) {//하나이상의 자식을 가진 경우

		if (p->left && p->right) {
			lab2_node *t = p;
			q = p;
			p = p->right;
			for (; p->left; p = p->left) {
				q = p;

				if (p->right) {
					if (t == q) {
						t->key = p->key;
						q->right = p->right;
					}

					else {
						t->key = p->key;
						q->left = p->right;
					}
				}

				else {
					t->key = p->key;
					t->left = p->left;
				}
			}
		}

		else {//자식이 하나일 경우
			if (p->key < q->key) {
				if (p->left)
					q->left = p->left;
				else
					q->left = p->right;
			}

			else {
				if (p->left)
					q->right = p->left;
				else
					q->right = p->right;
			}
		}
	}

	else {
		if (p == tree->root)//root노드를 삭제할 경우
			tree->root = 0;

		else if (p->key < q->key)
			q->left = 0;

		else
			q->right = 0;
	}

	lab2_node_delete(p);
	printf("deleting complete\n");
	return 1;


}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_cg function.
    pthread_mutex_lock(&lock);
	lab2_node *p = tree->root;//
	lab2_node *q = p;

	while (p) {
		if (key == p->key)//삭제할 key가 존재하면 빠져나옴
			break;
		else if (key < p->key) {
			q = p;
			p = p->left;
		}
		else {
			q = p;
			p = p->right;
		}
	}

	if (!p) {
		printf("no vlaue for removing\n");
        pthread_mutex_unlock(&lock);
		return 0;
	}

	if (p->left || p->right) {//하나이상의 자식을 가진 경우

		if (p->left && p->right) {
			lab2_node *t = p;
			q = p;
			p = p->right;
			for (; p->left; p = p->left) {
				q = p;

				if (p->right) {
					if (t == q) {
						t->key = p->key;
						q->right = p->right;
					}

					else {
						t->key = p->key;
						q->left = p->right;
					}
				}

				else {
					t->key = p->key;
					t->left = p->left;
				}
			}
		}

		else {//자식이 하나일 경우
			if (p->key < q->key) {
				if (p->left)
					q->left = p->left;
				else
					q->left = p->right;
			}

			else {
				if (p->left)
					q->right = p->left;
				else
					q->right = p->right;
			}
		}
	}

	else {
		if (p == tree->root)//root노드를 삭제할 경우
			tree->root = 0;

		else if (p->key < q->key)
			q->left = 0;

		else
			q->right = 0;
	}

	lab2_node_delete(p);
	printf("deleting complete\n");
    pthread_mutex_unlock(&lock);
	return 1;


}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // You need to implement lab2_tree_delete function.
	free(tree);    
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // You need to implement lab2_node_delete function.
    free(node);
}

