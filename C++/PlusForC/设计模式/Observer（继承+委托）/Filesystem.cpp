#include<iostream>
#include<vector>
#include<string>
using namespace std;

//抽象文件
class Document
{
public:
	Document(string dname) : name(dname) {}
	virtual void add(Document*) {}
	virtual void showname(){}
public:
	string name;
};

//目录
class Dir : public Document
{
public:	
	Dir(string dname) : Document(dname) {} 
	void add(Document* doc)
	{
		d.push_back(doc);
	}
	
	void showfile()
	{
		cout<<name<<endl;
		for(int i=0;i<d.size();i++)
		{
			d[i]->showname();
		}
	}
	
	void showname()	
	{
		cout<<name<<endl;
		for(int i=0;i<d.size();i++)
		{
			d[i]->showname();
		}
	}

private:
	vector<Document*> d;
};

//文件
class File : public Document	
{
public:
	File(string fname) : Document(fname){}
	void showname()
	{
		cout<<name<<endl;
	} 
};

int main(void)
{
	//创建根目录
	Dir dir("/");
	//根目录下创建一个子目录和文件
	Dir dir1("book");
	Dir dir2("bin");
	
	File f1("舞动乾坤.txt");	
	File f2("斗破苍穹.txt");
	File f3("吞噬星空.txt");

	dir.add(&dir1);
	dir.add(&f1);
	dir1.add(&f2);
	dir1.add(&dir2);
	dir2.add(&f3);

	dir.showfile();
	
	return 0;
}
