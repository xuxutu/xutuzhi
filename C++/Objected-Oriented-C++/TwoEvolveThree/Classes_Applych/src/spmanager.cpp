#include "spmanager.h"


void SpManager::showticket()const
{
	cout<< "当月作业总票一览:" <<endl;
	for(auto& it : tks)
	{
		cout<< it.tid <<endl;
		cout<< it.cve <<endl;
		cout<< it.project <<endl;
		cout<< it.android_id <<endl;
		cout<< it.patch_level <<endl;
		cout<< it.pnum <<" "<<it.patchfile <<endl;
		cout<<endl;
	}

	cout<< "project list :" <<endl;
	for(auto& it : prolist)
		cout<< it <<endl;
}


void SpManager::showcase()const
{
	cout<< "CVE下补丁一览:" <<endl;
	
    for(int i = 0; i < ptick.size(); i++)
    {
        cout<< ptick[i].cve <<endl;
		cout<< ptick[i].android_id <<endl;
        for(int j = 0; j < ptick[i].patchfiles.size(); j++)
        {
            cout<< ptick[i].patchfiles[j].project <<endl;
            for(auto it = ptick[i].patchfiles[j].pfiles.begin();  it != ptick[i].patchfiles[j].pfiles.end(); it++)
            {
                cout<<"["<<it->first<<"] "<<it->second<<endl;
            }
        }
		cout<<endl;
    }

}

static string get_project(const string& patchfiles)
{
	int pos = patchfiles.find("platform");
	int n   = patchfiles.find("/0");	
	if(pos != -1 && n != -1)
		return string().append(patchfiles, pos, n-pos);
	
	cout<<"get project data error!"<<endl;	
	exit(-1);		
}


static int get_key(string& files)
{
	int pos = files.find("/0");
	if(pos == -1)
	{
		cout<<"[key][patchfile] value get error!"<<endl;
		exit(-1);
	}

	return atoi(string(files, pos+1, 4).c_str());
}



static int findpIndex(vector<patch_files>& pf, string& pro)
{
	for(int i = 0; i < pf.size(); i++)
	{
		if(pf[i].project == pro)
		{
			return i;
		}
	}


	return -1;
}

static  vector<patch_files> get_patchfiles(const string& patchfiles, string version)
{
	vector<patch_files> pf;

	string keyword = "patches/android-" + version;	
	
	int pos1 = 0;
	int pos2 = 0;	


	int index = 0;
	

	while((pos1 = patchfiles.find(keyword, pos1)) != -1)
	{
		patch_files fs;	
		
		pos2 = patchfiles.find('|', pos1);
		if(pos2 != -1)
		{
			string files = string().append(patchfiles, pos1, pos2-pos1);
			fs.project = get_project(files);
			int key = get_key(files);			
		
			pos1 = pos2;
			
			if((index = findpIndex(pf, fs.project)) != -1)
			{
				pf[index].pfiles.insert(pair<int,string>(key, files));
			}
			else
			{
				fs.pfiles.insert(pair<int,string>(key, files));
				pf.push_back(fs);	
			}	
		}
		else
		{
			if((pos2 = patchfiles.find('"', pos1)) == -1)
				pos2 = patchfiles.size();					

			string files =string().append(patchfiles, pos1, pos2-pos1);
		
			fs.project = get_project(files);
			if((index = findpIndex(pf, fs.project)) != -1)
			{
				pf[index].pfiles.insert(pair<int,string>(get_key(files), files));
			}
			else
			{
				fs.pfiles.insert(pair<int,string>(get_key(files), files));
				pf.push_back(fs);
			}
			break;
		}
					
	}
	

	return pf;	
}


