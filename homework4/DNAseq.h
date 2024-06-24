//
// Created by nuh al-sharafi on 4/3/24.
//

#ifndef HOMEWORK4_DNASEQ_H
#define HOMEWORK4_DNASEQ_H

//include statements
#include<string>
#include<sstream>
using namespace std;

//enum definition
enum Nucleotide {A,C,G,T};

//class definition

class DNAseq
{
private:

    //private data members
    int length;
    Nucleotide* sequence;

public:

    //basic functions
    DNAseq();                      //default constructor
    DNAseq(const string& input);   //parametric constructor
    DNAseq(const DNAseq &);        //deep-copy constructor
    ~DNAseq();                     //destructor

    //operator overloads
    bool operator <= (const DNAseq& rhs) const;     //The const at the end should signify that the function does not
    int operator % (const Nucleotide& rhs) const;   //modify private members of the class
    DNAseq operator ! () const;
    const DNAseq & operator = (const DNAseq& rhs);
    DNAseq & operator += (DNAseq& rhs);

    //helpers
    int getLength() const;
    Nucleotide* getSequence() const;
    string getSequenceStr() const;
    bool sequenceExists(const DNAseq& seq) const;
    bool findNucleotide(const Nucleotide& nuc, int& location) const;
    int findSubsequence(const DNAseq& rhs) const;

};

//operator overloads as free functions
DNAseq operator * (const DNAseq& lhs, int rhs);
DNAseq operator + (const DNAseq& lhs, const DNAseq& rhs);
DNAseq operator + (const Nucleotide& lhs, const DNAseq& rhs);
DNAseq operator - (const DNAseq& lhs, const DNAseq& rhs);
ostream& operator << (ostream& output, const DNAseq& seq);

//helper functions
void complement(Nucleotide& nuc);
Nucleotide stringToNucleotide(const char& letter);
char nucleotideToString(const Nucleotide& nuc);
bool operator == (const DNAseq& lhs, const DNAseq& rhs);

#endif //HOMEWORK4_DNASEQ_H
