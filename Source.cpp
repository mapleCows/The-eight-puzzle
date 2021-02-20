#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
#include <list>
using namespace std;

class Child {
    public:
        vector<char> table;
        int h;
        int g;
        Child();
        Child(vector<char>, int, int);
        int GetGH();
        int GetG();

};

void PrintVector(vector<string>);                       //Helper fucntion when testing to print vector
void PrintTable(Child);                                 //Prints tile board for game
vector<string> FindValidMoves(Child);                   //Calculates all valid moves for the blank space
vector<char> CreateTable(string, int);                  //Create the starting parent table
int FindBlank(Child);                                   //Finds blank in table
void MoveBlank(Child&, string, int);                    //Moves(swaps) the blank in the table
vector<char> CreateGoal(int);                           //Creates the goal board to be compared to
bool IsGoal(Child);                                     //compare goal with child board
vector<Child> CreateChildren(vector<string>, Child);    //Creates children board based of valid moves
void CalcG(Child&);                                     //Calulates the G
void CalcMisplacedTile(Child&);                         //Calc H for misplaced
void CalcManhattanDistance(Child&);                     //Calc H for manhattan                                     
int FindGoalLocation(char);                             //Finds the index of the specific tile in goal board
int FindCurrLocation(Child, char);                      //Find index of specific tile in child board
void PutIntoList(vector<Child>);                        //Puts children in correct order in list
void UniformCostSearch(Child);                          //Uniform cost search
void AStarMisplacedSearch(Child);                       //A* Misplace
void AStarManhattanSearch(Child);                       //A* Manhattan
void EndGame(Child);                                    //End print screen
void ChangeZeroToB(string&);                            //Changing user input to replace the 0 with b for the blank
void RemoveSpacesAndTabs(string&);                      //Removes spaces and tabs from user input

//GLOBALS
int root;                                               //Square root of the board size
Child goal;                                             //Goal board
list<Child> L;                                          //List of children
int numOfNodes = 0;                                     //Number of nodes explored
int maxNumOfQ = 0;                                      //Max size of the list



int main() 
{
    string input;
    string usergame = "";
    string defaultGame = "1235b6478";
    string searchChoice;
    
    cout << "Welcome to Luis Sanchez's 8-puzzle solver.\nType 1 to use a default puzzle, or 2 to enter your own puzzle\n";
    cin >> input;
    if (input == "2") {
        cout << "Enter your puzzle, use a 0 to represent the blank\nEnter the first row, use space or tabs between numbers ";
        cin.ignore();
        getline(cin, input);
        usergame += input;
        cout << "Enter the second row, use space or tabs between numbers ";
        
        getline(cin, input);
        usergame += input;
        cout << "Enter the third row, use space or tabs between numbers ";
        
        getline(cin, input);
        usergame += input;
        RemoveSpacesAndTabs(usergame);  //remove spaces and tabs from user input
        ChangeZeroToB(usergame);        //change 0 to b
    }
    else { usergame = defaultGame; }
    

    //Game set up
    int size = usergame.length();  //size to check if perfect square root
    root = sqrt(size + 1);  //cacl root
    vector<char> pTable = CreateTable(usergame, size); //create Parent table from user input
    vector<char> gTable = CreateGoal(size);         //create Goal Table based off size
    Child parent = Child(pTable, 0, 0);             //Create Parent object CALC mixmatch on parents
    goal.table = gTable;                            //Goal object


    cout << "Enter your choice of algorithm\n1. Uniform Cost Search\n2. A* with the Misplaced Tile heuristic.\n";
    cout << "3. A* with the Manhattan distance heuristic.\n";
    cin >> searchChoice;

    if (searchChoice == "1") { UniformCostSearch(parent); }
    if (searchChoice == "2") { AStarMisplacedSearch(parent); }
    if (searchChoice == "3") { AStarManhattanSearch(parent); }
    
    return 0;
}


void UniformCostSearch(Child parent)
{
    //General search
    /*
        1. check if goal
        2. find all valid moves
        3. calculate the order to push into Q (Later with A*)
        4. push into Q
        5. repeat
    */

    parent.h = 0;
    L.push_back(parent);
    vector<Child> children;
    cout << "Expanding State:";
    PrintTable(L.front());
    while (!L.empty())
    {

        if (IsGoal(L.front())) { EndGame(L.front()); break; }   //Checking if goal state


        vector<string> validMoves = FindValidMoves(L.front());  //Find all valid moves
        numOfNodes += validMoves.size();                        //Tally Fronteir nodes

        children = CreateChildren(validMoves, L.front());       //Creates children;
        for (int i = 0; i < children.size(); ++i) {             //Calculate the G for each child
            CalcG(children.at(i));
        }

        L.pop_front();                                          //pop parent of list
        PutIntoList(children);                                  //Put new Children into list


        cout << "The best state to expand with a g(n) = " << L.front().g;
        PrintTable(L.front());


        if (L.size() > maxNumOfQ) { maxNumOfQ = L.size(); }

    }
}

