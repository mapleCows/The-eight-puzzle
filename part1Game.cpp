#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;


void PrintVector(vector<string>);
void PrintTable(vector<char>);
vector<string> FindValidMoves(int);
vector<char> CreateTable(string, int);
int FindBlank(vector<char>);
vector<char> MoveBlank(vector<char>, string, int);
vector<char> CreateGoal(int);
int root;

int main() {
    string input = "0123b5678"; //input for table
    int size = input.length();  //size to check if perfect square root
    
    vector<char> table = CreateTable(input, size); //create the table from user input
    vector<char> goal = CreateGoal(size);
    PrintTable(goal);
    

    root = sqrt(size + 1);  //cacl root
    int bLocation = FindBlank(table); //find blank cords
    vector<string> validMoves = FindValidMoves(bLocation); //find valid moves for blank
    PrintTable(table); //print table
    vector<char> childTable = MoveBlank(table, "DOWN", bLocation);
    PrintTable(childTable);
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

vector<string> FindValidMoves(int bLocation)
{
    vector<string> validMoves;
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
