/*

 File Battleship
 Owen Anderson and Ian Wallace
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define SIZE 10

struct allShips {
    int length;
    char id;
    char name[30];
    int life;
};

char playerBoard[SIZE][SIZE];
char compBoard[SIZE][SIZE];
char compHitBoard[SIZE][SIZE];
char alpha[10];
struct allShips ship[10];
char alpha[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
int targetSink = 0;
char targetSinkSymbol;
int chance = 100;

//prototypes
void printMenu(int); //prints out menu and gets user input for what they want to do
void printRules(void); //printing rules
void fillBoards(char c);; //Sets board with '_'
void printBoard(void); //prints out current board
void printHitBoard(void); //prints out board without given spaces
void placeShips(int, struct allShips ship[]); //Placing of player ships
void randomPlace(int numShips, struct allShips ship[], char board[SIZE][SIZE]); //randomly places ships on a board.
void winner(int); //prints winner
void playGame(int numShips, struct allShips ship[], char board[SIZE][SIZE],int playerLife[10], int compLife[10]); //Plays game
void saveGame(char board1[SIZE][SIZE], char board2[SIZE][SIZE], char board3[SIZE][SIZE]); //saves game to file
void loadGame(char board1[SIZE][SIZE], char board2[SIZE][SIZE], char board3[SIZE][SIZE]); //loads game from file
int compMove(int numShips, struct allShips ship[],int playerLife[10],int compMove); //computer move
int isValidShipPlacement(int x, int y, char playerBoard[SIZE][SIZE]); //Checks if the ships are placed in a valid location.
int checkHit(int hitChar, int hitY, int numShips, struct allShips ship[]); //checks for a hit
int checkCompHit(int x, int y, int numShips, struct allShips ship[]); //comp version of checking for a hit
int winLose(int numShips, struct allShips ship[], char board[SIZE][SIZE]); //checks to see if a winner or loser was determined
void winPrint(void); //prints winner
void loser(void);//prints loser
void printBattleshipTitle(void); //prints title of battleship

int main() {
    
    //seeding random number generator
    srand((unsigned int)time(NULL));
    
    //Read in ships file, contains ship info
    FILE *ifp = fopen("ships.txt", "r");
    
    
    
    //declaring variables
    int choice = 0;
    int playerLife[10], compLife[10];
    //Take in ships from file
    int i = 0;
    int numShips;
    fscanf(ifp, "%d", &numShips);
    
    // File input
    DIR *dir;
    struct dirent *ent;
    char files[numShips][30];
    if ((dir = opendir ("Sample folder")) != NULL) {
      /* print all the files and directories within directory */
        
        while ((ent = readdir (dir)) != NULL) {
            strncpy(&files[i][0],ent->d_name,30);
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }

    for(i=0; i<numShips; i++){
        fscanf(ifp, "%s %c %d", ship[i].name, &ship[i].id, &ship[i].length);
        ship[i].life = ship[i].length;
        playerLife[i] = ship[i].life;
        compLife[i] = ship[i].life;
    }
    
    printBattleshipTitle();
    
    while(choice != 4){
        printMenu(1);
        scanf("%d", &choice);
        
        if(choice == 3){
            printRules();
        }
        else if(choice == 2){
            loadGame(playerBoard, compBoard, compHitBoard);
            playGame(numShips, ship, compBoard,playerLife,compLife);
        }
        else if(choice == 1){
            fillBoards('_');
            placeShips(numShips, ship);
            playGame(numShips, ship, compBoard,playerLife,compLife);
        }
        choice = 4;
    }
    
    return 0;
}

void printMenu(int menu) {
    
    switch(menu) {
        case 1: //Print Main Menu
            printf("Welcome to Battleship, what would you like to do? (type in the menu choice number for your input)\n");
            printf("1. Play a new game of Battleship\n2. Load a saved game of Battleship\n3. Read the rules of Battleship in this format\n4. Exit the program\n");
            break;
        case 2: //Print options.
            printf("1. Place ships.\n2. Save game\n");
            break;
        case 3:
            printf("Would you like to place your ships yourself or have them randomly placed?\n");
            printf("1. Place manually.\n2. Place randomly.\n");
            break;
    }
    
}

