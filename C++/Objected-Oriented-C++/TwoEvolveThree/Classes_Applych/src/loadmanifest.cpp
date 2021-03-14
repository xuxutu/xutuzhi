#include "loadmanifest.h"

string key; 

static void readxml(map<string, string>& pro, int fd)
{
	char* line = NULL;	
	size_t len = 0;
	ssize_t nread;

	FILE* fp = fdopen(fd, "r");
	if(fp < 0)
	{
		perror("fdopen error");
		exit(-1);
	}
	char* p1;
	char* p2;	

	int flag1 = 0;

	while((nread = getline(&line, &len, fp)) != -1)
	{
		if(p1 = strstr(line, key.c_str()))
		{
			string name,path;
			p1 += strlen(key.c_str());
			if(p2 = strchr(p1, '"'))
			{
				name = string(p1, p2);
				flag1 = 1;
				//cout<<name<<endl;
			}
			else
			{
				cout<<"project get failed!"<<endl;
				exit(-1);
			}

			if(flag1)
			{
				if(p1 = strstr(line, "path=\""))
				{
					p1 += strlen("path=\"");

					if(p2 = strchr(p1, '"'))
					{
						path = string(p1, p2);
						//cout<<path<<endl;
						//cout<<line<<endl;
						//cout<<pro.size()<<endl;	
						pro.insert(pair<string, string>(name, path));
					}
					else
					{
						fprintf(stderr, "read xml failed :%s\n", name.c_str());
						exit(-1);
					}	
	
				}
				else
				{
					fprintf(stderr, "read xml failed :%s\n", name.c_str());
					exit(-1);
				}	

			}

		}

	}

	fclose(fp);
	free(line);

}






void loadfest(map<string,string>& pro, vector<string> xml)
{
	int fd;
	
	if(xml.size() == 1)
		key = "project name=\"";
	else
		key = "name=\"";

	for(auto& file : xml)
	{
		//open file
		fd = open(file.c_str(), O_RDONLY);
		if(fd < 0)
		{
			fprintf(stderr, "open error : %s, file:%s ",strerror(errno), file.c_str());
			exit(-1);
		}

		cout<<file.c_str()<<endl;
		readxml(pro, fd);

		close(fd);
	}

	cout<<"load fest over! project size = "<<pro.size()<<endl;
}
