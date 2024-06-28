/***
   Team Project - Classical CD Database
   By Aman Grewal, Bishwas Sherchan, Lance Dela Cruz, Dongjun Lee
   6/18/21
***/

#include "BinarySearchTree.h"
#include "HashTable.h"
#include "Stack.h"
#include "CD.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
#include <cstring>


using namespace std;

// Function prototypes
void buildBST(const string &filename, BinarySearchTree<CD> &);
void hDisplay(CD &);
void titleSearchManager(const BinarySearchTree<CD> &);
void displayManager(const BinarySearchTree<CD> &);
int key_to_index(const CD &key, int size);
void barcodeSearchManager(HashTable<CD>& list);
void buildHash(const string &filename, HashTable<CD> &);
void removeManager(HashTable<CD>&, BinarySearchTree<CD>&, Stack<CD>&);
void undoDelete(Stack<CD> &, HashTable<CD> &, BinarySearchTree<CD>&);
void insert(BinarySearchTree<CD> &bst,HashTable<CD> &hash);
void iDisplay(CD &, int);



int main()
{
    string filename = "Classical_CD_Repository3.txt";
    string outFileName = "HashReport.txt";
    
    // Create binary search tree, hashtable, and stack data structures
    BinarySearchTree<CD> bst;
    HashTable<CD> hash;
    Stack<CD> stack;
    
    
    // Build data structures
    buildBST(filename, bst);
    buildHash(filename,hash);

    // Menu options (9 is deliberately skipped as it is a hidden option for indented list)
    cout << "MENU OPTIONS" << endl;
    cout << "============" << endl << endl;
    cout << "1: Display Manager" << endl;
    cout << "2: Remove Manager" << endl;
    cout << "3: Search Manager(Barcode)" << endl;
    cout << "4: Search Manager(Title)" << endl;
    cout << "5: Undo" << endl;
    cout << "6: Write Data to the File" << endl;
    cout << "7: Stats" << endl;
    cout << "8: Insert" << endl;
    cout << "10: Help" << endl;
    
    cout << endl;
    
    
    // Enter option
    int option;
    cout << "Enter your options ['O' to quit/'10' for help (Back to menu)]: ";
    cin >> option;
    cout << endl;
    
    // Cases for option chosen
    while (option != 0) {
    if (option == 1) {
    displayManager(bst);   // Display inorder
    } else if (option == 2){
        cin.ignore();
        removeManager(hash,bst,stack);  // Remove barcode
    }else if (option == 3){
        cin.ignore();
        barcodeSearchManager(hash);  // Search barcode
    }else if (option == 4){
        cin.ignore();
        titleSearchManager(bst);  // Search title
    }else if (option == 5){
        undoDelete(stack, hash, bst);  // Undo removal
    }else if (option == 6){
        hash.writeToFile(outFileName);  // Write to output file
        stack.~Stack<CD>();
    }else if (option == 7){
        hash.statistics();   // Show statistics
    }else if (option == 8){
        insert(bst, hash);   // Insert new item
    }else if (option == 9){
        bst.printTree(iDisplay);   // Hidden menu option - indented display
    }else if (option == 10){
        cout << "MENU OPTIONS" << endl;
        cout << "============" << endl << endl;
        cout << "1: Display Manager" << endl;
        cout << "2: Remove Manager" << endl;
        cout << "3: Search Manager(Barcode)" << endl;
        cout << "4: Search Manager(Title)" << endl;
        cout << "5: Undo" << endl;
        cout << "6: Write Data to the File" << endl;
        cout << "7: Stats" << endl;
        cout << "8: Insert" << endl;
        cout << "10: Help" << endl;
    }
        
        cout << endl;
        cout << "Enter your options ['O' to quit/'10' for help (Back to menu)]: ";
        cin >> option;
        cout << endl;
    }
    
    
}


