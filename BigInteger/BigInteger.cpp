/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{6}
* BigInteger.cpp
* BigInteger ADT Program
***/ 
#include"List.h"
#include"BigInteger.h"

#define POWER 9
#define BASE 1000000000

void negateList(List& L);
void sumList(List& S, List A, List B, int sgn);
int normalizeList(List& L);
void shiftList(List& L, int p);
void scalarMultList(List& L, ListElement m);

using namespace std;

// Class Constructors & Destructors -------------------------------------------

   // BigInteger()
   // Constructor that creates a new BigInteger in the zero state: 
   // signum=0, digits=().
   BigInteger::BigInteger() {
      signum = 0;
      digits = List();
   }

   // BigInteger()
   // Constructor that creates a new BigInteger from the long value x.
   BigInteger::BigInteger(long x) : BigInteger(::to_string(x)) {}

   // BigInteger()
   // Constructor that creates a new BigInteger from the string s.
   // Pre: s is a non-empty string consisting of (at least one) base 10 digit
   // {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
   BigInteger::BigInteger(string s) {
      //check if length is non-zero
      if (!s.length()) throw invalid_argument("BigInteger: Constructor: empty string");

      //check if first character is sign, and modify signum
      signum = 1;
      switch (s[0]) {
         case '+':
            signum = 1;
            s.erase(0, 1);
            break;
         case '-':
            signum = -1;
            s.erase(0, 1);
      } if (!isdigit(s[0]))throw invalid_argument("BigInteger: Constructor: non-numeric string");

      //check for non-digit chars
      int lengthString = s.length();
      int temp_index = lengthString;
      bool notZero = false;
      long term = 0;
      for (int i = 0; i < lengthString; ++i) {
         if (isalpha(s[lengthString-i])) throw invalid_argument("BigInteger: Constructor: non-numeric string");
         if (i && !(i % POWER)) {
            temp_index = lengthString-i;
            term = (ListElement) stol((s.substr(temp_index, POWER)));
            digits.insertAfter(term);
            if (term) notZero = true;
         }
      } 

      //add remaining digits
      term = (ListElement) stol((s.substr(0, temp_index)));
      if (term) digits.insertAfter(term);
      if (digits.length() && !digits.front()) digits.eraseAfter();
      if (!notZero && !term) signum = 0;
   }


   // BigInteger()
   // Constructor that creates a copy of N.
   BigInteger::BigInteger(const BigInteger& B) {
      signum = B.signum;
      digits = B.digits;
   }


// Access functions -----------------------------------------------------------

   // sign()
   // Returns -1, 1 or 0 according to whether this BigInteger is positive, 
   // negative or 0, respectively.
   int BigInteger::sign() const{
      return signum;
   }

   // compare()
   // Returns -1, 1 or 0 according to whether this BigInteger is less than N,
   // greater than N or equal to N, respectively.
   int BigInteger::compare(const BigInteger& N) const{
      if (sign() > N.sign()) return 1;
      else if (sign() < N.sign()) return -1;
      else if (digits.length() > N.digits.length()) return 1;
      else if (digits.length() < N.digits.length()) return -1;

      List thisDigits = digits;
      List NDigits = N.digits;
      
      thisDigits.moveFront();
      NDigits.moveFront();

      while (thisDigits.position() < thisDigits.length()) {
         if (thisDigits.peekNext() > NDigits.peekNext()) return 1;
         else if (thisDigits.peekNext() < NDigits.peekNext()) return -1;
         thisDigits.moveNext();
         NDigits.moveNext();
      } return 0;
      
   }

// Manipulation procedures -------------------------------------------------

   // makeZero()
   // Re-sets this BigInteger to the zero state.
   void BigInteger::makeZero() {
      signum = 0;
      digits.clear();
   }

   // negate()
   // If this BigInteger is zero, does nothing, otherwise reverses the sign of 
   // this BigInteger positive <--> negative. 
   void BigInteger::negate() {
      if (!signum) return;
      signum = -signum;
   }

