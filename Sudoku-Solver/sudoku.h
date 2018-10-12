//======================
//    Sudoku Solver
//======================
//
//Started On - October 2nd, 2018
//Author - Arjun Aravind
//Objective - This is an application written in C++ which takes in Sudoku puzzles and outputs the solution.

#include<iostream>
using namespace std;

#define TRUE 1
#define FALSE 0

typedef int boolean;

/**	Sudoku-Frame (class)
	Contains functions and data members necessary fo holding the values in the sudoku-puzzle.
*/
class SudokuFrame{
	int sudokuFrame[9][9]; //This pointer will hold all the values in the matrix.
	int editableFrame[9][9]; //This pointer will tell us all the values which are editable
	int rowIter, colIter; //iterator variables
	
	/**	Sudoku-Frame() (Constructor)
		This constructor calls the initFrame() and getFrameValues() functions.
		@param none
		@return none
	*/
	public:SudokuFrame(){
		getFrameValues();
	}
	
	/**	getFrameValues()
		Gets the values to be inputted into the Sudoku-Frame.
		@param none
		@return none
	*/
	void getFrameValues(){
		cout<<"Enter the values for the Sudoku puzzle here!\n";
		cout<<"Enter the particular value when prompted. Enter 0 if cell is empty.\n\n";

		for(rowIter=0; rowIter<9; rowIter++){
			for(colIter=0; colIter<9; colIter++){
				cout<<"Enter value for cell["<<rowIter+1<<"]["<<colIter+1<<"] --> ";
				cin>>sudokuFrame[rowIter][colIter];

				if(sudokuFrame[rowIter][colIter]==0) editableFrame[rowIter][colIter]=0;
				else editableFrame[rowIter][colIter]=1;
			}
			cout<<"-------\n";
		}
	}
	
	/**	setCellValue()
		Takes in the row and col number and then assigns the passed 'num' value
		to that particular cell.
		@param row (int) row of the specified cell
		@param col (int) col of the specified cell
		@return none
	*/
	public:void setCellValue(int row, int col, int num){
		if(editableFrame[row][col]==0) sudokuFrame[row][col]=num;
	}
	
	/**	getCellValue()
		Returns the value of the cell at the specified row and col.
		@param row (int) row of the specified cell
		@param col (int) col of the specified cell
		@return (int) cellValue value at the specified cell
	*/
	public:int getCellValue(int row, int col){
		int cellValue=sudokuFrame[row][col];
		return cellValue;
	}
	
	/**	isEditable()
		Returns TRUE/FALSE depending on editableFrame values;
		@param row (int) row of the required cell
		@param col (int) col of the required cell
		@return (boolean) whether cell is editable or not
	*/
	public:boolean isEditable(int row, int col){
		return (editableFrame[row][col]-1)*(-1);
	}

	public:void clearFrameFrom(int row, int col){
		int jcount=0;

		for(rowIter=row; rowIter<9; rowIter++){
			
			if(jcount==0) colIter=col;
			else colIter=0;

			for(; colIter<9; colIter++){
				if(editableFrame[rowIter][colIter]==0) sudokuFrame[rowIter][colIter]=0;
			}

			jcount++;

		}
	}

	/**	displayFrame()
		Displays the values stored in the SudokuFrame object.
		@param none
		@return none
	*/
	public:void displayFrame(){
		cout<<"\n";
		for(rowIter=0; rowIter<9; rowIter++){
			cout<<"+---+---+---+---+---+---+---+---+---+\n";
			for(colIter=0; colIter<9; colIter++){
				cout<<"| ";
				if(sudokuFrame[rowIter][colIter]==0) cout<<"  ";
				else cout<<sudokuFrame[rowIter][colIter]<<" ";
			}
			cout<<"|\n";
		}
		cout<<"+---+---+---+---+---+---+---+---+---+\n";
	}
	
};


class Possibilities{
	
	struct node{
		int value;
		struct node* next;
	};
	
	typedef struct node* Node;

	Node head;
	Node pos;

	public:Possibilities(){
		head=new struct node;
		head->next=NULL;
	}

	public:~Possibilities(){
		destroy();
	}

	public:void push(int number){
		Node temp=new struct node;

		temp->value=number;
		temp->next=NULL;

		pos=head;
		while(pos!=NULL){
			if(pos->next==NULL){
				pos->next=temp;
				break;
			}
			pos=pos->next;
		}
	}

	public:int operator[](int index){
		int count=0;
		pos=head->next;
		
		while(pos!=NULL){
			if(count==index)
				return pos->value;
			pos=pos->next;
			count++;
		}

		return -1;
	}

	public:void print(){
		pos=head->next;
		while(pos!=NULL){
			cout<<pos->value<<",";
			pos=pos->next;
		}
		cout<<"\b";
	}

