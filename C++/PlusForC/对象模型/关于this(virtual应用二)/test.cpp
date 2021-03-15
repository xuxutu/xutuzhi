#include<iostream>
using namespace std;

class CDoc
{
public:
    void OnFileOpen()	
    {
	cout<<"step 1"<<endl;
	Serialize();
	cout<<"step 3"<<endl;
    } 
	
    virtual void Serialize(){}
};


class CMyDoc : public CDoc
{
public:
    virtual void Serialize()
    {
	cout<<"step 2"<<endl;

    }		

};



int main(void)
{
    CMyDoc c;
    c.OnFileOpen();
    


    return 0;
}
