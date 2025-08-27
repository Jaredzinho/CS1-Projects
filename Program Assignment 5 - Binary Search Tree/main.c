/* COP 3502C Assignment 5
This program is written by: Jared Camilo */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Struct for the fans
struct fans {
	char name[31];
	int tickets;
	struct fans* left;
	struct fans* right;
};

// Function prototypes
struct fans* create_node(char* name);
struct fans* purchaseTicket(struct fans* root, char* name, int numTickets);
struct fans* insert(struct fans* root, struct fans* fan);
struct fans* useTicket(struct fans* root, char* name, int usedTickets);
struct fans* findName(struct fans* current_ptr, char* name, int* depth);
float meanTickets(struct fans* current_ptr);
int add(struct fans* current_ptr);
int numnodes(struct fans* root);
void heightBalance(struct fans* root);
int leftDepth(struct fans* root);
int rightDepth(struct fans* root);
int countBefore(struct fans* root, char* name);
struct fans* parent(struct fans* root, struct fans* node);
struct fans* minVal(struct fans* root);
struct fans* maxVal(struct fans* root);
int isLeaf(struct fans* node);
int hasOnlyLeftChild(struct fans* node);
int hasOnlyRightChild(struct fans* node);
struct fans* findNode(struct fans* root, char* name);
struct fans* delete(struct fans* root, char* name);
int main();

// This function will create the node by allocating space, setting its name and tickets, and directions
struct fans* create_node(char* name) {
	struct fans* temp = (struct fans*)malloc(sizeof(struct fans)); 

	if (temp == NULL)
		return NULL;

	strcpy(temp->name, name);
	temp->tickets = 0;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

/* This function works by checking if the fan exists using findName, if they do their ticket count is simply updated and printed
if not the function will create and insert the node, update the tree, then print the fans data */
struct fans* purchaseTicket(struct fans* root, char* name, int numTickets) {
	int depth = 0;
	struct fans* temp = findName(root, name, &depth);

	if (temp != NULL) {
		temp->tickets += numTickets;
		printf("%s %d %d\n", temp->name, temp->tickets, depth);
	}
	else {
		struct fans* newFan = create_node(name);
		newFan->tickets = numTickets;
		root = insert(root, newFan);
		printf("%s %d %d\n", newFan->name, newFan->tickets, depth);
	}
	return root;
}

// This function will use strcmp to determine where to place the fans purchasing tickets in alphabetical order
struct fans* insert(struct fans* root, struct fans* fan) {
	if (root == NULL) {
		return fan;
	}

	if (strcmp(fan->name, root->name) == 0) {
		return root;
	}

	if (strcmp(fan->name, root->name) > 0) {
		if (root->right != NULL) {
			root->right = insert(root->right, fan);
		}
		else {
			root->right = fan;
		}
	}
	else if (strcmp(fan->name, root->name) < 0) {
		if (root->left != NULL) {
			root->left = insert(root->left, fan);
		}
		else {
			root->left = fan;
		}
	}
	return root;
}

/* This function will find the fan using their tickets and update their count
 if their count goes negative or hits zero they will be deleted, otherwise print their updated data */
struct fans* useTicket(struct fans* root, char* name, int usedTickets) {
	int depth = 0;
	struct fans* ticketUser = findName(root, name, &depth);
	
	if (ticketUser == NULL)
		return root;

	ticketUser->tickets -= usedTickets;

	if (ticketUser->tickets <= 0) {
		printf("%s deleted\n", ticketUser->name);
		root = delete(root, name);
	}
	else {
		printf("%s %d %d\n", ticketUser->name, ticketUser->tickets, depth);
	}
	return root;
}

// This function will perform a NULL check then search and compare using strcmp to determine the direction to go in
struct fans* findName(struct fans* current_ptr, char* name, int* depth) {
	if (current_ptr == NULL)
		return NULL;

	if (strcmp(current_ptr->name, name) == 0) 
		return current_ptr;

	// Increment depth at every recursive call moving down
	(*depth)++;

	struct fans* foundName = NULL;

	if (strcmp(current_ptr->name, name) > 0) 
		foundName = findName(current_ptr->left, name, depth);
	else
		foundName =  findName(current_ptr->right, name, depth);

	return foundName;
}

/* This will calculate the average number of tickets per fan in the tree
	it will do this by adding all the nodes up with the add function
	then call the function to count the total (numnodes) then divide the sum by the total*/
float meanTickets(struct fans* current_ptr) {
	int total = add(current_ptr);
	int numberOfNodes = numnodes(current_ptr);

	if (numberOfNodes == 0) {
		return 0.0;
	}

	return (float)total / (float)numberOfNodes;;
}

// Helper for meantickets
int add(struct fans* current_ptr) {
	if (current_ptr != NULL)
		return current_ptr->tickets + add(current_ptr->left) + add(current_ptr->right);
	else
		return 0;
}

// Helper for meantickets
int numnodes(struct fans* root) {
	if (root == NULL) return 0;

	return 1 + numnodes(root->left) + numnodes(root->right);
}

// This function will calculate each subtrees height and determine if they are balanced
void heightBalance(struct fans* root) {
	if (root == NULL) {
		return;
	}

	int lDepth = leftDepth(root->left);
	int rDepth = rightDepth(root->right);

	if (lDepth == rDepth) {
		printf("left height = %d right height = %d balanced\n", lDepth, rDepth);
	}
	else {
		printf("left height = %d right height = %d not balanced\n", lDepth, rDepth);
	}
}

// Helper functions for heightBalance that will recursively calculate left and right depth
int leftDepth(struct fans* root) {
	if (root == NULL) {
		return -1;
	}

	return 1 + leftDepth(root->left);
}

int rightDepth(struct fans* root) {
	if (root == NULL) {
		return -1;
	}

	return 1 + rightDepth(root->right);
}

// We will recursively traverse and sum the tree of nodes that come before the given nodes name
int countBefore(struct fans* root, char* name) {
	if (root == NULL)
		return 0;

	if (strcmp(root->name, name) < 0)
		return root->tickets + countBefore(root->left, name) + countBefore(root->right, name);
	else
		return countBefore(root->left, name);
}

// Deletion and its aux functions

// Returns the parent of the node that is passed in
struct fans* parent(struct fans* root, struct fans* node) {
	if (root == NULL || root == node)
		return NULL;

