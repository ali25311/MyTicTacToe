#include <iostream>
#include <vector>
#include <cctype>
#include <fstream>
#include <unistd.h> //sleep(n)
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */

enum results {none, Win, Tie};

void displayGrid(std::vector<char> &gridPos); //displays grid to player
bool repeatProgram(std::vector<char> &gridPos, results &currentResult, int &spotsTaken, bool &playerGoing); //for repeating
char getLetter(); //get player choice of piece
void playerMove(std::vector<char> &gridPos, char playerPiece, int &spotsTaken); //get player's move on grid
void aiMove(std::vector<char> &gridPos, char enemyPiece, int &spotsTaken, char playerPiece); //get AI's move on grid
results checkStatus(std::vector<char> gridPos, results &currentResult, char piece, int &spotsTaken); //check if either player or AI has won the game, if not continue game
void writeToBin(int wins); //write high score to binary file
inline void displayTally(int wins, int losses, int ties);
int getNum(); //validate int

int main() {
  srand(time(NULL));
  bool playerGoing = rand() % 2;
  int spotsTaken = 0;
  int wins = 0;
  int losses = 0;
  int ties = 0;
  bool playAgain = true;
  results currentResult = none;
  std::vector<char> gridPos = {'1','2','3','4','5','6','7','8','9'};

  char enemyPiece = '\0';
  char playerPiece = getLetter();
  enemyPiece = (playerPiece == 'X') ? 'O' : 'X';

  while(playAgain) {
    //playerS TURN
    if(playerGoing == 1) {
      playerGoing = 0;
      std::cout << "\e[2J\e[0;0H"; //clear screen
      std::cout << "\nIT'S YOUR TURN!\n";
      displayGrid(gridPos);
      playerMove(gridPos, playerPiece, spotsTaken);
      currentResult = checkStatus(gridPos, currentResult, playerPiece, spotsTaken);
    
      if(currentResult == Win) {
        std::cout << "\e[2J\e[0;0H"; //clear screen
        displayGrid(gridPos);
        std::cout << "\nYOU WIN!";
        wins++;
        displayTally(wins, losses, ties);
        sleep(1);
        playAgain = repeatProgram(gridPos, currentResult, spotsTaken, playerGoing);
        continue;
      }

      else if(currentResult == Tie) {
        std::cout << "\e[2J\e[0;0H"; //clear screen
        displayGrid(gridPos);
        std::cout << "\nIT'S A TIE";
        ties++;
        displayTally(wins, losses, ties);
        sleep(1);
        playAgain = repeatProgram(gridPos, currentResult, spotsTaken, playerGoing);
        continue;
      }
    }
    //AI'S TURN
    if(playerGoing == 0) {
      std::cout << "\e[2J\e[0;0H"; //clear screen
      std::cout << "it is now the AI's turn...\n";
      sleep(1);
      std::cout << "\e[2J\e[0;0H"; //clear screen

      aiMove(gridPos, enemyPiece, spotsTaken, playerPiece);
      currentResult = checkStatus(gridPos, currentResult, enemyPiece, spotsTaken);

      if(currentResult == Win) {
        std::cout << "\e[2J\e[0;0H"; //clear screen
        displayGrid(gridPos);
        std::cout << "\nAI WINS!";
        losses++;
        displayTally(wins, losses, ties);
        sleep(1);
        playAgain = repeatProgram(gridPos, currentResult, spotsTaken, playerGoing);
        continue;
      }

      else if(currentResult == Tie) {
        std::cout << "\e[2J\e[0;0H"; //clear screen
        displayGrid(gridPos);
        std::cout << "\nIT'S A TIE";
        ties++;
        displayTally(wins, losses, ties);
        sleep(1);
        playAgain = repeatProgram(gridPos, currentResult, spotsTaken, playerGoing);
        continue;
      }
    }
    playerGoing = 1;
  }
  writeToBin(wins);
	return 0;
}

inline void displayTally(int wins, int losses, int ties) {
  std::cout << "\n\nWINS: " << wins;
  std::cout << "\nLOSSES: " << losses;
  std::cout << "\nTies: " << ties << std::endl;
}



