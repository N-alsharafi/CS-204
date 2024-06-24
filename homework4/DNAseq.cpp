//
// Created by nuh al-sharafi on 4/3/24.
//

#include "DNAseq.h"

//basic function implementations

DNAseq :: DNAseq()
//Default constructor
{
    length = 0;
    sequence = nullptr;
}


DNAseq :: DNAseq(const string& input)
//parametric constructor
{
    //initialize internal variables
    length = input.size();
    sequence = new Nucleotide[length];

    //put the string input into the list
    for(int i=0; i<length; i++)
    {
        sequence[i] = stringToNucleotide(input[i]);
    }
}


DNAseq :: DNAseq(const DNAseq& copy)
//deep-copy constructor
{
    //This function is inspired by Lab-7 MathVector.cpp
    length = copy.length;
    sequence = new Nucleotide[length];

    for(int i=0; i < length; i++)
    //copy individual elements
    {
        sequence[i] = copy.sequence[i];
    }
}


DNAseq :: ~DNAseq()
//destructor
{
    length = 0;
    delete [] sequence;
}


//operator overloads for member functions

bool DNAseq :: operator <= (const DNAseq& rhs) const
// <= operator overload
{
    /* Function checks if object is a subsequence of rhs
     * also returns true if rhs == object
     * Therefore, it is safe to assume rhs->length >= object
     * */

    //check the size to verify that we can compare
    if(this->length > rhs.length)
    //lfs is larger and cannot therefore be a substring of rhs
    {
        return false;
    }
    else if(this->length == rhs.length)
    //both have identical length, check if they have identical nucleotides
    {
        return (*this == rhs);
    }
    else
    //lfs is shorter than rhs, check if it's a sequence
    {
        return rhs.sequenceExists(*this);
    }
}


int DNAseq :: operator % (const Nucleotide& rhs) const
// % operator overload
{
    /* This function counts the number of occurrences of a given Nucleotide (rhs)
     * in the object on which it is called
     * */

    int counter = 0;

    for(int i=0; i< this->length; i++)
    //loop over object to count occurrences
    {
        if(this->sequence[i] == rhs)
        //identical Nucleotide found
        {
            counter++;
        }
    }

    return counter;
}


DNAseq DNAseq ::operator ! () const
// ! operator overload
{
    /* This function would find the complement of an operator.
     * meaning it would switch each Nucleotide with it's complement
     * */

    DNAseq newSequence = DNAseq(*this); //make a copy of object

    for (int i=0; i<newSequence.length ; i++)
    //loop over the new object and find the complement
    {
        complement(newSequence.sequence[i]);
    }

    return newSequence; //return object by value
}


const DNAseq& DNAseq ::operator = (const DNAseq &rhs)
// = operator overload
{
    /* This function discards the content of the object on which it is called and
     * assigns a copy of the content of rhs to it
     * essentially x=3 where x is the object and 3 is rhs
     * */

    //function heavily inspired by Lab-7 MathVector.cpp

    if (this != &rhs)
    //check if lfs is not already equal to rhs
    {
        //first delete lfs
        length = 0;
        delete [] sequence;

        //create new vector for lfs
        length = rhs.length;
        sequence = new Nucleotide[length];

        //assign content
        for (int i=0; i<length; i++)
        {
            sequence[i] = rhs.sequence[i];
        }
    }

    return *this;
}


DNAseq& DNAseq :: operator += (DNAseq& rhs)
// += operator overload
{
    /* Function works like the first + operator except that it modifies lfs
     * should also have a return value to allow for cascading
     * */

    *this = *this + rhs;

    return *this;
}


// operator overloads for free functions

DNAseq operator * (const DNAseq& lhs, int rhs)
// * operator overload
{
    /* Function takes a DNAseq and repeats it rhs times
     * */

    Nucleotide* sequence = lhs.getSequence();
    string seq = ""; // NOLINT(*-redundant-string-init)

    for (int i = 0; i< rhs; i++)
    //repeat rhs times to copy the sequence
    {
        seq += lhs.getSequenceStr();
    }

    //construct new DNAseq
    DNAseq result = DNAseq(seq);
    return result;
}


DNAseq operator + (const DNAseq& lhs, const DNAseq& rhs)
// first + operator overload
{
    /* Function will check if first nucleotide of rhs is in lhs
     * if true, it will insert rhs after said nucleotide
     * if not, it will insert rhs at the end of lhs
     * */

    string result;
    int index=-1;

    //check if first nucleotide exists
    if(lhs.findNucleotide(rhs.getSequence()[0], index))
    //the nucleotide exists, going to insert mid-sequence
    {
        string lhsSequence = lhs.getSequenceStr();
        result = lhsSequence.substr(0,index+1) + rhs.getSequenceStr() + lhsSequence.substr(index+1);
    }
    else
    //nucleotide does not exist, going to insert after end of lhs
    {
        result = lhs.getSequenceStr() + rhs.getSequenceStr();
    }

    //make a new object and return it
    DNAseq resultObj = DNAseq(result);
    return resultObj;
}


DNAseq operator + (const Nucleotide& lhs, const DNAseq& rhs)
//second + operator overload
{
    /* Function takes a lhs Nucleotide and pre-pends it to
     * a copy of rhs
     * */

    //construct the new sequence string and add the first nucleotide
    char nucleotide = nucleotideToString(lhs);
    string newSequence;
    newSequence += nucleotide;

    //get sequence
    Nucleotide* sequence = rhs.getSequence();

    //add the remaining nucleotides
    newSequence += rhs.getSequenceStr();

    DNAseq result = DNAseq(newSequence);
    return result;
}


