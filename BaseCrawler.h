#pragma once
#include <bits/stdc++.h>
#include "Catcher.h"
#include "BaseObject.h"
using namespace std;

class BaseCrawler {
/*
    ObjectList is the list of all the downloaded movies
    TargetDataList is the list of all the targeted data
*/
public: 
    virtual void addTargetData (const DataType &) = 0; // add a kind of target data
    virtual void addObject (BaseObject*) = 0; // add an object(movie)
    virtual void init() = 0; // the general interface of initialization
    virtual void work() = 0; // the general interface of main work
    virtual vector<BaseObject*>& getObject() = 0; 
	virtual vector<DataType> getDatatype() = 0; 
	vector<DataType> readConfiguration (string File); 
	virtual ~BaseCrawler() {};
};
