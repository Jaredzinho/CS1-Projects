/* COP 3502C Assignment 1
This program is written by: Jared Camilo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
	char* name; //Player name (dynamically allocated)
	int totalGoals; //Total goals scored by the player(needs to be counted based on the match)
}Player;

typedef struct Team {
	char* name; // Name of the team (dynamically allocated)
	Player** players; //An array of Player pointers where each pointer is pointing to a player
	int playerCount; // Number of players in the team
}Team;

typedef struct Goal {
	Player* scorer; // Pointer to the player who scored
	int time; // Time of the goal in minutes
}Goal;

typedef struct Match {
	Team* team1; // Pointer to Team 1
	Team* team2; // Pointer to Team 2
	Goal* goals; // Array of goals scored in the match
	int goalCount; // Total number of goals in the match
} Match;

//this is a special struct that should only be used to process a particular command
//(mentioned below in the function requirement)
typedef struct MatchWinnerTeam
{
	Team* winnerTeam; //pointer to the winner team if any.
	int isDraw; //set to 1 if it was a draw. In that case winnerTeam will be NULL

}MatchWinnerTeam;

// We will use the player struct to allocate memory for a player and their name and also initialize their goals to 0
Player* createPlayer(char* name) {
	Player* player = (Player*)malloc(sizeof(Player));

	int len = (strlen(name) + 1);
	player->name = (char*)malloc(len * sizeof(char));
	strcpy(player->name, name);

	player->totalGoals = 0;

	return player;
}

// We will use the team struct to allocate memory for each team and its players
Team* createTeam(char* name, int playerCount) {
	Team* team = (Team*)malloc(sizeof(Team));
	team->players = (Player**)malloc(playerCount * sizeof(Player*));

	int len = (strlen(name) + 1);
	team->name = (char*)malloc(len * sizeof(char));
	strcpy(team->name, name);

	for (int i = 0; i < playerCount; i++) {
		team->players[i] = NULL;
	}

	team->playerCount = playerCount;
	return team;
}

// This function will loop through the list of players and match the entered name, if there is no match it returns NULL
Player* getPlayerByName(char* name, Player** playerList, int numPlayers) {
	if (playerList == NULL || name == NULL) {
		return NULL;
	}

	for (int i = 0; i < numPlayers; i++) {
		if (strcmp(playerList[i]->name, name) == 0) {
			return playerList[i];
		}
	}
	return NULL;
}

// This function does the same process as the previous but instead loops through the team names
Team* getTeamByTeamName(Team** teamList, int numTeams, char* tname) {
	if (teamList == NULL || tname == NULL) {
		return NULL;
	}

	for (int i = 0; i < numTeams; i++) {
		if (strcmp(teamList[i]->name, tname) == 0) {
			return teamList[i];
		}
	}
	return NULL;
}

// This function records goals and updates the players total goals
void recordGoal(Match* match, Player* player, int time) {
	if (match == NULL || player == NULL) {
		return;
	}

	Goal* tempGoal = realloc(match->goals, (match->goalCount + 1) * sizeof(Goal));

	if (tempGoal == NULL) {
		printf("Memory allocation failed for goals\n");
		return;
	}

	match->goals = tempGoal;

	match->goals[match->goalCount].scorer = player;
	match->goals[match->goalCount].time = time;

	player->totalGoals++;
	match->goalCount++;
}

// This function will find out who scored hat tricks in every match
int printHatTrickPlayers(Match* match) {
	if (match == NULL || match->goalCount <= 0) {
		return 0;
	}

	Player** pp = malloc(match->goalCount * sizeof(Player*));
	int* gc = calloc(match->goalCount, sizeof(int));
	int tally = 0;

	// The logic here is that if the player is in pp, their gc is incremented and playerFound is set to 1 so it doesn't go to !playerFound
	for (int i = 0; i < match->goalCount; i++) {
		int playerFound = 0;
		for (int j = 0; j < tally; j++) {
			if (strcmp(pp[j]->name, match->goals[i].scorer->name) == 0) {
				gc[j]++;
				playerFound = 1;
				break;
			}
		}

		// When the strcmp doesn't go through !playerFound is activated, adding a new entry to pp and setting their gc to 1 
		if (!playerFound) {
			pp[tally] = match->goals[i].scorer;
			gc[tally] = 1;
			tally++;
		}
	}

	// We will check who scored three goals and use a boolean value to ensure hat trick found is printed once 
	int hatTrick = 0;
	for (int k = 0; k < tally; k++) {
		if (gc[k] >= 3) {
			if (!hatTrick) {
				printf("hat trick found in %s vs %s:\n", match->team1->name, match->team2->name);
				hatTrick = 1;
			}
			printf("%s %d\n", pp[k]->name, pp[k]->totalGoals);
		}
	}

	free(pp);
	free(gc);
	return hatTrick;
}

// This function will use strcmp to determine which team had more goals in the matches and determine a winner
MatchWinnerTeam* getWinningTeam(Match* matches, int matchCount, char* team1Name, char* team2Name) {
	if (matches == NULL || team1Name == NULL || team2Name == NULL) {
		return NULL;
	}

	MatchWinnerTeam* winnerOfMatch = (MatchWinnerTeam*)malloc(sizeof(MatchWinnerTeam));

	int team1Goals = 0;
	int team2Goals = 0;
	winnerOfMatch->isDraw = 0;
	int matchFound = 0;
	int matchIndex = -1;

	// We will need a triple nested loop since team struct has a double pointer for players
	for (int i = 0; i < matchCount; i++) {
		if (strcmp(matches[i].team1->name, team1Name) == 0 && strcmp(matches[i].team2->name, team2Name) == 0) {
			// We need to save the match index from the loop for later
			matchFound = 1;
			matchIndex = i;

			// We will use boolean values again to assign the goals to each team
			for (int j = 0; j < matches[i].goalCount; j++) {
				int scorerFound = 0;
				for (int k = 0; k < matches[i].team1->playerCount; k++) {
					if (strcmp(matches[i].goals[j].scorer->name, matches[i].team1->players[k]->name) == 0) {
						team1Goals++;
						scorerFound = 1;
						break;
					}
				}
				if (!scorerFound) {
					team2Goals++;
				}
			}
			break;
		}
	}

	// We will use boolean values to determine if there was a match or not
	if (!matchFound) {
		free(winnerOfMatch);
		return NULL;
	}

	// Now we will compare the teams goals to see the winner and use the match index to keep the code clean
	if (team1Goals > team2Goals) {
		winnerOfMatch->winnerTeam = matches[matchIndex].team1;
		winnerOfMatch->isDraw = 0;
	}
	else if (team1Goals == team2Goals) {
		winnerOfMatch->winnerTeam = NULL;
		winnerOfMatch->isDraw = 1;
	}
	else {
		winnerOfMatch->winnerTeam = matches[matchIndex].team2;
		winnerOfMatch->isDraw = 0;
	}
	return winnerOfMatch;
}

// This function will call the previous in order to calculate the champion of the tournament using points
Team* getChampion(Team** teamList, int teamCount, Match* matchList, int matchCount) {
	if (teamList == NULL || teamCount <= 0 || matchList == NULL || matchCount <= 0) {
		return NULL;
	}

	// I used calloc to set the initial values to 0
	int* points = calloc(teamCount, sizeof(int));
	int champion = 0;
	int championIndex = 0;

	// We will use strcmp to compare the matchlist to the teamlist and find the teams that played against eachother in these matches
	for (int i = 0; i < matchCount; i++) {
		int teamOneIndex = -1;
		int teamTwoIndex = -1;
		for (int j = 0; j < teamCount; j++) {
			if (matchList[i].team1->name && teamList[j]->name && strcmp(matchList[i].team1->name, teamList[j]->name) == 0) {
				teamOneIndex = j;
			}
			if (matchList[i].team2->name && teamList[j]->name && strcmp(matchList[i].team2->name, teamList[j]->name) == 0) {
				teamTwoIndex = j;
			}
		}

		// Now with the indexes we will call the getWinningTeam function
		if (teamOneIndex != -1 && teamTwoIndex != -1) {
			MatchWinnerTeam* winnerTeam = getWinningTeam(matchList, matchCount, matchList[i].team1->name, matchList[i].team2->name);

			// We will use the teams indexes to assign points based on W/L/D
			if (winnerTeam && winnerTeam->winnerTeam == teamList[teamOneIndex]) {
				points[teamOneIndex] += 3;
			}
			else if (winnerTeam && winnerTeam->winnerTeam == teamList[teamTwoIndex]) {
				points[teamTwoIndex] += 3;
			}
			else if (winnerTeam && winnerTeam->isDraw) {
				points[teamOneIndex] += 1;
				points[teamTwoIndex] += 1;
			}
			free(winnerTeam);
		}
	}

	// Determine the winner by comparing points
	for (int k = 0; k < teamCount; k++) {
		if (points[k] > champion) {
			champion = points[k];
			championIndex = k;
		}
	}
	free(points);

	if (championIndex == -1) {
		return NULL;
	}

	return teamList[championIndex];
}

// This function exists for the max_goal_player function
Player* maxGoalPlayer(Player** playerList, int numPlayers) {
	if (playerList == NULL || numPlayers <= 0) {
		return NULL;
	}

	if (playerList[0] == NULL) {
		return NULL;
	}

	Player* topScorer = playerList[0];

	for (int i = 1; i < numPlayers; i++) {
		if (playerList[i] == NULL) {
			continue;
		}

		if (playerList[i]->totalGoals > topScorer->totalGoals) {
			topScorer = playerList[i];
		}

	}
	return topScorer;
}

// This function will be used to free any necessary memory
void freeMemory(Player** playerList, Team** teamList, int numPlayers, int numTeams) {
	if (playerList == NULL || teamList == NULL) {
		return;
	}

	for (int i = 0; i < numPlayers; i++) {
		if (playerList[i] != NULL && playerList[i]->name != NULL) {
			free(playerList[i]->name);
			playerList[i]->name = NULL;
		}
		free(playerList[i]);
		playerList[i] = NULL;
	}
	free(playerList);
	playerList = NULL;

	for (int j = 0; j < numTeams; j++) {
		free(teamList[j]->players);
		teamList[j]->players = NULL;
		free(teamList[j]->name);
		teamList[j]->name = NULL;
		free(teamList[j]);
		teamList[j] = NULL;
	}
	free(teamList);
	teamList = NULL;
}

// The main function
int main() {
	Player** playerList = NULL;
	Team** teamList = NULL;
	Match* matchList = NULL;

	int t, m, c;
	char teamName[21];
	int teamPlayers;
	int totalPlayers = 0;
	int playerIndex = 0;

	scanf("%d %d %d\n", &t, &m, &c);

	// We will allocate the array of arrays and initialize the values
	teamList = (Team**)malloc(t * sizeof(Team*));
	matchList = (Match*)malloc(m * sizeof(Match));
	for (int j = 0; j < m; j++) {
		matchList[j].team1 = NULL;
		matchList[j].team2 = NULL;
		matchList[j].goals = NULL;
		matchList[j].goalCount = 0;
	}

	// We will start scanning for the team names and players to fill the array
	for (int i = 0; i < t; i++) {
		scanf("%20s %d", teamName, &teamPlayers);
		teamList[i] = createTeam(teamName, teamPlayers);
		if (teamList[i] == NULL) {
			printf("Memory allocation failed for team %s\n", teamName);
			return 1;
		}

		for (int l = 0; l < teamPlayers; l++) {
			char playerName[21];
			scanf("%20s", playerName);
			teamList[i]->players[l] = createPlayer(playerName);
			totalPlayers++;
		}
	}

	playerList = (Player**)malloc(totalPlayers * sizeof(Player*));

	for (int i = 0; i < t; i++) {
		for (int j = 0; j < teamList[i]->playerCount; j++) {
			playerList[playerIndex++] = teamList[i]->players[j];
		}
	}

	char teamOneName[21];
	char teamTwoName[21];
	char Scorer[21];
	int time;

	// Now we will scan for scorers in each match
	for (int j = 0; j < m; j++) {
		scanf("%20s %20s", teamOneName, teamTwoName);
		matchList[j].team1 = getTeamByTeamName(teamList, t, teamOneName);
		matchList[j].team2 = getTeamByTeamName(teamList, t, teamTwoName);

		while (1) {
			scanf("%20s", Scorer);
			if (strcmp(Scorer, "end") == 0) {
				break;
			}
			scanf("%d", &time);

			Player* player = getPlayerByName(Scorer, matchList[j].team1->players, matchList[j].team1->playerCount);
			if (player == NULL) {
				player = getPlayerByName(Scorer, matchList[j].team2->players, matchList[j].team2->playerCount);
			}

			if (player == NULL) {
				player = getPlayerByName(Scorer, matchList[j].team2->players, matchList[j].team2->playerCount);
			}

			if (player != NULL) {
				recordGoal(&matchList[j], player, time);
			}
		}
	}

	// We will loop through each command calling each function based on input
	for (int k = 0; k < c; k++) {
		char command[21];
		scanf("%s", command);

		if (strcmp(command, "max_goal_player") == 0) {
			Player* topScorer = maxGoalPlayer(playerList, totalPlayers);
			if (topScorer != NULL) {
				printf("player with max goal %s %d\n", topScorer->name, topScorer->totalGoals);
			}
		}
		else if (strcmp(command, "hattrick") == 0) {
			int hatTrickFound = 0;

			for (int p = 0; p < m; p++) {
				int result = printHatTrickPlayers(&matchList[p]);
				if (result) {
					hatTrickFound = 1;
				}
			}
			if (!hatTrickFound) {
				printf("no hat trick found in the tournament\n");
			}
		}
		else if (strcmp(command, "champion") == 0) {
			Team* champion = getChampion(teamList, t, matchList, m);
			if (champion != NULL) {
				printf("champion is %s\n", champion->name);
			}
		}
		else if (strcmp(command, "winner") == 0) {
			char team1Name[21], team2Name[21];
			scanf("%20s %20s", team1Name, team2Name);

			Team* team1 = getTeamByTeamName(teamList, t, team1Name);
			Team* team2 = getTeamByTeamName(teamList, t, team2Name);

			MatchWinnerTeam* result = getWinningTeam(matchList, m, team1Name, team2Name);

			if (result == NULL) {
				printf("no match between %s and %s\n", team1Name, team2Name);
			}
			else if (result->isDraw) {
				printf("%s vs %s match drawn\n", team1Name, team2Name);
			}
			else {
				printf("winner of %s vs %s is %s\n", team1Name, team2Name, result->winnerTeam->name);
			}
			free(result);
		}
	}
	freeMemory(playerList, teamList, totalPlayers, t);
	for (int j = 0; j < m; j++) {
		if (matchList[j].goals != NULL) {
			free(matchList[j].goals);
			matchList[j].goals = NULL;
		}
	}
	free(matchList);
	return 0;
}
