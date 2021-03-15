#include<iostream>
#include<vector>
using namespace std;

class Subject;//声明

class Observer
{
public:
	virtual void update(Subject* sub,int value)=0;

};


class Subject
{
public:
	Subject(int value=0) : m_value(value) {}
	void attach(Observer* obs)
	{
		m_views.push_back(obs);
	}

	void set_val(int value)
	{
		m_value=value;
		notify();
	}

	void notify()
	{
		for(int i=0;i<m_views.size();++i)
		{
			m_views[i]->update(this,m_value);
		}
	}

private:
	int m_value;
	vector<Observer*>m_views;
};


class Ob : public Observer 
{
public:	
	void update(Subject* sub,int value)
	{
		content=value;
	}
	
	void showView()
	{
		cout<<"我看到了"<<content<<endl;
	}

private:
	int content;	
};


int main()
{
	Subject sub;//创建观察物	

	Ob ob[5];	   //创建五个观察者

	for(int i=0;i<5;++i)
	{
		sub.attach(ob+i);
	}	
	
	sub.set_val(8);	

	for(int i=0;i<5;++i)
	{
		ob[i].showView();
	}

	sub.set_val(9);

	for(int i=0;i<5;++i)
	{
		ob[i].showView();
	}


	return 0;	
}