void SpManager::sorticket()
{
	cout<< endl << "实际作业信息 : " <<endl;

	int index = 1;
	int flag = 1;

	for(auto& pr : prolist)
	{
		if(flag)
		{
			flag = 0;
			for(auto it = tks.begin(); it != tks.end(); it++)		
			//for(auto& tk : tks)
			{
				//cout<<tk.project<<"  "<<pr<<endl;
				if(it->project == pr)
				{
					if(it->pnum == index)
					{		
						it->isinque = 1;
						sortick.push_back(*it);
						cout<< it->cve << " " << it->patchfile <<endl;
						flag = 1;
						index++;
						it = --tks.begin();
									
					}	
				}
			}
		}
		index = 1;
		flag = 1;		
	}

	cout<< endl <<"重复票：" <<endl;

	for(auto& tik : tks)
	{
		if(tik.isinque == 0)
		{
			cout<< tik.cve <<endl;
			cout<< tik.android_id <<endl;
			cout<< tik.patchfile <<endl;
			cout<<endl;
		}
	}	

	cout<<endl;
}


char* getDate(char *date)
{

	time_t t;
	time(&t);
	struct tm* timeinfo;

	timeinfo = localtime(&t);

	if(timeinfo->tm_mon < 10)
	  sprintf(date, "%d0%d", timeinfo->tm_year+1900,timeinfo->tm_mon+1);
	else
	  sprintf(date, "%d%d", timeinfo->tm_year+1900,timeinfo->tm_mon+1);

	return date;
}

SpManager::SpManager(string version, string date)
{
	string spfile;	
	//加载数据
	if(date[0])
	{
		spfile = "inputfile/" + date + "/machine_readable.csv";
		if((access(spfile.c_str(), F_OK)) == -1)   
   		{   
   	    	fprintf(stderr, "file: %s not exist!\n", spfile.c_str());
			exit(-1);   
    	}
	}   
	else
	{
		char _date[32] = {0};
		spfile = "inputfile/" + string(getDate(_date)) + "/machine_readable.csv";
		if((access(spfile.c_str(), F_OK)) == -1)   
   		{   
   	    	fprintf(stderr, "file: %s not exist!\n", spfile.c_str());
			exit(-1);   
    	}
	}


	Csvloader csv(spfile);	

	//获取表头	
	auto head = csv.getHead();
	int row = csv.getRow();
	int clu = csv.getClu();
		
	//加载case
	for(int i = 1; i < row; i++)
	{
		if((head[i][csv.get_index("area")] == string("Platform")) && (head[i][csv.get_index("patch_files")].find(version) != -1))
		{
			Case pt;
			pt.cve = head[i][csv.get_index("CVE")];
			pt.version = version;
			pt.patch_level = head[i][csv.get_index("patch_level")];
			pt.android_id = head[i][csv.get_index("android_id")];
			
			pt.patchfiles = get_patchfiles(head[i][csv.get_index("patch_files")], version); 			
			
			ptick.push_back(pt);
		}	
	}

	int id = 0;	
	//初始化作业票
	for(int i = 0; i < ptick.size(); i++)
	{
		//cout<<ptick[i].cve<<endl;
		for(int j = 0; j < ptick[i].patchfiles.size(); j++)
		{
			//cout<<ptick[i].patchfiles[j].project<<endl;
			for(auto it = ptick[i].patchfiles[j].pfiles.begin();  it != ptick[i].patchfiles[j].pfiles.end(); it++)
			{
				//cout<<it->first<<" "<<it->second<<endl;
				ticket t;
				t.tid = ++id;
				t.cve = ptick[i].cve;
				t.version =	ptick[i].version;
				t.patch_level = ptick[i].patch_level;
				t.android_id = ptick[i].android_id;
				t.project =	ptick[i].patchfiles[j].project;
				t.pnum = it->first;	
				t.patchfile = it->second;
				t.isinque = 0;
				if(find(prolist.begin(), prolist.end(), t.project) == prolist.end())
				{
					prolist.push_back(t.project);
				}
				tks.push_back(t);
			}
		}
	}		
	
	//过滤排序作业票
	sorticket();
}











