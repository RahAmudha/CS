/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{6}
* Arithmetic.cpp
* Arithmetic.cpp runs various combinations of addition subtraction and multiplication with the BigInteger ADT
***/ 

#include <cstdio>
#include<iostream>
#include<fstream>
#include<string>
#include"BigInteger.h"

using namespace std;

int main(int argc, char * argv[]){

    ifstream in;
    ofstream out;

    string sA = "";
    string sB = "";
    string junk = "";

    // check command line for correct number of arguments
    if( argc != 3 ){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if( !in.is_open() ){
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    if (!getline(in, sA)) exit (1);
    if (!getline(in, junk)) exit (1);
    if (!getline(in, sB)) exit (1);

    BigInteger A = BigInteger(sA);
    BigInteger B = BigInteger(sB);

    out << A << endl << endl;
    out << B << endl << endl;

    out << A+B << endl << endl;
    out << A-B << endl << endl;
    out << A-A << endl << endl;
    out << 3*A-2*B << endl << endl;
    out << A*B << endl << endl;
    out << A*A << endl << endl;
    out << B*B << endl << endl;
    out << 9*A*A*A*A + 16*B*B*B*B*B << endl << endl;

    in.close();
    out.close();

    return(EXIT_SUCCESS);
}
