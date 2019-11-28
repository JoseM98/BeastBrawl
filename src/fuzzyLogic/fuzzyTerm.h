#pragma once


class FuzzyTerm{
public:
    virtual ~FuzzyTerm(){}
    //all terms must implement a virtual constructor
    virtual FuzzyTerm* Clone()const = 0;
    //retrieves the degree of membership of the term
    virtual double GetDOM()const=0;
    //clears the degree of membership of the term
    virtual void ClearDOM()=0;
    // method for updating the DOM of a consequent when a rule fires
    virtual void ORwithDOM(double val)=0;
};