void displayGrid(std::vector<char> &gridPos) {
  std::cout << gridPos[0] << " | " << gridPos[1] << " | " << gridPos[2] << std::endl;
  std::cout << gridPos[3] << " | " << gridPos[4] << " | " << gridPos[5] << std::endl;
  std::cout << gridPos[6] << " | " << gridPos[7] << " | " << gridPos[8] << std::endl;
}



bool repeatProgram(std::vector<char> &gridPos, results &currentResult, int &spotsTaken, bool &playerGoing) {
  char answer = '\0';
  bool validation = false;
  while(!validation) {
    std::cout << "\nWould you like to play again? (Y/N): ";
    std::cin.get(answer);
    answer = toupper(answer);
    std::cin.ignore(1000, '\n');
    validation = true;

    if((answer != 'Y') && (answer != 'N')) {
			std::cout << "\nINVALID INPUT!... try again\n\n";
      sleep(2);
			validation = false;
    }
    //reset grid
    else if(answer == 'Y') {
      gridPos = {'1','2','3','4','5','6','7','8','9'};
      currentResult = none;
      spotsTaken = 0;
      playerGoing = rand() % 2;
      return true;
    }
    else if(answer == 'N') {
      std::cout << "\nThanks for playing!!\n";
      return false;
    }
    else {
      exit(1);
    }
  }
  return false;

}


void writeToBin(int wins) {
  int highScore = 0;
  std::fstream myFile("highScore.bin", std::ios::binary | std::ios::in | std::ios::out);

  myFile.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));


  if (wins > highScore) {
		myFile.seekp(0, std::ios::beg);
    std::cout << "\n NEW HIGH SCORE(WINS)!\n\n";
		std::cout << "OLD HIGH SCORE: " << highScore << "\nNEW HIGH SCORE: " << wins << "\n\n\n";

    myFile.write(reinterpret_cast<char*>(&wins), sizeof(wins));
  }
	else {
		std::cout << "CURRENT HIGH SCORE(WINS): " << highScore << std::endl;
	}
	
  myFile.close();
}



char getLetter() {
  char answer = '\0';
  bool validation = false;
  while(!validation) {
    //std::cout << "\e[2J\e[0;0H"; //clear screen
    std::cout << "Enter if you want to be 'X' or 'O': ";
    std::cin.get(answer);
    answer = toupper(answer);
    std::cin.ignore(1000, '\n');
    validation = true;

    if((answer != 'X') && (answer != 'O')) {
			std::cout << "\ninvalid input! (must be X or O). try again..\n\n";
      sleep(2);
			validation = false;
    }
  }
  return answer;
}




void playerMove(std::vector<char> &gridPos, char playerPiece, int &spotsTaken) {
  spotsTaken++;
  int move = 0;
  int position = 0;
  bool validInt = true;
  while(validInt) {
    std::cout << "\nPlease enter where you'd like to move (1-9): ";
    move = getNum();
    position = move - 1;
    if(position > 9 && position < 1) {
      std::cout << "\nERROR, enter a # between 1 and 9.\n";
    }
    else if(gridPos[position] == 'X' || gridPos[position] == 'O') {
      std::cout << "\nERROR, position already taken! try again.\n";
    }
    else {
      gridPos[position] = playerPiece;
      validInt = false;
    }
  }
}



void getMove(std::vector<char> &gridPos, char enemyPiece, char playerPiece, bool &spotFound, int start, int end, int step, char piece) {
	// start is the start of the row we're checking
	// end is the end of the row we're checking
	int spots = 0;
	int spot = -1;  // Spot we will go to, start at -1 because this will stay -1 if a spot isn't open, and it will become something else if it finds an open spot.

	for (int i = start; i <= end; i += step) {
		if (gridPos[i] == piece) {
			spots++;
		}
		else if (isdigit(gridPos[i])) {
			spot = i;
		}
	}

	if ((spots == 2) && (spot != -1)) {
		gridPos[spot] = enemyPiece;
		spotFound = true;
	}
}




