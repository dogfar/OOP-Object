#include <bits/stdc++.h>
#include <direct.h>
#include <io.h>
#include "RottenTomatoCrawler.h"
using namespace std;

#define DEBUG

#define SEARCH(content, str) regex_search (content, matchstring, regex(str))
#define SEARCH_CHK(content, str) if (SEARCH(content, str) == false) return false

string trim(string s){
	if(s.empty()){return s;}
	s.erase(0,s.find_first_not_of(" "));
	return s;// remove the space before string
}
vector<BaseObject*>& RottenTomatoCrawler::getObject(){
	return ObjectList;
} 
vector<DataType> RottenTomatoCrawler::getDatatype(){
	return TargetDataList;
}
BaseData* RottenTomatoCrawler::newData(DataType type){
	BaseData* data=NULL;
	if(type==_rating){
		data= new DoubleData(type);
	}
	else if(type==_date || type==_director || type==_writers || type==_runtime || type==_producer || type==_earnings){
		data = new StringData(type);
	}
	else data = new VectorStringData(type);
	return data;
}
void RottenTomatoCrawler::addTargetData(const DataType &datatype){
	TargetDataList.push_back(datatype);
} 
void RottenTomatoCrawler::addObject(BaseObject* object){
	ObjectList.push_back(static_cast<BaseObject*>(object));
}
void RottenTomatoCrawler::readFile(const string &HTML, string &str){
	ifstream fin(HTML.c_str());
	cout << "READ: " << HTML << endl;
	for(char c; c=fin.get(), c!=EOF; str.push_back(c));
	fin.close();
}
bool RottenTomatoCrawler::getData(string content, BaseData *data){
	smatch matchstring;
	stringstream ss;
	stringstream tmp;//string to double 
	double rating_double=0;
	switch(data->getType()){
		cout << data->getType() << " "; 
		case _name:
			SEARCH_CHK(content, "https://www.rottentomatoes.com/m/([^\"/]*)");
			if(matchstring[1].str().length()==0)return false;
			data->setString(matchstring[1].str());
			return true;
		case _producer:
			SEARCH_CHK(content, "target=.movie-studio.>([^<]*)</a>");
			if(matchstring[1].str().length()==0)return false;
			data->setString(matchstring[1].str());
			return true;
		case _runtime:
			SEARCH_CHK(content, "<time datetime=(.*)>\n(.*) minutes"); 
			if(matchstring[0].str().length()==0){return false;} 
			data->setString(trim(matchstring[2].str())+" minutes");
			return true; 
		case _earnings:
			SEARCH_CHK(content, "Box Office: </div>\n(.*)<div class=.meta-value.>([^<]*)</div>");  
			if(matchstring[0].str().length()==0){return false;}
			data->setString(matchstring[2].str());
			return true;
		case _awards:
			return false;
		case _language:
			return false;
		case _country:
			return false;
		case _director:
			SEARCH_CHK(content, "Directed By: </div>");  
			if(matchstring[0].str().length()==0)return false;
			content=matchstring.suffix().str();
			SEARCH_CHK(content, "<a(.*)>(.*)</a>");
			data->setString(matchstring[2].str());
			return true;
		case _writers:
			SEARCH_CHK(content, "Written By: </div>"); 
			if(matchstring[0].str().length()==0)return false;
			content=matchstring.suffix().str();
			SEARCH_CHK(content, "<a(.*)>(.*)</a>"); 
			data->setString(matchstring[2].str());
			return true;
		case _date:
			SEARCH_CHK(content, "In Theaters: </div>"); 
			if(matchstring[0].str().length()==0){cout << "FDA" << endl;return false;}
			content=matchstring.suffix().str();
			SEARCH_CHK(content,"<time datetime=(.*)>(.*)</time>");
			data->setString(matchstring[2].str());
			return true;
		case _labels:
			SEARCH_CHK(content, "Genre: </div>"); 
			if(matchstring[0].str().length()==0){return false;}
			content=matchstring.suffix().str();
			SEARCH_CHK(content, "<a(.*)>\n(.*)</a></div>");
			data->setString(trim(matchstring[2].str()));
			content=matchstring.suffix().str();
			return true;
		case _actors:
			SEARCH_CHK(content,"Cast</h2>");  
			if(matchstring[0].str().length()==0)return false;
			content=matchstring.suffix().str();
			while(SEARCH(content, "<span title=.(.*).>")){
				data->setString(matchstring[1].str());
				content=matchstring.suffix().str();
			}
			return true;
		case _rating:
			SEARCH_CHK(content, "<span class=.meter-value superPageFontColor.><span>(.*)</span>"); 
			if(matchstring[1].str().length()==0)rating_double=-1;
			else {
				ss << matchstring[1].str();
				ss >> rating_double;
				rating_double=rating_double/10;
			}
			data->setDouble(rating_double);
			return true;
		case _reviews:
			while(SEARCH(content, "<p>\n(.*)</p>\n(.*)<div class=.pull-right smaller.>")){ //  
				data->setString(trim(matchstring[1].str()));
				content=matchstring.suffix().str();
			}
			return true;
		case _photos:
			SEARCH_CHK(content, "<img src=\"(.*)/206x305/([^\"]*)"); 
			if(matchstring[0].str().length()==0)return false;
			string pic=matchstring[1].str()+"/206x305/"+matchstring[2].str();
			data->setString(pic);
			return true;
		}
}
void RottenTomatoCrawler::init() {
	TargetDataList = readConfiguration ("rottentomato-configuration.txt");
} 

