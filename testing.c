#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct Node {
	struct Node* prev, * next;
	unsigned int pageNumber;
};

struct Queue
{
	unsigned int count;
	unsigned int numberOfFrames;
	struct Node *front, *rear;
};

struct Hash
{
	unsigned int capacity;
	struct Node** arr;
};

struct Hash* createHash(unsigned int capacity)
{
	struct Hash* temp = (struct Hash*)malloc(sizeof(struct Hash));

	temp->capacity = capacity;
	temp->arr = (struct Node**)malloc(capacity * sizeof(struct Node*));

	for (int i = 0; i < capacity; i++)
	{
		temp->arr[i] = NULL;
	}

	return temp;
}

int isQueueFull(struct Queue* q)
{
	return q->count == q->numberOfFrames;
}

int isQueueEmpty(struct Queue* q)
{
	return q->rear == NULL;
}

struct Node* newNode(unsigned int pageNumber)
{
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	temp->pageNumber = pageNumber;

	temp->prev = NULL;
	temp->next = NULL;

	return temp;
}

struct Queue* createQueue(unsigned int size)
{
	struct Queue* temp = (struct Queue*)malloc(sizeof(struct Queue));

	temp->count = 0;
	temp->front = NULL;
	temp->rear = NULL;

	temp->numberOfFrames = size;

	return temp;
}

void deQueue(struct Queue* q)
{
	if (isQueueEmpty(q)) return;

	if (q->front == q->rear)
		q->rear->next = NULL;

	struct Node* temp = q->rear;
	q->rear = q->rear->prev;

	if (q->rear)
		q->rear->next = NULL;
	
	free(temp);

	q->count--;

}

void enQueue(struct Queue* q, struct Hash* h, unsigned int pageNumber)
{
	if (isQueueFull(q))
	{
		h->arr[q->rear->pageNumber] = NULL;
		deQueue(q);
	}

	struct Node* temp = newNode(pageNumber);
	temp->next = q->front;

	if (isQueueEmpty(q))
	{
		q->rear = temp;
		q->front = temp;
	}
	else
	{
		q->front->prev = temp;
		q->front = temp;
	}

	h->arr[pageNumber] = temp;

	q->count++;

}

void ReferencePage(struct Queue* q, struct Hash* h, unsigned int pageNumber)
{
	struct Node* requestedPage = h->arr[pageNumber];

	if (requestedPage == NULL)
	{
		printf("Page Fault!\n");
		enQueue(q, h, pageNumber);
	}
	else if (requestedPage != q->front)
	{
		requestedPage->prev->next = requestedPage->next;
		if (requestedPage->next)
		{
			requestedPage->prev->next = requestedPage->next;
		}

		if (requestedPage == q->rear)
		{
			q->rear = requestedPage->prev;
			q->rear->next = NULL;
		}

		requestedPage->next = q->front;
		requestedPage->prev = NULL;

		requestedPage->next->prev = requestedPage;

		q->front = requestedPage;
		
	}
}

void displayCache(struct Queue* q)
{
	if (isQueueEmpty(q))
	{
		printf("Cache is empty!\n");
		return;
	}

	struct Node* temp = q->front;
	while (temp->next != NULL)
	{
		printf("%d->", temp->pageNumber);
		temp = temp->next;
	}
	printf("%d\n", temp->pageNumber);

}

int main()
{
	int size = 0;
	int pagenumber = 0;
	printf("Enter the max size of cache: ");
	scanf("%d", &size);

	struct Queue* q = createQueue(size);
	
	printf("Enter the total number of pages: ");
	scanf("%d", &size);

	struct Hash* h = createHash(size);
	
	

	printf("The cache before accessing any page: ");
	displayCache(q);

	printf("Enter the page numbers (-1 to exit) : \n");
	while (1)
	{
		fflush(stdin);
		scanf("%d", &pagenumber);
		
		if (pagenumber >= size)
		{
			printf("Invalid Page number!\n");
			continue;
		}

		if (pagenumber == -1)
			break;

		ReferencePage(q, h, pagenumber);
	}

	printf("The cache after accessing : ");
	displayCache(q);

	return 0;
}
