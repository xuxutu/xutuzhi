#include "mycomplex.h"

int main(void)
{

	MyComplex mc1(1,1);
	MyComplex mc2(1,1);
	
	mc1+=mc2;
	mc2+=mc1;
	mc2+=mc2;
	cout<<mc2<<endl;
	cout<<mc2+5.6<<endl;

	return 0;
}