void printRules(void) {
    printf("Rules:\n");
    printf("Battleship is a guessing game where the goal is to sink all of the enemy's ships. Because a player shouldn't be able to look at another human player's board so easily, the only mode available is player vs. computer. To play,the player will place his/her ships on the board, in strategic or random positions. The computer will do the same. The game will then\nstart and the player's goal will be to enter in points to 'fire at' where the opponent's ships may or may not be.\nThe winner is whoever sinks all of the other player's ships first.\n\n");
    main();
}

//Fills board with character
void fillBoards(char c) {
    int i, j;
    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++) {
            playerBoard[i][j] = c;
            compBoard[i][j] = c;
            compHitBoard[i][j] = c;
        }
    }
}

void printBoard(void){
    
    int i,j;
    printf("\n           ENEMY BOARD         \n********************************\n   ");
    for(i=1; i<=SIZE; i++)
        printf("%d  ", i);
    printf("\n");
    
    for(i = 0; i < SIZE; i++){
        printf("%c  ", alpha[i]);
        for(j = 0; j < SIZE; j++){
            printf("%c  ", compBoard[i][j]);
            if(j == 9) printf("\n");
        }
    }
    printf("\n          YOUR BOARD         \n********************************\n   ");
    for(i=1; i<=SIZE; i++)
        printf("%d  ", i);
    printf("\n");
    
    for(i = 0; i < SIZE; i++){
        printf("%c  ", alpha[i]);
        for(j = 0; j < SIZE; j++){
            printf("%c  ", playerBoard[i][j]);
            if(j == 9) printf("\n");
        }
    }
    printf("\n");
}

//Function to find where the player wants his/her ships to be
void placeShips(int numShips, struct allShips ship[]) {
    
    int currentShip = 0;
    int randomchoice;
    
    //Place computer ships
    randomPlace(numShips, ship, compBoard);
    
    printMenu(3);
    scanf("%d", &randomchoice);
    
    if (randomchoice == 2) {
        randomPlace(numShips, ship, playerBoard);
    } else {
        while(currentShip<numShips) {
            
            printHitBoard();
            
            int i;
            int x, y;
            int validDirection = 0;
            char direction, alphaY;
            
            while(!validDirection) {
                printf("Where would you like the beginning of your %s to be? (Format: letter num i.e 'A 5')\n", ship[currentShip].name);
                scanf(" %c %d", &alphaY, &x);
                x = x - 1;
                //Covert second coord from letter to num if letter is entered.
                if(alphaY >= 97) y = alphaY-97;
                else y = alphaY-65;
                
                printf("Which direction would you like your %s to face? ('l' for left, 'r' for right, 'u' for up, 'd' for down)\n", ship[currentShip].name);
                scanf(" %c", &direction);
                
                //different cases for ship position
                if(direction == 'r') {
                    
                    //check if valid placement for right direction
                    for(i = x; i < ship[currentShip].length+x; i++) {
                        if(!isValidShipPlacement(i, y, playerBoard)) {
                            validDirection = 0;
                            printf("That is not a valid placement.\nPlease enter a valid placement.\n");
                            break;
                        } else {
                            validDirection = 1;
                        }
                    }
                    if(validDirection) {
                        for(i = x; i < ship[currentShip].length+x; i++) {
                            playerBoard[y][i] = ship[currentShip].id;
                        }
                    }
                    
                } else if(direction == 'l') {
                    
                    //check if valid placement for left direction
                    for(i = x; i > x-ship[currentShip].length; i--) {
                        if(!isValidShipPlacement(i, y, playerBoard)) {
                            validDirection = 0;
                            printf("That is not a valid placement.\nPlease enter a valid placement.\n");
                            break;
                        } else {
                            validDirection = 1;
                        }
                    }
                    if(validDirection) {
                        for(i = x; i > x-ship[currentShip].length; i--) {
                            playerBoard[y][i] = ship[currentShip].id;
                        }
                    }
                    
                } else if(direction == 'u') {
                    
                    //check if valid placement for up direction
                    for(i = y; i > y-ship[currentShip].length; i--) {
                        if(!isValidShipPlacement(x, i, playerBoard)) {
                            validDirection = 0;
                            printf("That is not a valid placement.\nPlease enter a valid placement.\n");
                            break;
                        } else {
                            validDirection = 1;
                        }
                    }
                    if(validDirection) {
                        for(i = y; i > y-ship[currentShip].length; i--) {
                            playerBoard[i][x] = ship[currentShip].id;
                        }
                    }
                    
                } else if(direction == 'd') {
                    
                    //check if valid placement for down direction
                    for(i = y; i < ship[currentShip].length+y; i++) {
                        if(!isValidShipPlacement(x, i, playerBoard)) {
                            validDirection = 0;
                            printf("That is not a valid placement.\nPlease enter a valid placement.\n");
                            break;
                        } else {
                            validDirection = 1;
                        }
                    }
                    if(validDirection) {
                        for(i = y; i < ship[currentShip].length+y; i++) {
                            playerBoard[i][x] = ship[currentShip].id;
                        }
                    }
                    
                } else {
                    printf("That is not a valid placement. Please enter a valid placement.\n");
                }
            }
            currentShip++;
        }
    }
}

