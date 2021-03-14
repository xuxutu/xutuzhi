#ifndef __BUILDLISTS__
#define __BUILDLISTS__
#include "buildframe.h"

class Build_XH73Q : public BuildFrame
{
protected:
	Build_XH73Q(int dummy)
	{
		_id = count++;
	}

public:
	BuildFrame* clone(){ return new Build_XH73Q(1); }

	PhoneType returnType(){ return XH73Q;}

	void build();

private:
	static Build_XH73Q BUILD_XH73Q;
	Build_XH73Q()
	{
		addPrototype(this);
	}
	
	int _id;
	static int count;
};

Build_XH73Q Build_XH73Q::BUILD_XH73Q;//allocate memory.
int Build_XH73Q::count;//allocate memory.
void Build_XH73Q::build()
{
	cout<<"5.start build!"<<endl;
	MutiCmd cmd_build;
	SinCmd cmd_source("source build/envsetup.sh", "/home/zhaoyong/sp2/code/XH73Q/LINUX/android");
	cmd_source.excute();		
	SinCmd cmd_chooseCombo("choosecombo 1 fih_sdm660_64_OF6_00WW userdebug", "/home/zhaoyong/sp2/code/XH73Q/LINUX/android");
	cmd_chooseCombo.excute();

	SinCmd cmd_makeUpdateApi("make update-api", "/home/zhaoyong/sp2/code/XH73Q/LINUX/android");
	cmd_makeUpdateApi.excute();
	SinCmd cmd_make("make -j8", "/home/zhaoyong/sp2/code/XH73Q/LINUX/android");
	cmd_make.excute();
	//cmd_build.addCmd(&cmd_source);

	//cmd_build.addCmd(&cmd_chooseCombo);
	//cmd_build.addCmd(&cmd_makeUpdateApi);
	//cmd_build.addCmd(&cmd_make);

	//cmd_build.excute();		
}

#endif
