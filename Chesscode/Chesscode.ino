/*
 * Initialize board by saving 32 locations in a 2 seprates arrays(8x2) 
 * Each location on the board is represeted by 2 numbers from 0-7
 * Run a loop (64) through the 6 bit mux to scan the board. Requirements: 6 d outputs for MUX Select lines, 1 d input for MUX output.  
 * Mapping: Pawn - p, Rook -r, bishop -b, Horse -h, King-k, Queen-q 
 * 
 * scan, compare, indentifier, 
 */
#define MUX 2
#define MOVE 3  //Digital input, 0-White 1-Black

int pieceloc[2];
int temp1[8][8], temp2[8][8];
int Pins[6] = {8,9,10,11,12,7};
int maxCount = 64;
int TURN;

int white[2][8][2] = {
  {{1,0},{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7}}, //pawns
  {{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7}}, //rook- horse- bishop - king - queen - bishop - horse- rook
};

int black[2][8][2] = {
  {{6,0},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7}}, //pawns
  {{7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7}}, //rook- horse- bishop - king - queen - bishop - horse- rook
};



void scan(int temp[8][8]){ //works 
  int i,j;
  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      muxBinary((i*8)+j);  //selecting the desired output
      delay(5);  //to make sure output actually physically changes
      temp[i][j] = digitalRead(MUX);  //storing the location the pieces.
      //Serial.print(temp[i][j]); 
    }
    //Serial.print(" In row :"); Serial.println(i);
  } 
}

void muxBinary(byte numToShow){
  for(int i=0; i<7; i++){
    if(bitRead(numToShow, i) == 1){
      digitalWrite(Pins[i], HIGH);
    }
    else{
      digitalWrite(Pins[i], LOW);
    }  
  }   
}

int compare(int prev[8][8], int curr[8][8]){
  int i,j;
  for(i=0; i<8; i++){
    for(j=0; j<8; j++){
      if(prev[i][j] != curr[i][j]){
        return i*8+j;   //to retrive i = x/8; j = x%8;    
      }
    }
  }
  return -1;  
}

char identify(int changei, int changej){
  int i,j;
  if(TURN == LOW){
    for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        if(white[i][j][0] == changei and white[i][j][1] == changej){
          return mapper(i,j);
        } 
      }
    }
    return 'n';
  }

  if(TURN == HIGH){
    for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        if(black[i][j][0] == changei and black[i][j][1] == changej){
          return mapper(i,j);
        }
      }
    }
    return 'n';
  }
}

char mapper(int i,int j){
  if(i == 0){
    return 'p';
  }
  if(i ==1){
    switch(j){
      case 0: 
        return 'r';
        break;
      case 1: 
        return 'h';
        break;
      case 2: 
        return 'b';
        break;
      case 3: 
        return 'k';
        break; 
      case 4: 
        return 'q';
        break;
      case 5: 
        return 'b';
        break;
      case 6: 
        return 'h';
        break;
      case 7: 
        return 'r';
        break;
    }
  }
}

char identifyn(int changei, int changej){
  int i,j;
  if(TURN == LOW){
    for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        if(white[i][j][0] == changei and white[i][j][1] == changej){
          return mappern(i,j);
        } 
      }
    }
    return 'n';
  }

  if(TURN == HIGH){
    for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        if(black[i][j][0] == changei and black[i][j][1] == changej){
          return mappern(i,j);
        }
      }
    }
    return 'n';
  }
}

char mappern(int i,int j){
  if(i == 0){
    pawn(pieceloc, temp2);
    return 'p';
  }
  if(i ==1){
    switch(j){
      case 0: 
        rook(pieceloc, temp2);
        return 'r';
        break;
      case 1: 
        horse(pieceloc, temp2);
        return 'h';
        break;
      case 2: 
        bishop(pieceloc, temp2);
        return 'b';
        break;
      case 3: 
        king(pieceloc, temp2);
        return 'k';
        break; 
      case 4: 
        queen(pieceloc, temp2);
        return 'q';
        break;
      case 5: 
        bishop(pieceloc, temp2);
        return 'b';
        break;
      case 6: 
        horse(pieceloc, temp2);
        return 'h';
        break;
      case 7: 
        rook(pieceloc, temp2);
        return 'r';
        break;
    }
  }
}

