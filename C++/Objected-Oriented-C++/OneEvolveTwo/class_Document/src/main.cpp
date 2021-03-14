#include "document.h"

class Mydoc : public Document
{
public:
	void serialize()		
	{
		cout<<"第四步"<<endl;
	}
};


int main()
{
	Mydoc doc;
	doc.OnFileOpen();

	return 0;
}
