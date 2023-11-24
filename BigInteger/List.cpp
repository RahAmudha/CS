/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{6}
* List.cpp
* List ADT Program
***/ 
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include<stdexcept>
#include <string>
#include"List.h"


// Private Constructor --------------------------------------------------------

    // Node constructor
    List::Node::Node(ListElement x) {
        data = x;
        next = nullptr;
        prev = nullptr;
    }

// Class Constructors & Destructors -------------------------------------------

    // Default Constructor
    List::List() {
        Node* f = new Node(-1);
        Node* b = new Node(-1);
        frontDummy = f;
        backDummy = b;
        frontDummy->next = backDummy;
        backDummy->prev = frontDummy;
        afterCursor = backDummy;
        beforeCursor = frontDummy;
        num_elements = 0;
        pos_cursor = 0;
    }

    // Copy Constructor
    List::List(const List& L) {
        // make this an empty List
        Node* f = new Node(-1);
        Node* b = new Node(-1);
        frontDummy = f;
        backDummy = b;
        frontDummy->next = backDummy;
        backDummy->prev = frontDummy;
        afterCursor = backDummy;
        beforeCursor = frontDummy;
        num_elements = 0;
        pos_cursor = 0;

        if (L.length()) {
            // load elements of L into this List
            Node* N = L.frontDummy->next;
            while (N->next != nullptr) {
                this->insertBefore(N->data);
                N = N->next;
            }
        }
    }

    // Destructor
    List::~List() {
        clear();
        delete (frontDummy);
        delete (backDummy);
    }


// Access functions -----------------------------------------------------------

    // length()
    // Returns the length of this List.
    int List::length() const{
        return (num_elements);
    }

    // front()
    // Returns the front element in this List.
    // pre: length()>0
    ListElement List::front() const{
        if (!length()) throw std::length_error("List: front(): empty List");
        return (frontDummy->next->data);
    }

    // back()
    // Returns the back element in this List.
    // pre: length()>0
    ListElement List::back() const{
        if (!length()) throw std::length_error("List: back(): empty List");
        return (backDummy->prev->data);
    }

    // position()
    // Returns the position of cursor in this List: 0 <= position() <= length().
    int List::position() const {
        return (pos_cursor);
    }

    // peekNext()
    // Returns the element after the cursor.
    // pre: position()<length()
    ListElement List::peekNext() const{
        if (position() >= length()) throw std::range_error("List: peekNext(): position(" + std::to_string(position()) + ") >= length(" + std::to_string(length()) + ")");
        return (afterCursor->data);
    }

    // peekPrev()
    // Returns the element before the cursor.
    // pre: position()>0
    ListElement List::peekPrev() const{
        if (position() <= 0) throw std::range_error("List: peekPrev(): position(" + std::to_string(position()) + ") >= length(" + std::to_string(length()) + ")");
        return (beforeCursor->data);
    }

// Manipulation procedures -------------------------------------------------

    // clear()
    // Deletes all elements in this List, setting it to the empty state.
    void List::clear() {
        if (!length()) return;
        moveFront();
        while (length()) eraseAfter();
    }

    // moveFront()
    // Moves cursor to position 0 in this List.
    void List::moveFront() {
        afterCursor = frontDummy->next;
        beforeCursor = frontDummy;
        pos_cursor = 0;
    }

    // moveBack()
    // Moves cursor to position length() in this List.
    void List::moveBack() {
        beforeCursor = backDummy->prev;
        afterCursor = backDummy;
        pos_cursor = length();
    }

    // moveNext()
    // Advances cursor to next higher position. Returns the List element that
    // was passed over. 
    // pre: position()<length() 
    ListElement List::moveNext() {
        if (position() >= length()) throw std::range_error("List: moveNext(): position(" + std::to_string(position()) + ") >= length(" + std::to_string(length()) + ")");
        beforeCursor = afterCursor;
        afterCursor = afterCursor->next;
        ++pos_cursor;
        return (beforeCursor->data);
    }

    // movePrev()
    // Advances cursor to next lower position. Returns the List element that
    // was passed over. 
    // pre: position()>0
    ListElement List::movePrev() {
        if (position() <= 0) throw std::range_error("List: movePrev(): position(" + std::to_string(position()) + ") <= 0");
        afterCursor = beforeCursor;
        beforeCursor = beforeCursor->prev;
        --pos_cursor;
        return (afterCursor->data);
    }

    // insertAfter()
    // Inserts x after cursor.
    void List::insertAfter(ListElement x) {
        Node* temp = new Node(x);
        temp->next = afterCursor;
        temp->prev = beforeCursor;
        afterCursor->prev = temp;
        beforeCursor->next = temp;
        afterCursor = temp;
        ++num_elements;
    }

    // insertBefore()
    // Inserts x before cursor.
    void List::insertBefore(ListElement x) {
        Node* temp = new Node(x);
        temp->next = afterCursor;
        temp->prev = beforeCursor;
        beforeCursor->next = temp;
        afterCursor->prev = temp;
        beforeCursor = temp;
        ++num_elements;
        ++pos_cursor;
    }

    // setAfter()
    // Overwrites the List element after the cursor with x.
    // pre: position()<length()
    void List::setAfter(ListElement x) {
        if (position() >= length()) throw std::range_error("List: setAfter(): position(" + std::to_string(position()) + ") >= length(" + std::to_string(length()) + ")");
        afterCursor->data = x;
    }

    // setBefore()
    // Overwrites the List element before the cursor with x.
    // pre: position()>0
    void List::setBefore(ListElement x) {
        if (position() <= 0) throw std::range_error("List: setBefore(): position(" + std::to_string(position()) + ") <= 0");
        beforeCursor->data = x;
    }

    // eraseAfter()
    // Deletes element after cursor.
    // pre: position()<length()
    void List::eraseAfter() {
        if (position() >= length()) throw std::range_error("List: eraseAfter(): position(" + std::to_string(position()) + ") >= length(" + std::to_string(length()) + ")");
        beforeCursor->next = afterCursor->next;
        afterCursor->next->prev = beforeCursor;
        delete (afterCursor);
        afterCursor = beforeCursor->next;
        --num_elements;
    }

    // eraseBefore()
    // Deletes element before cursor.
    // pre: position()>0
    void List::eraseBefore() {
        if (position() <= 0) throw std::range_error("List: eraseBefore(): position(" + std::to_string(position()) + ") <= 0");
        afterCursor->prev = beforeCursor->prev;
        beforeCursor->prev->next = afterCursor;
        delete (beforeCursor);
        beforeCursor = afterCursor->prev;
        --num_elements;
        --pos_cursor;
    }

