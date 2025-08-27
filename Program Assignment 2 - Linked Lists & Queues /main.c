/* COP 3502C Assignment 2
This program is written by: Jared Camilo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 12
#define EMPTY 1

// The structures we will use for this program
typedef struct Customer {
	char name[18];
	int time;
	int ticketCount;
	int queueNumber;
} Customer;

typedef struct Node {
	Customer* customer;
	struct Node* next;
} Node;

typedef struct Queue {
	Node* front;
	Node* rear;
} Queue;


// Takes in the customers information and initializes it 
Customer* createCustomer(char* customerName, int customerTime, int numTicket, int numQueue) {
	Customer* customer = (Customer*)malloc(sizeof(Customer));

	if (customer == NULL) {
		return NULL;
	}

	strcpy(customer->name, customerName);
	customer->time = customerTime;
	customer->ticketCount = numTicket;
	customer->queueNumber = numQueue;

	return customer;
}

// Initializes the front and rear of the queue as NULL
void init(Queue* q) {
	if (q == NULL) {
		printf("Error: Queue is NULL in init.\n");
		return;
	}

	q->front = NULL;
	q->rear = NULL;
}

// Takes the customers information and creates a node for it and adds it to the queue
void enqueue(Queue* q, Customer* customer) {
	if (q == NULL || customer == NULL) {
		printf("Error: Queue or customer is NULL in enqueue.\n");
		return;
	}

	Node* newCustomer = (Node*)malloc(sizeof(Node));
	if (newCustomer == NULL) {
		printf("Memory allocation failed for new customer node\n");
		return;
	}

	newCustomer->customer = customer;
	newCustomer->next = NULL;

	if (q->front == NULL) {
		q->front = newCustomer;
		q->rear = newCustomer;
	}
	else {
		q->rear->next = newCustomer;
		q->rear = newCustomer;
	}
}

// This will take in the customer to dequeue first, loop until it finds it then free it
void dequeue(Queue* q, Customer* bestCustomer, int* currentTime) {
	int startTime = 0;
	if (q == NULL || q->front == NULL) {
		return EMPTY;
	}

	Node* tempNode = q->front;
	Node* prevNode = NULL;

	if (tempNode == NULL) {
		return NULL;
	}

	// We will check for the proper node to dequeue and use strcmp to ensure it is the correct one
	while (tempNode != NULL) {
		if (strcmp(tempNode->customer->name, bestCustomer->name) == 0 && tempNode->customer->time == bestCustomer->time &&
			tempNode->customer->ticketCount == bestCustomer->ticketCount && tempNode->customer->queueNumber == bestCustomer->queueNumber) {
			if (prevNode == NULL) {
				q->front = tempNode->next;
			}
			else {
				prevNode->next = tempNode->next;
			}

			if (tempNode == q->rear) {
				q->rear = prevNode;
			}


			if (*currentTime > bestCustomer->time) {
				startTime = *currentTime;
			}
			else {
				startTime = bestCustomer->time;
			}

			// We will update the currentTime pointer and free necessary memory
			*currentTime = startTime + 20 + (10 * bestCustomer->ticketCount);
			printf("%s left the counter at time %d from line %d.\n", bestCustomer->name, *currentTime, bestCustomer->queueNumber);


			free(tempNode->customer);
			free(tempNode);
			tempNode = NULL;
			break;
		}
		prevNode = tempNode;
		tempNode = tempNode->next;
	}
}

// This checks if the queue is empty by checking its front and rear
int empty(Queue* q) {
	if (q == NULL) {
		printf("Error: Queue pointer is NULL\n");
		return 1;
	}

	return (q->front == NULL && q->rear == NULL);
}

// This will return the first customer in the queue
Customer* peek(Queue* q) {
	if (q == NULL || empty(q)) {
		return NULL;
	}

	return q->front->customer;

}

Customer* findBestCustomer(Queue* q, int currentTime) {
	Customer* bestCustomer = NULL;
	int bestTicketCount = -1;
	int bestArrivalTime = -1;
	int bestQueueIndex = -1;

	for (int i = 0; i < MAX; i++) {
		if (!empty(&q[i])) {
			Customer* frontCustomer = peek(&q[i]);

			/* We will check the times of customers to find the first to dequeue 
			If there is no best time then we compare ticket counts
			(Yes it's a lot of edge cases */
			if (frontCustomer->time <= currentTime) {
				if (bestCustomer == NULL ||
					frontCustomer->ticketCount < bestTicketCount ||
					(frontCustomer->ticketCount == bestTicketCount && frontCustomer->time < bestArrivalTime)) {
					bestCustomer = frontCustomer;
					bestTicketCount = frontCustomer->ticketCount;
					bestArrivalTime = frontCustomer->time;
				}
			}
			else {
				if (bestCustomer == NULL || frontCustomer->time < bestArrivalTime) {
					bestCustomer = frontCustomer;
					bestArrivalTime = frontCustomer->time;
				}
			}
		}
	}
	return bestCustomer;
}


// Our main function
int main() {
	int c, n, t, m, x;
	Queue q[MAX];
	char customerName[18];
	int currentTime = 0;

	for (int i = 0; i < MAX; i++) {
		init(&q[i]);
	}

	// We will take the input and loop through the sets of queues and take in the amount of customers and data
	scanf("%d", &c);
	for (int i = 0; i < c; i++) {
		scanf("%d", &n);
		for (int j = 0; j < n; j++) {
			scanf("%d %d %17s %d", &t, &m, customerName, &x);

			if (m < 1 || m > MAX) {
				printf("Error: Invalid queue number %d\n", m);
				continue;
			}

			Customer* newCustomer = createCustomer(customerName, t, x, m);
			if (newCustomer == NULL) {
				printf("Error creating customer\n");
				continue;
			}
			enqueue(&q[m - 1], newCustomer);
		}

		// Use a do while to handle dequeuing properly for each set of customers
		while (1) {
			Customer* bestCustomer = findBestCustomer(q, currentTime);
			if (bestCustomer == NULL) {
				break;
			}

			dequeue(&q[bestCustomer->queueNumber - 1], bestCustomer, &currentTime);
		}
		// Set of queues ended so we print the line and reset current time
		printf("======\n");
		currentTime = 0;
	}

	// We free the necessary data
	for (int i = 0; i < MAX; i++) {
		Node* temp = peek(&q[i]);
		while (temp != NULL) {
			Node* freeNode = temp;
			temp = temp->next;
			free(freeNode->customer);
			free(freeNode);
		}
	}
	return 0;
}
