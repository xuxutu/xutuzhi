#include<iostream>
#include<vector>
using namespace std;

//for循环


int main()
{

	for(int i : {2,3,5,7,9})
	{
		cout<<i<<endl;
	}

	vector<int> v{10,100,1000,10000};
	for(auto e : v)
	{
		cout<<e<<endl;
	}

	return 0;
}