// Other Functions ------------------------------------------------------------

    // findNext()
    // Starting from the current cursor position, performs a linear search (in 
    // the direction front-to-back) for the first occurrence of element x. If x
    // is found, places the cursor immediately after the found element, then 
    // returns the final cursor position. If x is not found, places the cursor 
    // at position length(), and returns -1. 
    int List::findNext(ListElement x) {
        moveNext();
        //std::cout << pos_cursor << "EEEEEEEE";
        while (pos_cursor < length()) {
            //std::cout << pos_cursor;
            if (beforeCursor->data == x) {
                return pos_cursor;
            } moveNext();
        } return (-1);
    }

    // findPrev()
    // Starting from the current cursor position, performs a linear search (in 
    // the direction back-to-front) for the first occurrence of element x. If x
    // is found, places the cursor immediately before the found element, then
    // returns the final cursor position. If x is not found, places the cursor 
    // at position 0, and returns -1. 
    int List::findPrev(ListElement x) {
        movePrev();
        while (pos_cursor > 0) {
            if (afterCursor->data == x) {
                return pos_cursor;
            } movePrev();
        } return (-1);
    }

    // cleanup()
    // Removes any repeated elements in this List, leaving only unique elements.
    // The order of the remaining elements is obtained by retaining the frontmost 
    // occurrance of each element, and removing all other occurances. The cursor 
    // is not moved with respect to the retained elements, i.e. it lies between 
    // the same two retained elements that it did before cleanup() was called.
    void List::cleanup() {
        if (length() <= 1) return;
        int truePos = position();
        List storedVals = List();
        bool erased = false;
        moveFront();
        while (pos_cursor != length()) {
            erased = false;
            moveNext();
            for (storedVals.moveFront(); !erased && storedVals.pos_cursor < storedVals.length(); storedVals.moveNext()) {
                if (peekPrev() == storedVals.peekNext()) {
                    eraseBefore();
                    erased = true;
                    if (pos_cursor < truePos) --truePos;
                }
            } if (!erased) storedVals.insertAfter(peekPrev());
        } moveFront();
        for (int i = 0; i < truePos; ++i) moveNext();
        storedVals.clear();
    }
    
    // concat()
    // Returns a new List consisting of the elements of this List, followed by
    // the elements of L. The cursor in the returned List will be at postion 0.
    List List::concat(const List& L) const{
        Node* temp = nullptr;
        List newList = *this;
        for (temp = L.backDummy->prev; temp != L.frontDummy; temp = temp->prev) {
            newList.insertAfter (temp->data);
        } newList.moveFront();
        return newList;
    }

    // to_string()
    // Returns a string representation of this List consisting of a comma 
    // separated sequence of elements, surrounded by parentheses.
    std::string List::to_string() const{
        std::string s = "(";
        Node* temp = nullptr;
        for (temp = frontDummy->next; temp != backDummy; temp = temp->next) {
            if (temp != frontDummy->next) {
                s += ", ";
            } s += std::to_string(temp->data);
        } s += ") ";
        return (s);
    }

    // equals()
    // Returns true if and only if this List is the same integer sequence as R.
    // The cursors in this List and in R are unchanged.
    bool List::equals(const List& R) const{
        if (length() != R.length()) return false;
        Node* tempL = frontDummy->next;
        Node* tempR = R.frontDummy->next;
        while (tempL != backDummy) {
            if (tempL->data != tempR->data) return false;
            tempL = tempL->next;
            tempR = tempR->next;
        } return true;
    }

// Overloaded Operators --------------------------------------------------------

// operator<<()
// Inserts string representation of L, as defined by member function 
// to_string(), into stream.
std::ostream& operator<<( std::ostream& stream,  const List& L ) {
    return stream << L.to_string();
}

// operator==()
// Returns true if and only if List A equals List B, as defined by member
// member function equals().
bool operator==(const List& A, const List& B){
    return A.equals(B);
}

// operator=()
// Overwrites the state of this List with state of L, then returns a reference
// to this List.
List& List::operator=( const List& L ) {
    if (this != &L) { // not self assignment
        // make a copy of L
        List temp = L;

        // then swap the copy's fields with fields of this
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(num_elements, temp.num_elements);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(beforeCursor, temp.beforeCursor);
    }

    // return this with the new data installed
    return *this;

    // the copy, if there is one, is deleted upon return
}