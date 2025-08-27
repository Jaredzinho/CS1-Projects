/* COP 3502C Assignment 3
This program is written by: Jared Camilo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the max for array size and name length
#define MAX_PEOPLE 10

// Define our global variables
char names[MAX_PEOPLE][20];
int snacks[MAX_PEOPLE];
int seatingRestrictions[MAX_PEOPLE][MAX_PEOPLE];

/* We will check if each permutation is valid by returning boolean values
Then we will make sure the current person has access to snacks and that they 
aren't sitting next to a someone they aren't supposed to be next to*/
int seatCheck(int currentPerm[], int len) {
	if (currentPerm == NULL) {
		return 0;
	}

	for (int i = 0; i < len; i++) {
		if (snacks[currentPerm[i]] == 0) {
			if ((i == 0 || snacks[currentPerm[i - 1]] == 0) &&
				(i == len - 1 || snacks[currentPerm[i + 1]] == 0)) {
				return 0;
			}
		}

		if (i < len - 1 && seatingRestrictions[currentPerm[i]][currentPerm[i + 1]] == 1 ||
			(i > 0 && seatingRestrictions[currentPerm[i]][currentPerm[i - 1]] == 1)) {
			return 0;
		}
	}
	return 1;
}

// We will permute using the used array method and keep track of every permutation
void permute(int arr[], int used[], int k, int n) {
	if (k == n) {
		if (seatCheck(arr, n)) {
			for (int i = 0; i < n; i++) {
				printf("%s\n", names[arr[i]]);
			}
			exit(0);
		} else {
			return;
		}
	}

	for (int i = 0; i < n; i++) {
		if (!used[i]) {
			used[i] = 1;
			arr[k] = i;
			permute(arr, used, k + 1, n);
			used[i] = 0;
		}
	}
}

// Our main function
int main() {
	int n, p;
	char name1[20], name2[20];
	int arr[MAX_PEOPLE];
	int used[MAX_PEOPLE];
	int name1Index = -1, name2Index = -1;

	scanf("%d %d", &n, &p);

	for (int i = 0; i < n; i++) {
		scanf("%s %d", names[i], &snacks[i]);
	}

	for (int i = 0; i < n; i++) {
		used[i] = 0;
		for (int j = 0; j < n; j++) {
			seatingRestrictions[i][j] = 0;
		}
	}

	for (int i = 0; i < p; i++) {
		scanf("%s %s", name1, name2);

		for (int j = 0; j < n; j++) {
			if (strcmp(names[j], name1) == 0) {
				name1Index = j;
			}
			if (strcmp(names[j], name2) == 0) {
				name2Index = j;
			}
		}

		seatingRestrictions[name1Index][name2Index] = 1;
		seatingRestrictions[name2Index][name1Index] = 1;
	}

	permute(arr, used, 0, n);

	return 0;
}