void move_kill(int changei, int changej, int changeii, int changejj, int kill){
  int i,j;
  if(TURN == LOW){
    for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        if(white[i][j][0] == changei and white[i][j][1] == changej){
          white[i][j][0] = changeii;
          white[i][j][1] = changejj;
        } 
      }
    }
    if(kill == 1){
      for(i=0; i<2; i++){
        for(j=0; j<8; j++){
          if(black[i][j][0] == changeii and black[i][j][1] == changejj){
            black[i][j][0] = -1;
            black[i][j][1] = -1;
          } 
        }
      }        
    }
  }

  if(TURN == HIGH){
    for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        if(black[i][j][0] == changei and black[i][j][1] == changej){
          black[i][j][0] = changeii;
          black[i][j][1] = changejj;
          //Serial.println("Movment has occured here man!");
        }
      }
    }
    if(kill == 1){
      for(i=0; i<2; i++){
        for(j=0; j<8; j++){
          if(white[i][j][0] == changeii and white[i][j][1] == changejj){
            white[i][j][0] = -1;
            white[i][j][1] = -1;
          } 
        }
      }
    }
  }
}

void printmem(){
  int i,j;
  Serial.println("white:  ");
  for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        Serial.print(white[i][j][0]); Serial.print(white[i][j][1]); Serial.print("   ");
      }
  }
  Serial.println(" ");
  Serial.println("black:   ");
  for(i=0; i<2; i++){
      for(j=0; j<8; j++){
        Serial.print(black[i][j][0]); Serial.print(black[i][j][1]); Serial.print("   ");
      }
  }
  Serial.println(" ");
}

void printboard(int board[8][8]){
  Serial.println("Chess board is currently: ");
  int i,j;
  for(i=7;i>-1;i--){
    for(j=0;j<8;j++){
      Serial.print(board[i][j]); 
    }
    Serial.print("  row :"); Serial.println(i);
  } 
  
  
  }