int isValidShipPlacement(int x, int y, char board[SIZE][SIZE]) {
    //Checks if another ship is already placed there or the ship is placed out of bounds.
    if(board[y][x]!='_' || x > SIZE-1 || y > SIZE-1 || x < 0 || y < 0) return 0;
    return 1;
}

void randomPlace(int numShips, struct allShips ship[], char board[SIZE][SIZE]) {
    
    int i, rndDirection, x, y, currentShip = 0;
    
    while(currentShip < numShips) {
        
        int check = 0;
        x = rand() % 10;
        y = rand() % 10;
        rndDirection = rand() % 4 + 1;
        
        if(rndDirection==1) {
            for(i = x; i < ship[currentShip].length+x; i++) {
                if(isValidShipPlacement(i, y, board)) {
                    check++;
                }
                if(check == ship[currentShip].length) {
                    for(i = x; i < ship[currentShip].length+x; i++) {
                        board[y][i] = ship[currentShip].id;
                    }
                    currentShip++;
                }
            }
        }
        
        if(rndDirection==2) {
            for(i = x; i > x-ship[currentShip].length; i--) {
                if(isValidShipPlacement(i, y, board)) {
                    check++;
                }
                if(check == ship[currentShip].length) {
                    for(i = x; i > x-ship[currentShip].length; i--) {
                        board[y][i] = ship[currentShip].id;
                    }
                    currentShip++;
                }
            }
        }
        
        if(rndDirection==3) {
            for(i = y; i < ship[currentShip].length+y; i++) {
                if(isValidShipPlacement(x, i, board)) {
                    check++;
                }
                if(check == ship[currentShip].length) {
                    for(i = y; i < ship[currentShip].length+y; i++) {
                        board[i][x] = ship[currentShip].id;
                    }
                    currentShip++;
                }
            }
        }
        
        if(rndDirection==4) {
            for(i = y; i > y-ship[currentShip].length; i--) {
                if(isValidShipPlacement(x, i, board)) {
                    check++;
                }
                if(check == ship[currentShip].length) {
                    for(i = y; i > y-ship[currentShip].length; i--) {
                        board[i][x] = ship[currentShip].id;
                    }
                    currentShip++;
                }
            }
        }
        
    }
}
// hit is '*', miss is '@'
void playGame(int numShips, struct allShips ship[], char board[SIZE][SIZE], int playerLife[],int compLife[]) {
    int check = -1,choice,difficultyChoice = 0, difficulty = 0,turn = 0,checkWin = -1, x, y, computerMove, i;
    char hitChar,tmp;
    printf("Please select a difficulty for the computer\n1. Easy\n2. Hard\n3. Extreme\n4. How are the difficulties ranked?\n");
    scanf("%d", &difficultyChoice);
    
    //continues to ask for input until a valid choice is inputted
    while(difficultyChoice >= 4 || difficultyChoice < 1){
        if(difficultyChoice != 4){
        printf("Please select a valid option\n");
        scanf("%d", &difficultyChoice);
        }
        else{
            printf("1. The computer will randomly select points to hit for the entire game\n2. The computer will randomly select points to hit, and if it gets a hit, it will continue to get hits on that ship until it sinks the specific ship\n3. The ship has a 25 percent chance to get a hit, which goes up every time it gets a miss, maxing out at 50 percent (keep in mind it will not target sinking, as that would be too hard for the player to beat\n");
            scanf("%d", &difficultyChoice);
        }
    }
    //setting difficulty for the choice
    if(difficultyChoice == 1) difficulty = 1;
    else if(difficultyChoice == 2) difficulty = 2;
    else if(difficultyChoice == 3) difficulty = 3;

    //continues to play game until a winner is decided
    while(checkWin != 0){
        printf("-----------TURN #%d-----------\n", turn+1);
        printHitBoard();
        printf("Would you like to:\n------------------\n1. Select a coordinate to hit\n2. Print the current board\n3. Save your current game\n4. Surrender\n");
        scanf("%d", &choice);
        
        if(choice == 1){
            
            int isValid = 0;
            printf("Select a coordinate to hit (letter num ie 'A 5')\n");
            
            do {
                scanf(" %s %d", &hitChar, &y);
                
                if(hitChar >= 97) x = hitChar-97;
                else x = hitChar-65;
                
                //makes sure user cannot hit a coordinate again after hitting it once, and makes sure the coordinates are valid
                if(((hitChar >= 97 && hitChar <= 106) || (hitChar >= 65 && hitChar <= 74) ) && (compBoard[x][y-1] != '@' && compBoard[x][y-1] != '*'))
                    isValid = 1;
                else printf("That is not a valid coordinate. Please enter a different coordinate.\n");
                
            } while(!isValid);
            
            y--; //makes y coordinate valid on board (starts at 1 for board)
            tmp = compBoard[x][y];
            check = checkHit(x, y,numShips, ship); //check sees if ship was hit
            if(check == 1){
                    printf("\nYou got a hit!!\n");
                    for(i = 0; i< numShips; i++){
                        if(ship[i].id == tmp){ //goes through all different ship chars and finds the one it is equal to
                            compLife[i] --; //takes one away from the "life" of the ship aka the length, once it hits 0 it counts as being sunk
                            if(compLife[i] == 0){
                                printf("You sunk the opponent's %s!\n", ship[i].name);
                            }
                        }
                    }
            }
            else{
                printf("Miss!\n");
            }
            //get computer move once player move is over
            computerMove = compMove(numShips, ship,playerLife, difficulty);
            turn++;
            
        }
        else if(choice == 2)
            printHitBoard();
        else if(choice == 3)
            saveGame(playerBoard, compBoard, compHitBoard);
        else if(choice == 4){
            printf("You surrendered, here is what the boards looked like:\n");
            printBoard();
            loser();
            checkWin = 0;
            break;
        }
        checkWin = winLose(numShips, ship, compBoard);
        if(checkWin == 1){
            winPrint();
            checkWin = 0;
        }
        if(checkWin == -1){
            loser();
            checkWin = 0;
        }
        
    }
    
}
//Does the computer move for the turn
int compMove(int numShips, struct allShips ship[], int playerLife[], int difficulty){
    int x = 0, y = 0,z,i,j,check, tmp,randHit;
    
    if(difficulty == 1 || difficulty == 2){
        if(targetSink == 1){
            for(i = 0; i < SIZE; i++){
                for(j = 0; j < SIZE; j++){
                    //goes through the loop, and once it finds the mark, it'll hit it
                    if(playerBoard[i][j] == targetSinkSymbol ){
                        x = i;
                        y = j;
                        break;
                    }
                }
            }
        }
        else{
            //hits at random places on the board
                x = rand() % 10;
                y = rand() % 10;
                //Checks if another ship is already placed there or the ship is placed out of bounds.
                while(playerBoard[x][y] == '*'){
                        x = rand() % 10;
                        y = rand() % 10;
                }
        }
    }
    //code for the extreme difficulty
    else{
        //random number from 1-100 and if it is less than 25 (a 25% chance) then it will hit a point of a ship
        randHit = rand() % chance + 1;
        if(randHit <= 25 && chance >= 50){
            printf("%d\n", randHit);
            for(z = 0; z < numShips; z++){
                for(i = 0; i < SIZE; i++){
                    for(j = 0; j < SIZE; j++){
                        if(playerBoard[i][j] == ship[z].id){
                        x = i;
                        y = j;
                        break;
                    }
                }
            }
            }
            }
        else{
            x = rand() % 10;
            y = rand() % 10;
            //Checks if another ship is already placed there or the ship is placed out of bounds.
            while(playerBoard[x][y] != '_'){
                x = rand() % 10;
                y = rand() % 10;
            }
            //if no hit, then the percent goes up by 1, being limited at 50
            chance--;
            
        }
    }
    //tmp is to check for the ship symbol
    tmp = playerBoard[x][y];
    
    check = checkCompHit(x, y, numShips, ship);
    
    if(check == 1){
        if(difficulty == 2){
        targetSink = 1;
        targetSinkSymbol = tmp;
        }
        printf("\nThe computer got a hit!\n");
        for(i = 0; i< numShips; i++){
            if(ship[i].id == tmp){ //goes through all different ship chars and finds the one it is equal to
                playerLife[i] --; //takes one away from the "life" of the ship aka the length, once it hits 0 it counts as being sunk
                if(playerLife[i] == 0){
                    printf("The computer sunk your %s!\n", ship[i].name);
                    targetSink = 0;
                }
            }
        }
        return 1;
    }
    printf("\nThe computer missed!\n");
    return 0;
}
//same as the regular checkHit but modifies the playerBoard instead
int checkCompHit(int x, int y, int numShips, struct allShips ship[]){
    if(playerBoard[x][y] != '_' && playerBoard[x][y] != '*' && playerBoard[x][y] != '@'){
        playerBoard[x][y] = '*';
        return 1;
    }
    else{
        playerBoard[x][y] = '@';
        return 0;
    }
    return 0;
}
int winLose(int numShips, struct allShips ship[], char board[SIZE][SIZE]){
    int i, j,z, playerWin = 0, compWin = 0;
    //player win check
    for(z = 0; z < numShips; z++){
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++) {
                if(compBoard[i][j] == ship[z].id) playerWin = 1;
            }
        }
    }
    if(playerWin == 0) return 1;
    
    //computer win check
    for(z = 0; z < numShips; z++){
        for(i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++) {
                if(playerBoard[i][j] == ship[z].id) compWin = 1;
            }
        }
    }
    if(compWin == 0) return -1;
    
    return 2;
}

