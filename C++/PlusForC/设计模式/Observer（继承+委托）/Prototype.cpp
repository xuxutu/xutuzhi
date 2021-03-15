#include<iostream>
using namespace std;

enum imageType{LAST,SPOT};

class Image
{
public:
	virtual void draw()=0;

	static Image* findAndClone(imageType);

protected:
	virtual imageType returnType()=0;

	virtual Image* clone()=0;

	static void addPrototype(Image* image)
	{
		_prototypes[_nextSlot++]=image;
	}

private:
	static Image* _prototypes[10];
	static int _nextSlot;
};

Image* Image::_prototypes[10];
int Image::_nextSlot;

Image* Image::findAndClone(imageType type)
{
	for(int i=0;i<_nextSlot;i++)
	{
		if(_prototypes[i]->returnType()==type)
		{	
			return _prototypes[i]->clone();
		}
	}

}

class LandSatImage : public Image
{
protected:
	LandSatImage(int dummy)	
	{
		_id=_count++;
	}
public:
	void draw()	
	{
		cout<<"LandSatImage::draw"<<_id<<endl;
	}

	imageType returnType()
	{
		return LAST;	
	}

	Image* clone()
	{
		return new LandSatImage(1);
	}
private:
	static LandSatImage landSatImage;
	LandSatImage()
	{
		addPrototype(this);
	}

	int _id;
	static int _count;
};

LandSatImage LandSatImage::landSatImage;
int LandSatImage::_count=1;

int main()
{
	

}