void pawn(int loc[2], int board[8][8]){ //loc tells the position in 8x8 board is the map of pieces.
  int i,j;
  i = loc[0];
  j = loc[1];
  Serial.print("Possible moves from ["); Serial.print(i); Serial.print(", "); Serial.print(j); Serial.println("]  are: ");
  if(TURN == LOW){ //white pawn will move only forward 
    //move forward
    if(-1<i+1 and i+1<8 and -1<j and j<8){  //boundary conditions
      if(board[i+1][j] == HIGH){  //if there is conflict
        Serial.print("Cannot move pawn forward to ["); Serial.print(i+1); Serial.print(", "); Serial.print(j); Serial.println("]   ");
      }
      else{
        Serial.print("Move pawn forward to ["); Serial.print(i+1); Serial.print(", "); Serial.print(j); Serial.println("]   ");
      }
    }

    //Kill left diag
    if(-1<i+1 and i+1<8 and -1<j-1 and j-1<8){  //boundary conditions
      if(board[i+1][j-1] == HIGH){  //if there is conflict
        
        char z = identify(i+1,j-1); //check to see if any piece exists and if it exists see if its same team or opposite team!
        if(z == 'n'){ //this means that piece is of opposite team so kill is possible!
          Serial.print("Kill possible by pawn at ["); Serial.print(i+1); Serial.print(", "); Serial.print(j-1); Serial.println("]   ");     
        }           
      }
      else{
        Serial.print("Cannot move pawn forward to ["); Serial.print(i+1); Serial.print(", "); Serial.print(j-1); Serial.println("]   ");
      }
    }  

    //Kill right diag
    if(-1<i+1 and i+1<8 and -1<j+1 and j+1<8){  //boundary conditions
      if(board[i+1][j+1] == HIGH){  //if there is conflict
        char z = identify(i+1,j+1); //check to see if any piece exists and if it exists see if its same team or opposite team!
        if(z == 'n'){ //this means that piece is of opposite team so kill is possible!
          Serial.print("Kill possible by pawn at ["); Serial.print(i+1); Serial.print(", "); Serial.print(j+1); Serial.println("]   ");
        } 
      }
      else{
        Serial.print("Cannot move pawn forward to ["); Serial.print(i+1); Serial.print(", "); Serial.print(j+1); Serial.println("]   ");
      }
    }
    Serial.println("Make your move!");
  }
  
  if(TURN == HIGH){
    //move forward
    
    if(-1<i-1 and i-1<8 and -1<j and j<8){  //boundary conditions
      if(board[i-1][j] == HIGH){  //if there is conflict
        Serial.print("Cannot move pawn forward to ["); Serial.print(i-1); Serial.print(", "); Serial.print(j); Serial.println("]   ");
      }
      else{
        Serial.print("Move pawn forward to ["); Serial.print(i-1); Serial.print(", "); Serial.print(j); Serial.println("]   ");
      }
    }

    //Kill left diag
    if(-1<i-1 and i-1<8 and -1<j-1 and j-1<8){  //boundary conditions
      if(board[i-1][j-1] == HIGH){  //if there is conflict
        char z = identify(i-1,j-1); //check to see if any piece exists and if it exists see if its same team or opposite team!
        if(z == 'n'){ //this means that piece is of opposite team so kill is possible!
          Serial.print("Kill possible by pawn at ["); Serial.print(i-1); Serial.print(", "); Serial.print(j-1); Serial.println("]   ");
        }
      }
      else{
          Serial.print("Cannot move pawn forward to ["); Serial.print(i-1); Serial.print(", "); Serial.print(j-1); Serial.println("]   ");
        }
      } 

    //Kill right diag
    if(-1<i-1 and i-1<8 and -1<j+1 and j+1<8){  //boundary conditions
      if(board[i-1][j+1] == HIGH){  //if there is conflict
        char z = identify(i-1,j+1); //check to see if any piece exists and if it exists see if its same team or opposite team!
        if(z == 'n'){ //this means that piece is of opposite team so kill is possible!
          Serial.print("Kill possible by pawn at ["); Serial.print(i-1); Serial.print(", "); Serial.print(j+1); Serial.println("]   ");
        } 
      }
      else{
          Serial.print("Cannot move pawn forward to ["); Serial.print(i-1); Serial.print(", "); Serial.print(j+1); Serial.println("]   ");
      }
    } 
  Serial.println("Make your move!");    
  }
}