void AStarMisplacedSearch(Child parent)
{
    //General search
    /*
        1. check if goal
        2. find all valid moves
        3. calculate the order to push into Q (Later with A*)
        4. push into Q
        5. repeat
    */


    L.push_back(parent);
    vector<Child> children;
    cout << "Expanding this node:";
    PrintTable(L.front());
    while (!L.empty())
    {

        if (IsGoal(L.front())) { EndGame(L.front()); break; }   //Checking if goal state


        vector<string> validMoves = FindValidMoves(L.front());  //Find all valid moves
        numOfNodes += validMoves.size();                        //Tally Fronteir nodes

        children = CreateChildren(validMoves, L.front());       //Creates children;

        for (int i = 0; i < children.size(); ++i) {             //Calculate the G and H for each child
            CalcMisplacedTile(children.at(i));
            CalcG(children.at(i));
        }

        L.pop_front();                                          //pop parent off list
        PutIntoList(children);                                  //Put new Children into list


        cout << "The best state to expand with a g(n) = " << L.front().g << " and h(n) = " << L.front().h << " is...";
        PrintTable(L.front());


        if (L.size() > maxNumOfQ) { maxNumOfQ = L.size(); }     //keeping track of max size of nodes in list

    }
}

void AStarManhattanSearch(Child parent)
{
    //General search
   /*
       1. check if goal
       2. find all valid moves
       3. calculate the order to push into Q (Later with A*)
       4. push into Q
       5. repeat
   */


    L.push_back(parent);
    vector<Child> children;
    CalcManhattanDistance(parent);
    cout << "Expanding this node:";
    PrintTable(L.front());
    while (!L.empty())
    {

        if (IsGoal(L.front())) { EndGame(L.front()); break; }   //Checking if goal state


        vector<string> validMoves = FindValidMoves(L.front());  //Find all valid moves
        numOfNodes += validMoves.size();                        //Tally Fronteir nodes

        children = CreateChildren(validMoves, L.front());       //Creates children;

        for (int i = 0; i < children.size(); ++i) {             //Calculate the G and H for each child
            CalcManhattanDistance(children.at(i));
            CalcG(children.at(i));
        }

        L.pop_front();                                          //pop parent off list
        PutIntoList(children);                                  //Put new Children into list


        cout << "The best state to expand with a g(n) = " << L.front().g << " and h(n) = " << L.front().h << " is...";
        PrintTable(L.front());


        if (L.size() > maxNumOfQ) { maxNumOfQ = L.size(); }     //keeping track of max size of nodes in list

    }
}

void PrintVector(vector<string> v)
{
    for (unsigned int i = 0; i < v.size(); ++i) {
        cout << v.at(i) <<" ";
    }
}

void PrintTable(Child c)
{ 
    vector<char> v = c.table;
    cout << "\n|";
    int k = 0;
    for (unsigned int i = 0; i < v.size(); ++i) {
        cout << v.at(i) << " |";
        ++k;
        if (k%3 == 0 && k != v.size()) { cout << "\n|"; }
    }
    cout << endl;
}

vector<string> FindValidMoves(Child c)
{
    vector<string> validMoves;
    int bLocation = FindBlank(c);       //Blank index in child
    int x = bLocation % root;           //Calculating x cord
    int y = bLocation / root;           //y cord
    
    if (y != 0) { validMoves.push_back("UP"); } //up
    
    if (y != root - 1) { validMoves.push_back("DOWN"); } //down
    
    if (x != 0) { validMoves.push_back("LEFT"); } //left
    
    if (x != root - 1) { validMoves.push_back("RIGHT"); } //right
    
    return validMoves;
}

vector<char> CreateTable(string s, int size)
{
    vector<char> table;

    for (unsigned int i = 0; i < size; ++i) {
        table.push_back(s.at(i));   //pushing user input into char vector
    }
    
    return table;
}

int FindBlank(Child c) 
{
    vector<char> table = c.table;
    int index = -1;
    for (unsigned int i = 0; i < table.size(); ++i) {
        if (table.at(i) == 'b') { index = i; }        
    }
    return index;
}

void MoveBlank(Child &c, string move, int bLocation)
{
    vector<char> table = c.table;
    char temp;
    //up -root 
    if (move == "UP") {
        temp = table.at(bLocation);
        table.at(bLocation) = table.at(bLocation - root);
        table.at(bLocation - root) = temp;
    }
    //down -root 
    if (move == "DOWN") {
        temp = table.at(bLocation);
        table.at(bLocation) = table.at(bLocation + root);
        table.at(bLocation + root) = temp;
    }
    //right -root 
    if (move == "RIGHT") {
        temp = table.at(bLocation);
        table.at(bLocation) = table.at(bLocation +1);
        table.at(bLocation +1) = temp;
    }
    //left -root 
    if (move == "LEFT") {
        temp = table.at(bLocation);
        table.at(bLocation) = table.at(bLocation - 1);
        table.at(bLocation - 1) = temp;
    }
    c.table = table;
}

