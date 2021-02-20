#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <queue>
using namespace std;


void PrintVector(vector<string>);
void PrintTable(vector<char>);
vector<string> FindValidMoves(vector<char>);
vector<char> CreateTable(string, int);
int FindBlank(vector<char>);
vector<char> MoveBlank(vector<char>, string, int);
vector<char> CreateGoal(int);
void CreateChildren(vector<string>);
void EndGame(vector<char>, int);

//GLOBALS
int root;
vector<char> goal;
queue<vector<char>> q;
int numOfNodes = 0;
int maxNumOfQ = 0;
int depth = 0;


int main() 
{
    
    string input = "1234567b8"; //input for table
    int size = input.length();  //size to check if perfect square root
    root = sqrt(size + 1);  //cacl root
    vector<char> pTable = CreateTable(input, size); //create Parent table from user input
    goal = CreateGoal(size);                        //create Goal Table
    PrintTable(pTable);
    PrintTable(goal);
    q.push(pTable);
    
    //General search
    /*  
        1. check if goal
        2. find all valid moves
        3. calculate the order to push into Q (Later with A*)
        4. push into Q
        5. repeat
    */

    while (!q.empty()) 
    {
        //Checking if goal state
        if (q.front() == goal) { EndGame(q.front(), numOfNodes); break; }
        
        //Find all valid moves (Create Children)
        vector<string> validMoves = FindValidMoves(q.front());
        numOfNodes += validMoves.size();
        PrintVector(validMoves);
        CreateChildren(validMoves);
        
        //pop parent
        q.pop();

        
        ++depth;
        if (q.size() > maxNumOfQ) { maxNumOfQ = q.size(); }
    }

    return 0;
}

void PrintVector(vector<string> v)
{
    for (unsigned int i = 0; i < v.size(); ++i) {
        cout << v.at(i) <<" ";
    }
}

void PrintTable(vector<char> v)
{ 
    cout << "\n|";
    int k = 0;
    for (unsigned int i = 0; i < v.size(); ++i) {
        cout << v.at(i) << " |";
        ++k;
        if (k%3 == 0 && k != v.size()) { cout << "\n|"; }
    }
    cout << endl;
}

vector<string> FindValidMoves(vector<char> table)
{
    vector<string> validMoves;
    int bLocation = FindBlank(table);
    int x = bLocation % root;
    int y = bLocation / root;
    //up
    if (y != 0) { validMoves.push_back("UP"); }
    //down
    if (y != root - 1) { validMoves.push_back("DOWN"); }
    //left
    if (x != 0) { validMoves.push_back("LEFT"); }
    //right
    if (x != root - 1) { validMoves.push_back("RIGHT"); }
    
    return validMoves;
}

vector<char> CreateTable(string s, int size)
{
    vector<char> table;

    for (unsigned int i = 0; i < size; ++i) {
        table.push_back(s.at(i));
    }
    for (unsigned int i = 0; i < table.size(); ++i) {
        cout << table.at(i);
    }
    return table;
}

int FindBlank(vector<char> table) 
{
    int index = -1;
    for (unsigned int i = 0; i < table.size(); ++i) {
        if (table.at(i) == 'b') { index = i; }
    }
    return index;
}

vector<char> MoveBlank(vector<char> table, string move, int bLocation)
{
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
    return table;
}

vector<char> CreateGoal(int size)
{
    vector<char> table;
    for (int i = 1; i < size; ++i) {
        string s = to_string(i);
        table.push_back(s[0]);
    }
    table.push_back('b');
    return table;
}

void CreateChildren(vector<string> validMoves)
{
    int bLocation = 0;
    vector<char> child;
    
    for (int i = 0; i < validMoves.size(); ++i) {
        child = q.front();
        bLocation = FindBlank(child);
        child = MoveBlank(child, validMoves.at(i), bLocation);
        //cout << "\nThisis child:" << validMoves.at(i) << endl;
        //PrintTable(child);
        q.push(child);
    }
}

void EndGame(vector<char>, int)
{
    PrintTable(q.front());
    cout << "To solve this problem the search algorithm expanded a total of "<<numOfNodes <<" nodes.\n";
    cout << "The maximum number of nodes in the queue at any one time was " << maxNumOfQ << ".\n";
    cout << "The depth of the goal node was " << depth << endl;

}