void rook(int loc[2], int board[8][8]){
  //Rook can move in 4 directions, up -down-left-right, irrespective of its color
  int i,j,x;
  i = loc[0];
  j = loc[1];
  int move4[4] = {7,0,7,0}; //stores the extremes of i in first 2 and extreme of j in last 2.

  //move the rook upwards in limit
  for(x=i+1; x<8; x++){  //boundary conditions + move
    if(board[x][j] == HIGH){    //if there is conflict we need to stop movement over there!
      char z = identify(x,j);
      if(z == 'n'){  //if opponent piece is there! kill is possible
        Serial.print("Kill possible by rook at ["); Serial.print(x); Serial.print(", "); Serial.print(j); Serial.println("]   ");
        move4[0] = x;
        break;
      }
      else{ //which means that friendly piece is there, so we can move one block before it.
        move4[0] = x-1;
        break;
      }
    }
  }

  //move the rook downwards in limit
  for(x=i-1; x>-1; x--){  //boundary conditions + move
    if(board[x][j] == HIGH){    //if there is conflict we need to stop movement over there!
      char z = identify(x,j);
      if(z == 'n'){  //if opponent piece is there! kill is possible
        Serial.print("Kill possible by rook at ["); Serial.print(x); Serial.print(", "); Serial.print(j); Serial.println("]   ");
        move4[1] = x;
        break;
      }
      else{ //which means that friendly piece is there, so we can move one block before it.
        move4[0] = x+1;
        break;
      }
    }
  }

  //move the rook rightwards in limit
  for(x=j+1; x<8; x++){  //boundary conditions + move
    if(board[i][x] == HIGH){    //if there is conflict we need to stop movement over there!
      char z = identify(i,x);
      if(z == 'n'){  //if opponent piece is there! kill is possible
        Serial.print("Kill possible by rook at ["); Serial.print(i); Serial.print(", "); Serial.print(x); Serial.println("]   ");
        move4[2] = x;
        break;
      }
      else{ //which means that friendly piece is there, so we can move one block before it.
        move4[2] = x-1;
        break;
      }
    }
  }

  //move the rook leftwards in limit
  for(x=j-1; x>-1; x--){  //boundary conditions+move
    if(board[i][x] == HIGH){    //if there is conflict we need to stop movement over there!
      char z = identify(i,x);
      if(z == 'n'){  //if opponent piece is there! kill is possible
        Serial.print("Kill possible by rook at ["); Serial.print(i); Serial.print(", "); Serial.print(x); Serial.println("]   ");
        move4[3] = x;
        break;
      }
      else{ //which means that friendly piece is there, so we can move one block before it.
        move4[3] = x+1;
        break;
      }
    }
  }
  
  Serial.println("Possible moves are: ");
  Serial.print("Move rook upto ["); Serial.print(move4[0]); Serial.print(", "); Serial.print(j); Serial.println("]   ");
  Serial.print("Move rook upto ["); Serial.print(move4[1]); Serial.print(", "); Serial.print(j); Serial.println("]   ");
  Serial.print("Move rook upto ["); Serial.print(i); Serial.print(", "); Serial.print(move4[2]); Serial.println("]   ");
  Serial.print("Move rook upto ["); Serial.print(i); Serial.print(", "); Serial.print(move4[3]); Serial.println("]   ");  
  Serial.print("Make your move!");
}

void horse_fun(int i, int j, int board[8][8]){
  if(-1<i and i<8 and -1<j and j<8){ //boundary conditions
    if(board[i][j] == HIGH){ //see if there is a conflict
      char z = identify(i,j);
      if(z == 'n'){ //If opponent piece is there kill is possible!
        Serial.print("Kill possible by horse at ["); Serial.print(i); Serial.print(", "); Serial.print(j); Serial.println("]   ");
      }
    }
    else{ //no conflict is there so we can move
      Serial.print("Move horse to ["); Serial.print(i); Serial.print(", "); Serial.print(j); Serial.println("]   ");
    }
  }
}

void horse(int loc[2], int board[8][8]){ 
  //8 possible locations to go to, so we define a single function irrespective of color
  int i,j;
  i = loc[0];
  j = loc[1];


  //now call the function 8 times for each possible location
  Serial.println("Possible moves are: ");
  horse_fun(i+2,j+1,board);
  horse_fun(i+2,j-1,board);
  horse_fun(i-2,j+1,board);
  horse_fun(i-2,j-1,board);
  horse_fun(i+1,j+2,board);
  horse_fun(i+1,j-2,board);
  horse_fun(i-1,j+2,board);
  horse_fun(i-1,j-2,board); 
  Serial.println("Make your move!");
}