vector<char> CreateGoal(int size)
{
    vector<char> table;
    for (int i = 1; i < size; ++i) {
        string s = to_string(i);        //appending 12345678 or any size of the board given
        table.push_back(s[0]);
    }
    table.push_back('b');               //Adding a blank at the end 
    return table;
}

bool IsGoal(Child c)
{
    if (c.table == goal.table) { return true; }
    
    return false;
}

vector<Child> CreateChildren(vector<string> validMoves, Child p)
{
    int bLocation = 0;
    Child c;
    vector<Child> children;

    for (int i = 0; i < validMoves.size(); ++i) {
        c = p;                                          //copying prarent -> child
        bLocation = FindBlank(c);                       //finding blank
        MoveBlank(c, validMoves.at(i), bLocation);      //moving tile
        //cout << "\nThisis child:" << validMoves.at(i) << endl;
        //PrintTable(c);
        //CalcG(c);                                       //Calc G for child
        //CalcH(c);                                       //Calc H for child
        children.push_back(c);                          //putting into q to be added to list
    }
    
    return children;
}

void CalcG(Child &c)
{
    ++c.g;
}

void CalcMisplacedTile(Child& c)
{
    int tally = 0;
    for (int i = 0; i < c.table.size(); ++i) {
        
        if (c.table.at(i) == 'b') {             //skipping blank
            continue;
        }
        
        if (c.table.at(i) != goal.table.at(i)) {    //compare if misplaced or not
            ++tally;
        }
    }
    c.h = tally;
}

void CalcManhattanDistance(Child& c)
{
    int hy = 0 , hx = 0, h = 0;
    int currTile = 0;
    int goalTile = 0;
    int cx = 0, cy = 0, gx = 0, gy = 0;
    c.h = 0;                                             //Reseting H from parent

    for (int i = 0; i < c.table.size(); ++i) {
        if (c.table.at(i) == 'b') {
            continue;
        }
        currTile = FindCurrLocation(c, c.table.at(i));  //getting index of curr location 
        goalTile = FindGoalLocation(c.table.at(i));     //getting index of destination

        cx = currTile % root;                           //calculating cords
        cy = currTile / root;

        gx = goalTile % root;
        gy = goalTile / root;

        c.h += abs(cx - gx) + abs(cy - gy);            //Calculating distance/num of moves needed
        
    }
                
}

int FindGoalLocation(char c) {
    for (int i = 0; i < goal.table.size(); ++i) {
        if (c == goal.table.at(i)) {
            return i;
        }
    }
}

int FindCurrLocation(Child c, char tile) {
    for (int i = 0; i < c.table.size(); ++i) {
        if (tile == c.table.at(i)) {
            return i;
        }
    }
}

void PutIntoList(vector<Child> children)
{
    Child currC;        //current child
    Child it;           // List Child
    
    if (L.empty()) {                    //in case the list is empty when adding in the new children
        L.push_back(children.at(0));
        children.erase(children.begin());
    }


    for(int k = 0; k < children.size(); ++k) {
        currC = children.at(k);
        
        for (list<Child>::iterator i = L.begin(); i != L.end(); i++) {
            it = *i;
            
            if (currC.GetGH() <= it.GetGH()) {      //if curr GH is less than List child insert into list
                L.insert(i, currC);
                break;
            }
            if (currC.GetGH() >= L.back().GetGH()) {    //If curr is greather than the end of the list
                L.insert(L.end(), currC);
                break;
            }
        }
    }
    
}

void EndGame(Child c)
{
    //PrintTable(q.front());
    cout << "\n\nGoal!!\n\nTo solve this problem the search algorithm expanded a total of "<<numOfNodes <<" nodes.\n";
    cout << "The maximum number of nodes in the queue at any one time was " << maxNumOfQ << ".\n";
    cout << "The depth of the goal node was " << L.front().g << endl;

}

void ChangeZeroToB(string& s )
{
    for (int i = 0; i < s.size(); ++i) {
        if (s.at(i) == '0') { s.at(i) = 'b'; }
    }
}

void RemoveSpacesAndTabs(string& s)
{

    for (string::iterator i = s.begin(); i != s.end(); ++i) {
        if (*i == ' ' || *i == '\t') {
            s.erase(i);
            --i;
        }
    }
}

Child::Child()
{
}

Child::Child(vector<char> table, int g, int h)
{
    this->table = table;
    this->h = h;
    this->g = g;
}

int Child::GetGH()
{
    return (this->g + this->h);
}

int Child::GetG()
{
    return (this->g);
}


