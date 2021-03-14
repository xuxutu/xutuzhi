#include "shape.h"
#include <vector>

int main()
{
	vector<Shape*> paint;
	
	Circular cir;
	Rectangle rec;
	Triangle tri;

	paint.push_back(&cir);
	paint.push_back(&rec);
	paint.push_back(&tri);

	//遍历
	for(int i = 0;  i < paint.size() ;i++)
	{

		paint[i]->draw();
	}


	Shape* s = paint[0];

	return 0;
}
