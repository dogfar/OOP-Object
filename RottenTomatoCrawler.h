#pragma once
#include <bits/stdc++.h>
#include "BaseCrawler.h"
#include "BaseObject.h"
using namespace std;

static Catcher &catcher = Catcher::getCatcher();

static vector<DataType> RottenTomatoMovieDataList={
	_name, 
	_rating,
	_actors,
	_director,
	_writers,
	_labels,
	_date,
	_earnings,
	_runtime,  
	_producer,  
	_photos,
	_reviews,
};

class RottenTomatoCrawler : public BaseCrawler{
	vector<DataType> TargetDataList;
	vector<BaseObject* > ObjectList;
	int index=0; 
public:
	BaseData* newData(DataType type);
	virtual ~RottenTomatoCrawler(){
		for (auto it: ObjectList)
		    delete it;
	}
	virtual void addTargetData (const DataType &);
	virtual void addObject(BaseObject*);
	virtual void init();
	virtual void work();
	virtual vector<BaseObject*>& getObject();
	virtual vector<DataType> getDatatype();
	bool getData(string content, BaseData *data);
	void readFile(const string&, string&);
	void downloadPics(BaseObject *object);
	BaseObject* scanPage(const string& HTML);
};
 