/*
 This function reads data about CDs from a file and inserts them
 into a binary search tree. The list is sorted in ascending order by code
*/
void buildBST(const string &filename, BinarySearchTree<CD> &list)
{
    ifstream fin(filename);

    // Check file
    if(!fin)
    {
        cout << "Error opening the input file: \""<< filename << "\"" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    
    // Input data into data structure
    while (getline(fin, line))
    {
       string barcode, composer, title, conductor;
       int year;

       stringstream temp(line);   // create temp with data from line
       temp >> barcode;              // read from temp
       temp >> composer;
        temp>>std::ws;
       getline(temp, title, ';');// stop reading name at ';'
        temp>>std::ws;
       getline(temp, conductor, ';');  // stop reading name at ';'
       temp >> year;
       // create a College object and initialize it with data from file
       CD aCD(barcode, composer, title, conductor, year);
       list.insert(aCD);
    }
    
    // Close file
    fin.close();
}

/*
 Function to build hash table
*/
void buildHash(const string &filename, HashTable<CD> &list){
    ifstream fin(filename);

    if(!fin)
    {
        cout << "Error opening the input file: \""<< filename << "\"" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    
    // Input data into data structure
    while (getline(fin, line))
    {
       string barcode, composer, title, conductor;
       int year;

       stringstream temp(line);   // create temp with data from line
       temp >> barcode;              // read from temp
       temp >> composer;
        temp>>std::ws;
       getline(temp, title, ';');// stop reading name at ';'
        temp>>std::ws;
       getline(temp, conductor, ';');  // stop reading name at ';'
       temp >> year;
       // create a College object and initialize it with data from file
       CD aCD(barcode, composer, title, conductor, year);
        list.insert(aCD);
    }
    
    // Close input file
    fin.close();
    
    // Rehashes load factor is 0.5
    while (list.getLoadFactor() >= 0.75) {
        list.rehash();
    }
}

/*
 Function to display inorder functions
*/
void displayManager(const BinarySearchTree<CD> & list){

    string action;
    cout << "Display inorder by Title [Y/N]: ";
    cin >> action;
    char option = toupper(action[0]);
    cout << endl;
    if(option == 'Y')
    {   cout << left << setw(12) << "Barcode" << "   ";
        cout << left << setw(15) << "Composer" << "   ";
        cout << left << setw(24) << "Title" << "    ";
        cout << left << setw(23) << "Conductor"<< "    ";
        cout << right << setw(4) << "Year" << endl;

        cout << left << setw(12) << "=======" << "   ";
        cout << left << setw(15) << "========" << "   ";
        cout << left << setw(24) << "=====" << "    ";
        cout << left << setw(23) << "========="<< "    ";
        cout << right << setw(4) << "====" << endl << endl;
        
        list.inOrder(hDisplay);
    }
}


/*
 Function to search items by title
*/
void titleSearchManager(const BinarySearchTree<CD> &list){

    string title;
    CD a, item;
    string bar;


    cout << "Search By Title" << endl;
    cout << "===============" << endl << endl;

    cout << "Enter Title or Q to quit: ";
    getline(cin, title);
    cout << endl;

    while(toupper(title[0]) != 'Q')
    {
        a.setTitle(title);

        if(list.search(a,item))
        {
            cout << left << setw(12) << "Barcode" << "   ";
            cout << left << setw(15) << "Composer" << "   ";
            cout << left << setw(24) << "Title" << "    ";
            cout << left << setw(23) << "Conductor"<< "    ";
            cout << right << setw(4) << "Year" << endl;

            cout << left << setw(12) << "=======" << "   ";
            cout << left << setw(15) << "========" << "   ";
            cout << left << setw(24) << "=====" << "    ";
            cout << left << setw(23) << "========="<< "    ";
            cout << right << setw(4) << "====" << endl << endl;
            
            list.printAll(a,hDisplay);
        } else
        {
            cout << "Title \"" << title << "\" was not found in this list." << endl;
        }

        cout << endl;
        cout << "Enter Title or Q to quit: ";
        getline(cin, title);
    }
}

/*
 Function to search items by barcode
*/
void barcodeSearchManager(HashTable<CD>& list){

    cout << "Search by Barcode" << endl;
    cout << "=================" << endl << endl;

    string barcode;

    cout << "Enter barcode or Q to quit: ";
    getline(cin, barcode);
    cout << endl;

    while (toupper(barcode[0]) != 'Q' )
    {
        CD found, key(barcode, " ", " "," ",-1);

        if (list.search(found, key) != -1){
            
            cout << left << setw(12) << "Barcode" << "   ";
            cout << left << setw(15) << "Composer" << "   ";
            cout << left << setw(24) << "Title" << "    ";
            cout << left << setw(23) << "Conductor"<< "    ";
            cout << right << setw(4) << "Year" << endl;

            cout << left << setw(12) << "=======" << "   ";
            cout << left << setw(15) << "========" << "   ";
            cout << left << setw(24) << "=====" << "    ";
            cout << left << setw(23) << "========="<< "    ";
            cout << right << setw(4) << "====" << endl << endl;
            cout << found;
        }
        else
        {
            cout << endl << barcode << " not found!" << endl;
        }
        cout << endl;
        cout << "Enter barcode or Q to quit: ";
        getline(cin, barcode);
        cout << endl;
    }
}

/*
 Function to remove items
*/
void removeManager(HashTable<CD>& list, BinarySearchTree<CD>& bst, Stack<CD>& stack){
    string barcode;

    cout << "Remove CD" << endl;
    cout << "=========" << endl << endl;

    cout << "Enter barcode or Q to quit: ";
    getline(cin, barcode);
    cout << endl;

    while (toupper(barcode[0]) != 'Q' )
    {
        CD itemOut, key(barcode, " ", " "," ",-1);

        /*
         insert barcode (unique) to delete from the hash
         if found get the title of the barcode and delete the title with the
         samebarcode from the bst.
         push both to stack
         use undo function to reinsert in hash and bst.
         */

        if (list.remove(itemOut, key))
        {
            cout << "Item deleted" << endl;
            cout << itemOut << endl;
            stack.push(itemOut);
            bst.remove(itemOut);
        }
        else
        {
            cout << endl << barcode << " not found!" << endl;
        }
        cout << endl;
        cout << "Enter barcode or Q to quit: ";
        getline(cin, barcode);
    }
}
/*
 Function to undo delete
*/
void undoDelete(Stack<CD> & stack, HashTable<CD> & hash, BinarySearchTree<CD> & bst){

    cout << "Undo Delete" << endl;
    cout << "===========" << endl << endl;

    CD a;
    char option;
    cout << "Undo Delete [Y/N]: " << endl;
    cin >> option;
    if(stack.isEmpty()) {
        cout << "Error: Remove an Item First" << endl;
    } else if (option == 'y'|| 'Y'){
        while(!stack.isEmpty()){
            a = stack.pop();
            hash.insert(a);
            bst.insert(a);
        }
    }
}
/*
 Function to insert new item
*/
void insert(BinarySearchTree<CD> &bst,HashTable<CD> &hash)
{

    string barcode, conductor, title, composer;
    int year;
    char option;

    cout << "Insert Item" << endl;
    cout << "===========" << endl << endl;

    cout << "Insert Item[Y/N]: ";
    cin >> option;
    cout << endl;

    while (option == 'y' || option == 'Y')
    {
        cout << "Enter Barcode (12 digits): ";
        cin >> barcode;
        cout << "Enter Composer: ";
        cin >>composer;
        cout << "Enter title: ";
        cin.ignore();
        getline(cin,title);
        cout << "Enter conductor: ";
        getline(cin,conductor);
        cout << "Enter year: ";
        cin >> year;

        CD a(barcode,composer,title,conductor,year);

        if (hash.insert(a)){
            bst.insert(a);
            cout << endl <<"Item Added" << endl << endl;

            cout << left << setw(12) << "Barcode" << "   ";
            cout << left << setw(15) << "Composer" << "   ";
            cout << left << setw(24) << "Title" << "    ";
            cout << left << setw(23) << "Conductor"<< "    ";
            cout << right << setw(4) << "Year" << endl;

            cout << left << setw(12) << "=======" << "   ";
            cout << left << setw(15) << "========" << "   ";
            cout << left << setw(24) << "=====" << "    ";
            cout << left << setw(23) << "========="<< "    ";
            cout << right << setw(4) << "====" << endl << endl;

            cout << a << endl;

        }
        else
        {
        
        cout << "Duplicate barcode could not be added or barcode is longer than 12 digits" << endl;
        }
        cout << "Insert Item[Y/N]: ";
        cin >> option;
        cout << endl;
    }
}

/*
 horizontal display
*/
void hDisplay (CD &item)
{
    cout << left << setw(12) << item.getBarcode() << "   ";
    cout << left << setw(15) << item.getComposer() << "   ";
    cout << left << setw(24) << item.getTitle() << "    ";
    cout << left << setw(23) << item.getConductor()<< "    ";
    cout << right << setw(4) << item.getYear() << endl;
}


/*
 indented tree display: one item per line, including the level number
*/
void iDisplay(CD &item, int level)
{
    for (int i = 1; i < level; i++)
        cout << "..";
    cout << level << "). " << item << endl;
    
}