void bishop(int loc[2], int board[8][8]){
  //bishop can move in 4 possible directions
  int i,j,x;
  i = loc[0];
  j = loc[1];
  int move4[4][2] = {{i,j},{i,j},{i,j},{i,j}};  //initalize all of them to 
  
  for(x=1; x<8; x++){  //move piece diagonally up-right
    if(-1<i+x and i+x<8 and -1<j+x and j+x<8){
      if(board[i+x][j+x] == HIGH){ //conflict is there so we need to stop movement and check for opponent
        char z = identify(i+x,j+x);
        if(z == 'n'){ //If opponent piece is there kill is possible!
          move4[0][0] = i+x;  move4[0][1] = j+x;
          Serial.print("Kill possible by bishop at ["); Serial.print(i+x); Serial.print(", "); Serial.print(j+x); Serial.println("]   "); 
          break;
        }
        else{ //own piece is there so move till one before
          move4[0][0] = i+x-1;  move4[0][1] = j+x-1;
        }
      }
      else{ //conflict is not there so keep moving
        move4[0][0] = i+x;
        move4[0][1] = j+x;
      }  
    } 
  }

  for(x=1; x<8; x++){  //move piece diagonally up-left
    if(-1<i+x and i+x<8 and -1<j-x and j-x<8){
      if(board[i+x][j-x] == HIGH){ //conflict is there so we need to stop movement and check for opponent
        char z = identify(i+x,j-x);
        if(z == 'n'){ //If opponent piece is there kill is possible!
          move4[1][0] = i+x;  move4[1][1] = j-x;
          Serial.print("Kill possible by bishop at ["); Serial.print(i+x); Serial.print(", "); Serial.print(j-x); Serial.println("]   "); 
          break;
        }
        else{ //own piece is there so move till one before
          move4[1][0] = i+x-1;  move4[1][1] = j-x+1;
        }
      }
      else{ //conflict is not there so keep moving
        move4[0][0] = i+x;
        move4[0][1] = j-x;
      }  
    } 
  }

  for(x=1; x<8; x++){  //move piece diagonally down-right
    if(-1<i-x and i-x<8 and -1<j+x and j+x<8){
      if(board[i-x][j+x] == HIGH){ //conflict is there so we need to stop movement and check for opponent
        char z = identify(i-x,j+x);
        if(z == 'n'){ //If opponent piece is there kill is possible!
          move4[2][0] = i-x;  move4[2][1] = j+x;
          Serial.print("Kill possible by bishop at ["); Serial.print(i-x); Serial.print(", "); Serial.print(j+x); Serial.println("]   "); 
          break;
        }
        else{ //own piece is there so move till one before
          move4[2][0] = i-x+1;  move4[2][1] = j+x-1;
        }
      }
      else{ //conflict is not there so keep moving
        move4[2][0] = i-x;
        move4[2][1] = j+x;
      }  
    } 
  }

  for(x=1; x<8; x++){  //move piece diagonally up-left
    if(-1<i-x and i-x<8 and -1<j-x and j-x<8){
      if(board[i-x][j-x] == HIGH){ //conflict is there so we need to stop movement and check for opponent
        char z = identify(i-x,j-x);
        if(z == 'n'){ //If opponent piece is there kill is possible!
          move4[3][0] = i-x;  move4[3][1] = j-x;
          Serial.print("Kill possible by bishop at ["); Serial.print(i-x); Serial.print(", "); Serial.print(j-x); Serial.println("]    "); 
          break;
        }
        else{ //own piece is there so move till one before
          move4[3][0] = i-x+1;  move4[3][1] = j-x+1;
        }
      }
      else{ //conflict is not there so keep moving
        move4[3][0] = i-x;
        move4[3][1] = j-x;
      }  
    } 
  }

  Serial.println("Possible moves are: ");
  Serial.print("Move bishop upto ["); Serial.print(move4[0][0]); Serial.print(", "); Serial.print(move4[0][1]); Serial.println("]   ");
  Serial.print("Move bishop upto ["); Serial.print(move4[1][0]); Serial.print(", "); Serial.print(move4[1][1]); Serial.println("]   ");
  Serial.print("Move bishop upto ["); Serial.print(move4[2][0]); Serial.print(", "); Serial.print(move4[2][1]); Serial.println("]   ");
  Serial.print("Move bishop upto ["); Serial.print(move4[3][0]); Serial.print(", "); Serial.print(move4[3][1]); Serial.println("]   ");
  Serial.println("Make your move!");
}


