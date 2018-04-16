# Chess_SLAM
Contains the code for Embedded Systems Term Project. 


In this project, I am tracking the complete chess board using a MUX(64X1), and keeping the record of all the pieces on board.Which is called Slamming. SLAM stands for simultaneous localization and mapping. I am tracking the pieces using on-off toggle button switches placed on each square of the chess board.
	Then, I compare the previous state and current state of switches and detect the position of the piece on chess board, and use different functions for different types of pieces on the chess board which compute all the legal moves for that particular piece and displays them in Serial Monitor.
	The location of each piece is stored separately in an array which helps us identify where all the piece currently are(mapping) and then call the functions. The algorithm can also handle kill of a piece.  A Turn switch is used to detect which player is being made