//checks to see if the coordinate was a hit and sets it to * for a hit and @ for a miss, respectively
int checkHit(int x, int y, int numShips, struct allShips ship[]){
    if(compBoard[x][y] != '_' && compBoard[x][y] != '*' && compBoard[x][y] != '@' && compBoard[x][y] != '+'){
        compBoard[x][y] = '*';
        compHitBoard[x][y] = '*';
        return 1;
    }
    else{
        compBoard[x][y] = '@';
        compHitBoard[x][y] = '@';
        return 0;
    }
    return 0;
}



//prints the board that the player sees in game AKA can't see the computer's ships
void printHitBoard(void){
    
    int i,j;
    printf("\n           ENEMY BOARD         \n********************************\n   ");
    for(i=1; i<=SIZE; i++)
        printf("%d  ", i);
    printf("\n");
    
    for(i = 0; i < SIZE; i++){
        printf("%c  ", alpha[i]);
        for(j = 0; j < SIZE; j++){
            printf("%c  ", compHitBoard[i][j]);
            if(j == 9) printf("\n");
        }
    }
    printf("\n          YOUR BOARD         \n********************************\n   ");
    for(i=1; i<=SIZE; i++)
        printf("%d  ", i);
    printf("\n");
    
    for(i = 0; i < SIZE; i++){
        printf("%c  ", alpha[i]);
        for(j = 0; j < SIZE; j++){
            printf("%c  ", playerBoard[i][j]);
            if(j == 9) printf("\n");
        }
    }
    printf("\n");
}

