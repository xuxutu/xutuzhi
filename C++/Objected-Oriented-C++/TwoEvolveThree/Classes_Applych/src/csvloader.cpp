#include "csvloader.h"

int Csvloader::get_index(string value)
{
	for(int i = 0; i < column; i++)
	{
		if(msv[0][i] == value)
			return i;
	}

	cout<<"this value "<<value<<" is not find!"<<endl;
	exit(-1);
}


static int countc(char* line)
{
	if(strchr(line, '|'))
	{
		puts("this program can't read this table since the identifier '|' exists in this row.");
		exit(EXIT_FAILURE);
	}

	char* p1 = line;
	char* p2;
	char* p3;
	char* p4 = line;
	int n = 0;
	
	while(p1 = strchr(p1, '"'))
	{
		p3 = p1;
		p2 = ++p1;
		if(p2 = strchr(p2, '"'))
		{	
			while(((p3 = strchr(p3, ',')) > p1) && (p3 < p2))	
			{
				*p3 = '|';
			}

			p1 = ++p2;				
		}
		else
		{		
			puts("this csv table don't following the standard.");		
			exit(EXIT_FAILURE);	
		}
	}	


	//count comma
	while(p4 = strchr(p4, ','))	
	{
		n++;
		p4++;
	}

	return n;
}


long getFileSize(string filename) 
{
	struct stat info;
	if(stat(filename.c_str(), &info) < 0)
	{
		perror("stat error");
		exit(-1);
	}
	
	//printf("file size: %ld\n", info.st_size);		

	return info.st_size;
}

Csvloader::Csvloader(string filename) : m_filename(filename)
{
	//open file

	FILE* pf = fopen(m_filename.c_str(), "r");
	if(pf == 0)
	{
		perror("open file error");
		exit(EXIT_FAILURE);
	}
	
	char* line = NULL;
	size_t len = 0;
	ssize_t nread;

	//get file size
	long size = getFileSize(filename);

	//allocate space
	char* file = (char*)malloc(sizeof(char) * size);
	if(file == NULL)
	{
		perror("allocate error");
	}	

	memset(file, 0, size);

	int count = 0;	
	int tmp = 0;
	int sum = 0;
	while((nread = getline(&line, &len, pf)) != -1)
	{
		//*strchr(line, '\n') = 0;
		count++;
		if(count == 1)
		{
			//Assume:in standard csv table,the first line decide its clumns
			//count','
			tmp = countc(line);
		}

		strcat(file, line);
	}

	//printf("file size = %ld\n",strlen(file));
	sum = countc(file);


	if(sum % tmp != 0)
	{
		puts("this csv table dont follow the standard.");
		exit(EXIT_FAILURE);
	}	

	//load data
	char* p = file;
	int n = 0;
	int x = 0;
	char* p1 = p;

	for(int i = 0; i < sum / tmp; i++)
	{
		vector<string> v;
		while(p = strchr(p, ','))
		{
			*p = 0;
			v.push_back(string(p1));
			n++;
			p1 = ++p;
			if(n == tmp)	
			{
				p = strchr(p, '\n');
				*p = 0;
				v.push_back(string(p1));	
				msv.push_back(v);
				n = 0;
				p1 = ++p;
				break;
			}
		}
	}

	cout<<"Data loading over! "<<"row = "<<msv.size()<<" "<<"column = "<<msv[0].size()<<endl;
	row = msv.size();	
	column = msv[0].size();
	//close file
	free(line);
	free(file);
	fclose(pf);	
}
