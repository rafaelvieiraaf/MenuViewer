#ifndef KEYS_DEFINE_H
#define KEYS_DEFINE_H

enum class Keys
{
	KEY_NULL='\0',
	KEY_ESC='\033'
	
};
char getKey(Keys key)
{
	return static_cast<char>(key);
}
Keys getKey(char key)
{
	return static_cast<Keys>(key);	
}
#endif//KEYS_DEFINE_H
