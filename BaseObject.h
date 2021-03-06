#pragma once
#include <bits/stdc++.h>
#include "BaseData.h"
using namespace std;

class BaseObject {
protected:
    vector<BaseData*> DataSet;
public:
	virtual void addData(BaseData*);
	virtual BaseData* getData(DataType) const;
    virtual vector<BaseData*> getDataSet() const;
    virtual void printObject (ofstream &);
    virtual int getDataSize() const;
    virtual ~BaseObject() {
		for (auto it : DataSet)
			delete it;
	}
};