void aiMove(std::vector<char> &gridPos, char enemyPiece, int &spotsTaken, char playerPiece) {
	bool spotFound = false;
	int randomSpot = 0;
  bool validSpot = false;

	spotsTaken++;

	// OFFENSIVE MOVES...
	// Checking Row 1 
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 0, 2, 1, enemyPiece);
	if (spotFound == true) return;

	// Checking Row 2
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 3, 5, 1, enemyPiece);
	if (spotFound == true) return;

	// Checking Row 3
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 6, 8, 1, enemyPiece);
	if (spotFound == true) return;

  // Checking Col 1
  getMove(gridPos, enemyPiece, playerPiece, spotFound, 0, 6, 3, enemyPiece);
	if (spotFound == true) return;	
	
	// Checking Col 2
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 1, 7, 3, enemyPiece);
	if (spotFound == true) return;

  // Checking Col 3
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 2, 8, 3, enemyPiece);
	if (spotFound == true) return;

  // Checking Dia 1
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 0, 8, 4, enemyPiece);
	if (spotFound == true) return;

  // Checking Dia 2
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 2, 6, 2, enemyPiece);
	if (spotFound == true) return;
	

	// DEFENSIVE MOVES...
		// Checking Row 1 
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 0, 2, 1, playerPiece);
	if (spotFound == true) return;

	// Checking Row 2
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 3, 5, 1, playerPiece);
	if (spotFound == true) return;

	// Checking Row 3
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 6, 8, 1, playerPiece);
	if (spotFound == true) return;

  // Checking Col 1
  getMove(gridPos, enemyPiece, playerPiece, spotFound, 0, 6, 3, playerPiece);
	if (spotFound == true) return;	
	
	// Checking Col 2
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 1, 7, 3, playerPiece);
	if (spotFound == true) return;

  // Checking Col 3
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 2, 8, 3, playerPiece);
	if (spotFound == true) return;

  // Checking Dia 1
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 0, 8, 4, playerPiece);
	if (spotFound == true) return;

  // Checking Dia 2
	getMove(gridPos, enemyPiece, playerPiece, spotFound, 2, 6, 2, playerPiece);
	if (spotFound == true) return;

  
	while (!validSpot) {
    randomSpot = rand() % 9;
    if (isdigit(gridPos[randomSpot])) {
      validSpot = true;
      gridPos[randomSpot] = enemyPiece;
    }
	}
}





results checkStatus(std::vector<char> gridPos, results &currentResult, char piece, int &spotsTaken) {
  if( ((gridPos[0] == piece) && (gridPos[1] == piece) && (gridPos[2] == piece)) ) {
    return currentResult = Win;
  }
  else if( ((gridPos[3] == piece) && (gridPos[4] == piece) && (gridPos[5] == piece)) ) {
    return currentResult = Win;
  }
  else if( ((gridPos[6] == piece) && (gridPos[7] == piece) && (gridPos[8] == piece)) ) {
    return currentResult = Win;
  }
  else if( ((gridPos[0] == piece) && (gridPos[3] == piece) && (gridPos[6] == piece)) ) {
    return currentResult = Win;
  }
  else if( ((gridPos[1] == piece) && (gridPos[4] == piece) && (gridPos[7] == piece)) ) {
    return currentResult = Win;
  }
  else if( ((gridPos[2] == piece) && (gridPos[5] == piece) && (gridPos[8] == piece)) ) {
    return currentResult = Win;
  }
  else if( ((gridPos[0] == piece) && (gridPos[4] == piece) && (gridPos[8] == piece)) ) {
    return currentResult = Win;
  }
  else if( ((gridPos[2] == piece) && (gridPos[4] == piece) && (gridPos[6] == piece)) ) {
    return currentResult = Win;
  }
  else if(spotsTaken == 9) {
    return currentResult = Tie;
  }
  return currentResult;
}



int getNum() {
  int answer = 0;
  bool facts = true;
  while(facts) {
    //std::cout << " ";
    std::cin >> answer;
    if(std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(1000,'\n'); 
      std::cout << std::endl << "INVALID INPUT! try again!: "; 
      continue;
    }
    else {
      std::cin.ignore(1000,'\n'); 
      facts = false;
    }
  }
  return answer;
}