// BigInteger Arithmetic operations ------------------------------------------------------------

   // add()
   // Returns a BigInteger representing the sum of this and N.
   BigInteger BigInteger::add(const BigInteger& N) const{
      if (!sign()) return N;
      if (!N.sign()) return *this;
      BigInteger sum = BigInteger();
      if (sign() != N.sign()) {
         if (sign() < 0) sumList(sum.digits, N.digits, digits, -1);
         else sumList(sum.digits, digits, N.digits, -1);
         sum.signum = normalizeList(sum.digits);
      } else {
         sumList(sum.digits, digits, N.digits, 1);
         sum.signum = normalizeList(sum.digits);
         if (sign() < 0) sum.signum = -1;
      }
      return sum;
   }

   // sub()
   // Returns a BigInteger representing the difference of this and N.
   BigInteger BigInteger::sub(const BigInteger& N) const{
      if (!this->compare(N)) return 0;
      BigInteger subt = BigInteger (N);
      subt.signum *= -1;
      return this->add (subt);
   }

   // mult()
   // Returns a BigInteger representing the product of this and N. 
   BigInteger BigInteger::mult(const BigInteger& N) const{
      List mulBottom = List(N.digits);
      BigInteger prod = BigInteger();
      int multiplier = 0;

      prod.digits.insertAfter(0);

      for (mulBottom.moveBack(); mulBottom.position() > 0; mulBottom.movePrev()) {
         if (mulBottom.peekPrev()) {
            BigInteger top = BigInteger(*this);
            scalarMultList(top.digits, mulBottom.peekPrev());
            shiftList(top.digits, multiplier);
            prod += top;
            normalizeList(prod.digits);
         } ++multiplier;
      } normalizeList(prod.digits);

      prod.signum = this->sign() * N.sign();
      return prod;
   }

// Other operations ------------------------------------------------------------

   // negateList()
   // Changes the sign of each integer in List L. Used by sub().
   void negateList(List& L) {
      for (L.moveFront(); L.position() < L.length(); L.moveNext()) L.setAfter(L.peekNext() * -1);
   }

   // sumList()
   // Overwrites the state of S with A + sgn*B (considered as vectors).
   // Used by both sum() and sub().
   void sumList(List& S, List A, List B, int sgn) {
      if (sgn == -1) negateList(B);
      A.moveFront();
      B.moveBack();
      if (!A.peekNext()) S = B;
      else if (!sgn) S = A;
      else {
         A.moveBack();
         if (A.length() >= B.length()) {
            while (B.position() > 0) {
               S.insertAfter(A.peekPrev() + B.peekPrev());
               A.movePrev();
               B.movePrev();
            } while (A.position() > 0) {
               S.insertAfter(A.peekPrev());
               A.movePrev();
            }
         } else if (A.length() < B.length()) {
            while (A.position() > 0) {
               S.insertAfter(A.peekPrev() + B.peekPrev());
               A.movePrev();
               B.movePrev();
            } while (B.position() > 0) {
               S.insertAfter(B.peekPrev());
               B.movePrev();
            }
         }
      }
   }

   // normalizeList()
   // Performs carries from right to left (least to most significant
   // digits), then returns the sign of the resulting integer. Used
   // by add(), sub() and mult().
   int normalizeList(List& L) {
      //default sign value
      int sign = 1;

      //zero boolean
      bool zeroTerm = true;

      //checking if sum is negative or positive
      L.moveFront();
      if (L.peekNext() < 0) {
         negateList(L);
         sign = -1;
      }

      //adding extra space at the end for carry if there is one
      L.moveFront();
      L.insertAfter(0);

      //normalize lol
      L.moveBack();
      for (L.movePrev(); L.position() > 0; L.movePrev()) { //put cursor in between sum and carry

         //only normalize if ListElement < 0 or > BASE
         if (L.peekNext() >= BASE || L.peekNext() < 0) {

            //calculates carry
            long carry = L.peekNext() / BASE;
            if (L.peekNext() < 0) --carry;

            //calculates sum
            long sum = L.peekNext() % BASE;

            //set carry and sum
            L.setBefore(L.peekPrev() + carry);
            L.setAfter(sum);

            //corrects negative sum
            if (L.peekNext() < 0) L.setAfter(L.peekNext() + BASE);
         }

         //check for 0 term
         if (L.peekNext()) zeroTerm = false;
      }

      //deletes extra space if unused
      if (!L.front()) {
         L.moveFront();
         L.eraseAfter();
      }

      //return 0 if all ListElements are 0
      if (zeroTerm && !L.front()) return 0;
      return sign;
   }

   // shiftList()
   // Prepends p zero digits to L, multiplying L by base^p. Used by mult().
   void shiftList(List& L, int p) {
      L.moveBack();
      for (int i = 0; i < p; ++i) {
         L.insertBefore(0);
      }
   }

   // scalarMultList()
   // Multiplies L (considered as a vector) by m. Used by mult().
   void scalarMultList(List& L, ListElement m) {
      for (L.moveBack(); L.position() > 0; L.movePrev()) L.setBefore(L.peekPrev() * m);
   }

   // to_string()
   // Returns a string representation of this BigInteger consisting of its
   // base 10 digits. If this BigInteger is negative, the returned string 
   // will begin with a negative sign '-'. If this BigInteger is zero, the
   // returned string will consist of the character '0' only.
   std::string BigInteger::to_string() {
      if (!signum) return "0";
      string s = "";
      if (signum == -1) s += "-";
      for (digits.moveFront(); digits.position() < digits.length(); digits.moveNext()) {
         if (digits.position()) for (int i = std::to_string(digits.peekNext()).length(); i < POWER; ++i) s += "0";
         else if (!digits.peekNext()) digits.eraseAfter();
         s += std::to_string(digits.peekNext());
      } return s;
   }