ofstream ouf ("test.out");

void RottenTomatoCrawler::downloadPics(BaseObject *object){
	vector<string> PhotoList=object->getData(_photos)->getVectorString();
	for(auto it: PhotoList){
		cout << "Photo: " << it << endl;
		index++;
		catcher.saveFile(it, "pic/"+to_string(index)+".jpg", true);
	}
}
BaseObject* RottenTomatoCrawler::scanPage(const string& HTML){
	string content;
	readFile(HTML, content);
	BaseObject* object= new BaseObject;
	BaseData *data;
	for(auto it: RottenTomatoMovieDataList){
		data=newData(it);
		getData(content, data);
		object->addData(data);
#ifdef DEBUG
    	data->printData(ouf), ouf << endl; 
#endif
	}
#ifdef DEBUG
	ouf << endl;
#endif
	fprintf(stderr, "Scanned: %s\n", (object->getData(_name)->getVectorString())[0].c_str());
	return object;
} 
void RottenTomatoCrawler::work(){
	ofstream _ouf ("con");
	string StartUrl = "https://www.rottentomatoes.com/top/" ;
	vector<string> QUrl {StartUrl};
    map<string, bool> visited;
    
    system (("md " + string("pic")).c_str());
  //  srand ((unsigned) time (NULL));
    for (int i = 0; i < QUrl.size() && i <= 1000; i++) {
      //  clock_t delay = (rand() % 100 + 100) / 400 * CLOCKS_PER_SEC, start = clock();
    	smatch matchstring;
    	string Url=QUrl[i],content;
    	string HTML="page"+to_string(i)+".html";
    	catcher.saveFile(Url,HTML,true);
    	readFile(HTML,content);
    	while(SEARCH(content, "https://www.rottentomatoes.com/m/([^\"/]*)")){
    		if(!visited[matchstring[1]]){
    			visited[matchstring[1]]=1;
    			QUrl.push_back(matchstring[0].str());
			}
			content=matchstring.suffix().str();
		}
		if(i){
			_ouf << i << endl;
			BaseObject* object=scanPage(HTML);
			downloadPics(object);
			addObject(object);
		}
	//	while(clock()-start<delay);
	} 
	_ouf << QUrl.size() << endl;
	for(int i=0;i<QUrl.size();i++){
		_ouf << QUrl[i] << endl;
	}
}