DNAseq operator - (const DNAseq& lhs, const DNAseq& rhs)
// - operator overload
{
    /* Function return lhs with rhs removed from it, if rhs is a subsequence of lhs
     * Function returns lhs unchanged if not
     * */


    if(rhs <= lhs)
    //check if rhs is a subsequence of lhs
    {
        int rhsLength = rhs.getLength();

        //find the starting position
        int index = lhs.findSubsequence(rhs);

        //construct the new subsequence
        string lhsString = lhs.getSequenceStr();
        string result = lhsString.substr(0, index) + lhsString.substr(index+rhsLength);
        DNAseq resultSeq = DNAseq(result);
        return resultSeq;
    }
    else
    //rhs is not a subsequence of lhs
    {
        DNAseq result = DNAseq(lhs.getSequenceStr());
        return result;
    }

}


ostream& operator << (ostream& output, const DNAseq& seq)
// << operator overload
{
    /* This function allows the object sequence contents to be output
     * it will return ostream to allow for cascading
     * */

    //Function inspired by clockt.cpp from week 7 materials

    ostringstream ostr;
    Nucleotide* sequence = seq.getSequence();

    for(int i=0; i< seq.getLength(); i++)
    {
        ostr << nucleotideToString(sequence[i]);
    }

    output << ostr.str();
    return output;
}


//helper function implementations

void complement(Nucleotide& nuc)
{
    /* This function takes a nucleotide and returns its complement
     * A -> T
     * T -> A
     * G -> C
     * C -> G
     * */

    if(nuc == A)
    {
        nuc = T;
    }
    else if (nuc == T)
    {
        nuc = A;
    }
    else if (nuc == G)
    {
        nuc = C;
    }
    else
    {
        nuc = G;
    }
}


int DNAseq :: getLength() const
//accessor to get array length
{
    return this->length;
}


Nucleotide* DNAseq :: getSequence() const
//accessor to get array
{
    return this->sequence;
}


Nucleotide stringToNucleotide(const char& letter)
//convert string to Nucleotide
{
    if(letter == 'A')
    {
        return A;
    }
    else if(letter == 'T')
    {
        return T;
    }
    else if(letter == 'G')
    {
        return G;
    }
    else
    {
        return C;
    }
}


char nucleotideToString(const Nucleotide& nuc)
//convert Nucleotide to char
{
    if(nuc == A)
    {
        return 'A';
    }
    else if(nuc == T)
    {
        return 'T';
    }
    else if(nuc == G)
    {
        return 'G';
    }
    else
    {
        return 'C';
    }
}


bool operator == (const DNAseq& lhs, const DNAseq& rhs)
{
    /* Operator checks if lhs and rhs are an identical sequence
     * function assumes lhs and rhs are of identical length
     * */

    //initialize flag and get sequences
    bool identical = true;
    Nucleotide* lhsSequence = lhs.getSequence();
    Nucleotide* rhsSequence = rhs.getSequence();


    for(int x = 0; x < lhs.getLength(); x++)
    //check all elements and update flag
    {
        if(lhsSequence[x] != rhsSequence[x])
        //found a contradiction
        {
            identical = false;
        }
    }

    return identical;
}


bool DNAseq :: sequenceExists(const DNAseq& seq) const
{
    /* Function checks if seq is a subsequence of object
     * it is assumed that length of object > seq
     */

    bool subsequence = true;

    for(int i=0; i< this->length - seq.length + 1; i++)
    //loop over the possible beginnings of the subsequence
    {
        //reset flag before each attempt
        subsequence = true;

        //for each possible starting position, check if we have the subsequence
        for(int j = 0; j< seq.length; j++)
        {
            if(this->sequence[j + i] != seq.sequence[j])
            {
                subsequence = false;
            }
        }

        //we check if subsequence is still true, meaning we have a subsequence
        if(subsequence)
        {
            return subsequence;
        }
    }

    //did not find subsequence
    return false;
}


string DNAseq ::getSequenceStr() const
//function gets nucleotide sequence as string
{
    //initialize string
    string result;

    //put elements into string
    for(int i = 0; i< this->length; i++)
    {
        result += nucleotideToString(this->sequence[i]);
    }

    return result;
}


bool DNAseq ::findNucleotide(const Nucleotide &nuc, int &location) const
//check if nucleotide exists in object sequence, returns index as location
{
    if(this->sequence != nullptr)
    //make sure it's a non-empty sequence
    {
        for(int i=0; i<this->length; i++)
        {
            if(this->sequence[i] == nuc)
                //function returns the first occurrence of nucleotide
            {
                location = i;
                return true;
            }

        }
    }

    //in case we don't find nucleotide
    location = 0;
    return false;

}


int DNAseq ::findSubsequence(const DNAseq &rhs) const
//function finds the starting index of a subsequence rhs
//function assumes rhs is a subsequence of object, it finds the starting index
{

    for (int i =0; i<this->length - rhs.getLength() + 1; i++)
    //loop over all the possible starting points for the string
    {
        bool subsequence = true;
        Nucleotide* subseq = rhs.getSequence();

        for(int j=0; j< rhs.getLength(); j++)
        //test if this is the starting position of the subsequence
        {
            if(this->sequence[i+j] != subseq[j])
            {
                subsequence = false;
            }
        }

        //return index if it's the start of the subsequence
        if(subsequence)
        {
            return i;
        }
    }

    //should not get here
    return -1;
}