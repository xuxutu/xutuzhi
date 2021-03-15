#include<iostream>
using namespace std;

class Document
{
public:
	Document(){}
	//非虚函数，我不希望子类重写
	void OnFileOpen()		
	{
		cout<<"第一"<<endl;
		cout<<"第二"<<endl;
		Serialize();	   //this->Serialize();this是子类
		cout<<"第四"<<endl;
	}
	//虚函数，子类可以重写
	virtual void Serialize(){}//此处加了virtual关键字才会动态绑定。

};


class MyDoc : public Document
{
public:
	MyDoc(){}
	//void OnFileOpen(){cout<<"haha"<<endl;}	
	void Serialize()
	{
		cout<<"第三"<<endl;
	} 
	void speak(){cout<<"er zi"<<endl;}; 
};


int main()
{
	MyDoc mydoc;
	mydoc.OnFileOpen();//Document::OnFileOpen(&mydoc);
	//mydoc.Document::OnFileOpen();//默认调用子类同名函数，调用父类函数时需要显示指明。


	return 0;	
}