void winPrint(void){
    printf("\n\n-------------------\n------YOU WIN------\n-------------------\n");
}
void loser(void){
    printf("\n\n--------------------\n------YOU LOSE------\n--------------------\n");
}

void printBattleshipTitle(void) {
    printf("\n888888b.            888    888    888                   888      d8b\n");
    printf("888  \"88b           888    888    888                   888      Y8P          \n");
    printf("888  .88P           888    888    888                   888                   \n");
    printf("8888888K.   8888b.  888888 888888 888  .d88b.  .d8888b  88888b.  888 88888b.  \n");
    printf("888  \"Y88b     \"88b 888    888    888 d8P  Y8b 88K      888 \"88b 888 888 \"88b \n");
    printf("888    888 .d888888 888    888    888 88888888 \"Y8888b. 888  888 888 888  888 \n");
    printf("888   d88P 888  888 Y88b.  Y88b.  888 Y8b.          X88 888  888 888 888 d88P \n");
    printf("8888888P\"  \"Y888888  \"Y888  \"Y888 888  \"Y8888   88888P' 888  888 888 88888P\"  \n");
    printf("                                                                     888      \n");
    printf("                                                                     888      \n");
    printf("                                                                     888      \n\n");
}

void saveGame(char board1[SIZE][SIZE], char board2[SIZE][SIZE], char board3[SIZE][SIZE]) {
    char fileName[251];
    
    printf("What would you like to name your save file? (no spaces)\n");
    scanf("%s", fileName);
    
    strcat(fileName, ".txt");
    
    FILE *ofp = fopen(fileName, "w");
    
    int i, j;
    
    //Save player board
    for(i=0; i<SIZE; i++)
        for(j=0; j<SIZE; j++) {
            fprintf(ofp, " %c", board1[i][j]);
            if(j==9) fprintf(ofp, "\n");
        }
    
    //Save computer board
    for(i=0; i<SIZE; i++)
        for(j=0; j<SIZE; j++) {
            fprintf(ofp, " %c", board2[i][j]);
            if(j==9) fprintf(ofp, "\n");
        }
    
    //Save comp hit board
    for(i=0; i<SIZE; i++)
        for(j=0; j<SIZE; j++) {
            fprintf(ofp, " %c", board3[i][j]);
            if(j==9) fprintf(ofp, "\n");
        }
    
    fclose(ofp);
    
    printf("...Game Saved\n");
}

void loadGame(char board1[SIZE][SIZE], char board2[SIZE][SIZE], char board3[SIZE][SIZE]) {
    
    int i, j;
    char fileName[251];
    
    printf("What is the name of the file you would like to load?\n");
    scanf("%s", fileName);
    
    strcat(fileName, ".txt");
    
    FILE *jfp = fopen(fileName, "r");
    
    for(i=0; i<SIZE; i++) //load player board
        for(j=0; j<SIZE; j++)
            fscanf(jfp, " %c", &board1[i][j]);
    
    for(i=0; i<SIZE; i++) //load computer board
        for(j=0; j<SIZE; j++)
            fscanf(jfp, " %c", &board2[i][j]);
    
    for(i=0; i<SIZE; i++) //Save comp hit board
        for(j=0; j<SIZE; j++)
            fscanf(jfp, " %c", &board3[i][j]);
    
    fclose(jfp);
}
