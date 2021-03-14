#include <iostream>
#include "loadmanifest.h"



int main()
{
	vector<string> xml;
	
	xml.push_back(".repo/manifests/30100013_SP/SDM660.xml");
	xml.push_back(".repo/manifests/30100013_SP/MDM.xml");
	xml.push_back(".repo/manifests/30100013/QC.xml");
	xml.push_back(".repo/manifests/30100013_SP/BSP.xml");
	xml.push_back(".repo/manifests/30100013/PFAQ.xml");

	
	map<string, string> pro;


	loadfest(xml, pro);


	auto pos = pro.find("platform/packages/apps/ARCore");

	if(pos != pro.end());
	{
		cout<<pos->second<<endl;
	}

	return 0;
}