// Overloaded Operators --------------------------------------------------------

   // operator<<()
   // Inserts string representation of N into stream.
   std::ostream& operator<<(std::ostream& stream, BigInteger N) {
      return stream << N.to_string();
   }

   // operator==()
   // Returns true if and only if A equals B. 
   bool operator==(const BigInteger& A, const BigInteger& B) {
      return A.compare(B) == 0;
   }

   // operator<()
   // Returns true if and only if A is less than B. 
   bool operator<(const BigInteger& A, const BigInteger& B) {
      return A.compare(B) < 0;
   }

   // operator<=()
   // Returns true if and only if A is less than or equal to B. 
   bool operator<=(const BigInteger& A, const BigInteger& B) {
      return A.compare (B) <= 0;
   }

   // operator>()
   // Returns true if and only if A is greater than B. 
   bool operator>(const BigInteger& A, const BigInteger& B) {
      return A.compare(B) > 0;
   }

   // operator>=()
   // Returns true if and only if A is greater than or equal to B. 
   bool operator>=(const BigInteger& A, const BigInteger& B) {
      return A.compare(B) >= 0;
   }

   // operator+()
   // Returns the sum A+B. 
   BigInteger operator+(const BigInteger& A, const BigInteger& B) {
      return A.add(B);
   }

   // operator+=()
   // Overwrites A with the sum A+B. 
   BigInteger operator+=(BigInteger& A, const BigInteger& B) {
      A = A.add(B);
      return A;
   }

   // operator-()
   // Returns the difference A-B. 
   BigInteger operator-(const BigInteger& A, const BigInteger& B) {
      return A.sub(B);
   }

   // operator-=()
   // Overwrites A with the difference A-B. 
   BigInteger operator-=(BigInteger& A, const BigInteger& B) {
      A = A.sub(B);
      return A;
   }

   // operator*()
   // Returns the product A*B. 
   BigInteger operator*(const BigInteger& A, const BigInteger& B) {
      return A.mult(B);
   }

   // operator*=()
   // Overwrites A with the product A*B. 
   BigInteger operator*=(BigInteger& A, const BigInteger& B) {
      A = A.mult(B);
      return A;
   }