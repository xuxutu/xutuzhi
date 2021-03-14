#include "fs.h"


int main()
{
	Dir d("/");	
	File f1("1.txt");
	d.add(&f1);
	Dir d2("home");
	d.add(&d2);
	File f2("2.txt");
	d2.add(&f2);

	Dir d3("下载");
	d.add(&d3);
	File f3("3.txt");	
	d3.add(&f3);

	Dir d4("桌面");
	d2.add(&d4);

	d.showName();
	cout<<endl;
	return 0;
}

