#include "loadmanifest.h"






int main()
{
	vector<string> xmfile;

	xmfile.push_back(".repo/manifests/30100013_SP/SDM660.xml");
	xmfile.push_back(".repo/manifests/30100013_SP/MDM.xml");
	xmfile.push_back(".repo/manifests/30100013/QC.xml");
	xmfile.push_back(".repo/manifests/30100013_SP/BSP.xml");
	xmfile.push_back(".repo/manifests/30100013/PFAQ.xml");

	map<string, string> manifest = loadfest(xmfile);

	map<string,string>::iterator pos = manifest.find("platform/packages/apps/LocationInfoQuoter");

	if(pos != manifest.end())
	{
		cout<<pos->second<<endl;
	}

	cout<<manifest.size()<<endl;
}
