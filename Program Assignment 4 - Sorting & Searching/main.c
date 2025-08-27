/* COP 3502C Programming Assignment 4
This program is written by: Jared Camilo */

#include <stdio.h>
#include <stdlib.h>

// We will create a struct to store our coordinates
typedef struct Coordinates {
	int x, y;
} Coordinates;

// Global variable of our location
Coordinates location;

// My function prototypes
void insertionSort(Coordinates*, int, int);
void mergeSort(Coordinates*, int, int, int);
void merge(Coordinates*, int, int, int);
void wrapperSort(Coordinates*, int, int);
int compareTo(Coordinates*, Coordinates*);

// Reads the inputs and returns an array of points for the sorting processes
Coordinates* readData(int* n, int* s, int* t, Coordinates** queries) {
	// xy = location, n = number of stadiums, s is the number of points to search for, t represents the threshold to use merge or insertion sort
	scanf("%d %d %d %d %d", &location.x, &location.y, &(*n), &(*s), &(*t));

	Coordinates* stadiums = (Coordinates*)malloc(*n * sizeof(Coordinates));
	*queries = (Coordinates*)malloc(*s * sizeof(Coordinates));

	if (!stadiums || !queries) {
		return NULL;
	}

	for (int i = 0; i < *n; i++) {
		scanf("%d %d", &stadiums[i].x, &stadiums[i].y);
	}

	for (int i = 0; i < *s; i++) {
		scanf("%d %d", &(*queries)[i].x, &(*queries)[i].y);
	}
	return stadiums;
}

// Compare points during sorting and reurns an integer based on the result
int compareTo(Coordinates* ptrPt1, Coordinates* ptrPt2) {
	int pt1Dist = (ptrPt1->x - location.x) * (ptrPt1->x - location.x) + 
		(ptrPt1->y - location.y) * (ptrPt1->y - location.y);
	int pt2Dist = (ptrPt2->x - location.x) * (ptrPt2->x - location.x) + 
		(ptrPt2->y - location.y) * (ptrPt2->y - location.y);

	if (ptrPt1->x == ptrPt2->x && ptrPt1->y == ptrPt2->y) {
		return 0;
	}

	if (pt1Dist < pt2Dist) {
		return -1;
	} else if (pt1Dist > pt2Dist) {
		return 1;
	} else if (pt1Dist == pt2Dist) {
		if (ptrPt1->x < ptrPt2->x) {
			return -1;
		} else if (ptrPt1->x == ptrPt2->x) {
			if (ptrPt1->y < ptrPt2->y) {
				return -1;
			}
			else {
				return 1;
			}
		} else {
			return 1;
		}
	}
	return 0;
}

/* After sorting using insertion and merge we will take in the sorted array and a coordinate
the function will then use binary search to see if there is at a stadium at the coordinate
we will complete this by checking the return value of compareTo */
int binarySearch(Coordinates* arr, Coordinates item, int len) {
	int l = 0, h = len - 1;
	int mid;

	while (l <= h) {
		mid = (l + h) / 2;
		int comparable = compareTo(&arr[mid], &item);

		if (comparable == 0) {
			return mid;
		}
		else if (comparable < 0) {
			l = mid + 1;
		} else {
			h = mid - 1;
		}
	}
	return -1;
}

// Start for sorting as it will take in the array, its length, and a threshold value and pass the array to the merge sort 
void wrapperSort(Coordinates* arr, int len, int t) {
	if (len <= t) {
		insertionSort(arr, 0, len - 1);
	}
	else {
		mergeSort(arr, 0, len - 1, t);
	}
}

/* Will take in the array and indexes then use recursion to divide the array and use merge sort
after reaching the threshold it will pass the array into insertion sort for runtime */
void mergeSort(Coordinates* arr, int startingIdx, int endingIdx, int t) {
	if (endingIdx - startingIdx + 1 <= t) {
		insertionSort(arr, startingIdx, endingIdx);
		return;
	}

	int m = (endingIdx + startingIdx) / 2;

	mergeSort(arr, startingIdx, m, t);
	mergeSort(arr, m + 1, endingIdx, t);
	merge(arr, startingIdx, m, endingIdx);
}

// The merging portion for merge sort
void merge(Coordinates* arr, int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	Coordinates* L = (Coordinates*)malloc(n1 * sizeof(Coordinates));
	Coordinates* R = (Coordinates*)malloc(n2 * sizeof(Coordinates));

	for (i = 0; i < n1; i++) {
		L[i] = arr[l + i];
	}

	for (j = 0; j < n2; j++) {
		R[j] = arr[m + 1 + j];
	}

	i = 0; 
	j = 0; 
	k = l; 

	while (i < n1 && j < n2) {
		if (compareTo(&L[i], &R[j]) <= 0) {
			arr[k] = L[i];
			i++;
		} else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}

	free(L);
	free(R);
}

// Will be used when merge sort reaches its treshold
void insertionSort(Coordinates* arr, int startingIdx, int endingIdx) {
	int i, j;
	Coordinates item;

	for (i = startingIdx + 1; i <= endingIdx; i++) {
		item = arr[i];
		j = i - 1;
		
		while (j >= startingIdx && compareTo(&arr[j], &item) > 0) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = item;
	}
}

// Our main function
int main() {
	int n, s, t;
	Coordinates* queries = NULL;

	Coordinates* stadiums = readData(&n, &s, &t, &queries);

	wrapperSort(stadiums, n, t);

	for (int i = 0; i < n; i++) {
		printf("%d %d\n", stadiums[i].x, stadiums[i].y);
	}
	
	// We will use binary search to check the sorted array for queries and print results based on the return value
	for (int i = 0; i < s; i++) {
		int searchResult = binarySearch(stadiums, queries[i], n);

		if (searchResult == -1) {
			printf("%d %d: no stadium found at this location\n", queries[i].x, queries[i].y);
		} else {
			printf("%d %d: stadium found at rank %d in the sorted order\n", queries[i].x, queries[i].y, searchResult + 1);
		}
	}

	free(stadiums);
	free(queries);

	return 0;
}






