#ifndef __DOCUMENT__
#define __DOCUMENT__
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//抽象文件
class Document
{
public:
	Document(string name) : fileName(name){} 
	virtual void add(){}
	virtual void showName() = 0;
public:
	string fileName;	
};


//文件
class File : public Document
{
public:
	File(string name) : Document(name){}
	void showName(){cout<<fileName<<" ";}	
};


//目录
class Dir : public Document
{
public:
	Dir(string name) : Document(name){}
	
	void add(Document* d)
	{
		doc.push_back(d);
	}	

	void showName()
	{
		cout<<" "<<fileName<<"/";
		for(auto it : doc)
		{
			it->showName(); 
		}
	}
private:
	vector<Document*> doc;
};
#endif