void king_fun(int i, int j, int board[8][8]){
  if(-1<i and i<8 and -1<j and i<8){ //boundary conditions
    if(board[i][j] == HIGH){ //see if there is a conflict
      char z = identify(i,j);
      if(z == 'n'){ //If opponent piece is there kill is possible!
        Serial.print("Kill possible by king at ["); Serial.print(i); Serial.print(", "); Serial.print(j); Serial.println("]   ");
      }
    }
    else{ //no conflict is there so we can move
      Serial.print("Move king to ["); Serial.print(i); Serial.print(", "); Serial.print(j); Serial.println("]   ");
    }
  }    
}
  
void king(int loc[2], int board[8][8]){
  //king can move to 8 possible locations around it
  int i,j;
  i = loc[0];
  j = loc[1];

  //now call the funtion 8 times for each possible location
  Serial.println("Possible moves are: ");
  king_fun(i+1,j,board);
  king_fun(i+1,j+1,board);
  king_fun(i+1,j-1,board);
  king_fun(i-1,j,board);
  king_fun(i-1,j+1,board);
  king_fun(i-1,j-1,board);
  king_fun(i,j+1,board);
  king_fun(i,j-1,board); 
  Serial.println("Make your move!");
}


void queen(int loc[2], int board[8][8]){
  //queen is just a combined movement of rook and bishop!
  rook(loc, board);
  bishop(loc, board);  
  }


void setup() {
  // put your setup code here, to run once:
  //Initiallizing the select lines!
  for(int i=0; i<6; i++){
    pinMode(Pins[i], OUTPUT);
  }
  pinMode(MUX, INPUT);
  pinMode(MOVE, INPUT);
  //Starting serial communication
  Serial.begin(9600);
  Serial.println("Game has started! \n");

}

void loop() {
  // put your main code here, to run repeatedly:
  TURN = digitalRead(MOVE); //0-White, 1-Black
  if(TURN == LOW){
    Serial.println("\n White's turn : ");
  }
  else{ 
    Serial.println("\n Blacks's turn : ");
  }
  
  scan(temp1);
  printboard(temp1);
  Serial.println("Please choose the piece which you want to move: ");
  //piece detection and suggestion part
  scan(temp2);
  while(compare(temp1, temp2) == -1){
    scan(temp2);
    //Serial.println("");
  }
  int a = compare(temp1,temp2);
  Serial.println("You selected piece at: "); 
  Serial.print("[ "); Serial.print(a/8); Serial.print(", "); Serial.print(a%8); Serial.println("] "); 
  pieceloc[0] = a/8;
  pieceloc[1] = a%8;
  identifyn(pieceloc[0], pieceloc[1]); //see which piece it is and show options to move

  //move making and kill detecting part
  scan(temp1);  ///check to see A) where piece is placed, B) Which piece has been killed
  while(compare(temp1, temp2) == -1){
    scan(temp1);
  }
  int b = compare(temp1,temp2);
  Serial.println("\n You placed piece at: "); 
  Serial.print("[ "); Serial.print(b/8); Serial.print(", "); Serial.print(b%8); Serial.println("] \n"); 
  //check for kill now, if kill isnt ther player will change turn so the below loop will exit
  scan(temp2);
  while(compare(temp1, temp2) == -1 and TURN ==digitalRead(MOVE)){
    scan(temp2);
  }
  if(compare(temp1, temp2) == -1){
    //normal move
    //printmem();
    Serial.println("No kill was made!");
    move_kill(a/8, a%8, b/8, b%8, 0);  
    //printmem();
  }
  else{
    //kill
    Serial.println("Kill was made!");
    //printmem();
    move_kill(a/8, a%8, b/8, b%8, 1);
    //printmem();
  }
  while(TURN == digitalRead(MOVE)){
    delay(5);  
  }
  
}