	int length(){
		pos=head->next;
		int count=0;

		while(pos!=NULL){
			count++;
			pos=pos->next;
		}
		
		return count;
	}

	void copy(Possibilities possibilities){ //Need to make this clear the old list if exists
		int oldLength=possibilities.length();
		int iter=0;
		
		pos=head;
		for(iter=0; iter<oldLength; iter++){
			Node temp=new struct node;

			temp->next=NULL;
			temp->value=possibilities[iter];

			pos->next=temp;
			pos=pos->next;
		}
	}

	void destroy(){
		Node temp;
		pos=head;
		while(pos!=NULL){
			temp=pos;
			pos=pos->next;
			delete temp;
		}
	}

};


/**	SudokuSolver (class)
	Takes in the SudokuFrame object and solves the Sudoku Puzzle.
*/
class SudokuSolver{
	
	int rowIter, colIter; //Iter variables
	int recursiveCount; //Stats variable
	SudokuFrame frame;

	public:SudokuSolver(){
		recursiveCount=0;
	}
	
	/**	cellValueValid()
		Checks if the value is valid or not.
		@param row (int) row of the required value
		@param col (int) col of the required value
		@param currentValue (int) the required value
		@return (boolean) whether the value is valid or not in the sudoku frame
	*/
	public:boolean cellValueValid(int row, int col, int currentValue){

		//Checking if value exists in same column
		for(rowIter=0; rowIter<9; rowIter++){
			if(rowIter!=row){
				int comparingValue=frame.getCellValue(rowIter,col);
				if(comparingValue==currentValue) return FALSE;
			}
		}

		//Checking if value exists in same row
		for(colIter=0; colIter<9; colIter++){
			if(colIter!=col){
				int comparingValue=frame.getCellValue(row,colIter);
				if(comparingValue==currentValue) return FALSE;
			}
		}

		//Checking if value exists in the same 3x3 square block
		if(ThreeByThreeGridValid(row,col,currentValue)==FALSE) return FALSE;

		return TRUE;
	}
	
	/**	ThreeByThreeGridValid()
		Checks if the same value is also present in the same 3x3 grid block.
		@param row (int) row of the required cell
		@param col (int) col of the required cell
		@param currentValue (int) required value
		@return (boolean) whether the value is present or not
	*/
	boolean ThreeByThreeGridValid(int row, int col, int currentValue){
		int rowStart=(row/3)*3;
		int rowEnd=(rowStart+2);

		int colStart=(col/3)*3;
		int colEnd=(colStart+2);

		for(rowIter=rowStart; rowIter<=rowEnd; rowIter++){
			for(colIter=colStart; colIter<=colEnd; colIter++){
				if(frame.getCellValue(rowIter,colIter)==currentValue) return FALSE;
			}
		}

		return TRUE;	
	}

	public:Possibilities getCellPossibilities(int row, int col){
		int iter=0;

		Possibilities possibilities;

		for(iter=1; iter<=9; iter++){
			if(cellValueValid(row,col,iter))
				possibilities.push(iter);
		}

		return possibilities;
	}

	int singleCellSolve(int row, int col){
		
		statsIncrement();

		if(frame.isEditable(row,col)){

			Possibilities possibilities;
			possibilities.copy(getCellPossibilities(row,col));

			int posLength=possibilities.length();
			int posIter=0, newRow=row, newCol=col;

			for(posIter=0; posIter<posLength; posIter++){
				int possibility=possibilities[posIter];
				frame.setCellValue(row,col,possibility);

				if(col<8) newCol=col+1;
				else if(col==8){
					if(row==8) return 1;
					newRow=row+1;
					newCol=0;
				}

				{

					if(singleCellSolve(newRow,newCol)==0){
						frame.clearFrameFrom(newRow,newCol);
					}
					else return 1;

				} //Recursion block ends here

			} //Loop ends here

			return 0;
			
		} //The isEditable() if block ends here.
		else{

			int newRow=row, newCol=col;

			if(col<8) newCol=col+1;
			else if(col==8){
				if(row==8) return 1;
				newRow=row+1;
				newCol=0;
			}

			return singleCellSolve(newRow,newCol);

		} //The else block ends here

	}

	public:void solve(){
		int success=singleCellSolve(0,0);
		if(success==1) cout<<"\nYour Sudoku puzzle has been solved! QED.\n";
		else cout<<"This didn't work!\n";
	}
	
	/**	displayFrame()
		Displays the sudoku frame by calling the displayFrame() func of the
		SudokuFrame object.
		@param none
		@return none
	*/
	public:void displayFrame(){
		frame.displayFrame();
	}

	void statsIncrement(){
		recursiveCount++;
	}

	public:void statsPrint(){
		cout<<"\nThe singleCellSolve() function was recursively called "<<recursiveCount<<" times.\n";
	}
	
};
