#ifndef __STRING__ 
#define __STRING__


class String
{
public:
	String(const char* cstr=0);
	String(const String& str);//必须重新实现拷贝构造函数
    String& operator=(const String& str);//必须重新实现赋值操作符
    ~String();

    char* get_c_str()const{return m_data;}


private:
	char* m_data;

};












#endif