	if (root->left == node || root->right == node)
		return root;

	if (strcmp(node->name, root->name) < 0)
		return parent(root->left, node);
	else 
		return parent(root->right, node);

	return NULL; 
}

// Returns a pointer to the node storing the minimum value in the tree
struct fans* minVal(struct fans* root) {
	if (root->left == NULL)
		return root;
	else
		return minVal(root->left);
}

// Returns a pointer to the node storing the maximum value in the tree
struct fans* maxVal(struct fans* root) {
	if (root->right == NULL)
		return root;
	else
		return maxVal(root->right);
}

// Returns 1 if node is a leaf node, 0 otherwise.
int isLeaf(struct fans* node) {
	return (node->left == NULL && node->right == NULL);
}

// Returns 1 if node has a left child and no right child.
int hasOnlyLeftChild(struct fans* node) {
	return (node->left != NULL && node->right == NULL);
}

// Returns 1 if node has a right child and no left child.
int hasOnlyRightChild(struct fans* node) {
	return (node->left == NULL && node->right != NULL);
}

/*
// Returns the node being searched for 
struct fans* findNode(struct fans* root, char* name) {
	if (root == NULL)
		return NULL;

	struct fans* left = findNode(root->left, name);

	if (left != NULL) {
		return left;
	}

	return findNode(root->right, name);
}
*/

// Will delete the node storing value in the tree rooted at root. 
struct fans* delete(struct fans* root, char* name) {
	struct fans* delnode, * new_del_node, * save_node;
	struct fans* par;
	int depth = 0;
	delnode = findName(root, name, &depth);

	if (delnode == NULL)
		return root;

	par = parent(root, delnode); 
	
	// If the node is a leaf we can simply free and delete it
	if (isLeaf(delnode)) {
		if (par == NULL) {
			free(delnode);
			return NULL;
		}

		if (par->left == delnode) {
			par->left = NULL;
		}
		else {
			par->right = NULL;
		}
		free(delnode);
		return root;
	}

	// If the node has a left child we will delete it and reassign the parent
	if (hasOnlyLeftChild(delnode)) {
		if (par == NULL) {
			save_node = delnode->left;
			free(delnode);
			return save_node;
		}

		if (par->left == delnode) {
			save_node = par->left; 
			par->left = par->left->left; 
			free(save_node); 
		}

		else {
			save_node = par->right; 
			par->right = par->right->left; 
			free(save_node);
		}
		free(delnode);
		return root;
	}

	// If the node has a right child we will delete it and reassign the parent
	if (hasOnlyRightChild(delnode)) {
		if (par == NULL) {
			save_node = delnode->right;
			free(delnode);
			return save_node;
		}

		if (par->left == delnode) {
			save_node = par->left;
			par->left = par->left->right;
			free(save_node);
		}

		else {
			save_node = par->right;
			par->right = par->right->right;
			free(save_node);
		}
		free(delnode);
		return root;
	}

	// If the node has two children we will have to find the smallest node in the subtree before deletion
	new_del_node = minVal(delnode->right);
	strcpy(delnode->name, new_del_node->name);
	delnode->tickets = new_del_node->tickets;
	root = delete(root, new_del_node->name);
	return root;
}

// This function will free the BST
void freeTree(struct fans* root) {
	if (root == NULL) {
		return;
	}

	freeTree(root->left);
	freeTree(root->right);

	free(root);
}

// Our main function
int main() {
	int n, count, depth;
	char command[20];
	char name[31];

	struct fans* root = NULL;

	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		scanf("%s", command);

		if (strcmp(command, "buy") == 0) {
			scanf("%s %d", name, &count);  
			struct fans* fan = create_node(name);
			root = purchaseTicket(root, name, count);
		}
		else if (strcmp(command, "use") == 0) {
			scanf("%s %d", name, &count); 
			root = useTicket(root, name, count);
		}
		else if (strcmp(command, "find") == 0) {
			scanf("%s", name);
			depth = 0;
			struct fans* nameToFind = findName(root, name, &depth);
			if (nameToFind == NULL) {
				printf("%s not found\n", name);
			}
			else {
				printf("%s %d %d\n", nameToFind->name, nameToFind->tickets, depth);
			}
		}
		else if (strcmp(command, "mean_tickets") == 0) {
			printf("%.2f\n", meanTickets(root));
		}
		else if (strcmp(command, "height_balance") == 0) {
			heightBalance(root);
		}
		else if (strcmp(command, "count_before") == 0) {
			scanf("%s", name);
			depth = 0;
			int before = countBefore(root, name);
			printf("%d\n", before);
		}
	}
	freeTree(root);
	return 0;
}
