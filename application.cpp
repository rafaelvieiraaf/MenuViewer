#include "application.hpp"
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fstream>

using namespace std;

Application::Application(int argc , char * argv[])
{
	init();
	for( int i = 0 ; i < argc ; ++i)
	{
		args.push_back(argv[i]);
	}
}

Application::~Application()
{
	finish();
}
int Application::run()
{
	while(running)
	{
		try
		{	
			keyhandle();	
		}
		catch(std::runtime_error ex)
		{
			cerr << ex.what();
			return -1;
		}
	}
	return 0;
}
void Application::close()
{
	running = false;
}
void Application::keyhandle()
{
	input_event ev;
	int fd = open(keyboard.c_str(),O_RDONLY);

	if(fd == -1)
	{
		string error =  "Cannot open " +  keyboard + ": ";
		error += strerror(errno);
		error +=  "\n";
		throw std::runtime_error(error);
	}
	
	int bytes = read(fd,&ev,sizeof(ev));
	
	while(bytes > 0)
	{
		if(ev.type == EV_KEY)
		{
			keys[ev.code] = (ev.value != 0);
			if(ev.value == 1 and keymap.contains(ev.code))
			{
				char c = keymap[ev.code];
				cout << c;
				cout.flush();
				if((keys[KEY_LEFTCTRL] or keys[KEY_RIGHTCTRL] ) and c == 'q' or c == 'Q' )
				{
					close();
				}
			}
			else if(ev.value == 1 and !keymap.contains(ev.code))
			{
				cout << ev.code << endl;
			}
		}
		else if(ev.type == EV_SYN)
		{
			return;
		}
		bytes = read(fd,&ev,sizeof(ev));
	}
	
}

bool Application::init()
{
	try
	{
		keyboard =  getKeyboardPath();
	}
	catch(std::runtime_error ex)
	{
		cerr << ex.what();
		return false;
	}

	cout << "start aplication" << endl;
	system("stty -icanon");
	system("setterm -cursor off");
	running = true;
	tcgetattr(STDIN_FILENO,&tio_orig);

	tio_atual = tio_orig;
	
	tio_atual.c_lflag &= ~(ICANON | ISIG | ECHO | ECHOE);
	tio_atual.c_lflag |= CS8;
	tio_atual.c_cc[VMIN] = 1;
	tio_atual.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO,TCSANOW,&tio_atual);
	fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);

	ifstream keymap_file("keymap.txt");
	string line;
	if(keymap_file.is_open())
	{
		while(getline(keymap_file,line))
		{
			solveKeyEntry(line,keymap);
		}
		keymap_file.close();
	}
	return true;
}

string getKeyboardPath()
{
	string input;
	filesystem::path input_path{"/dev/input/by-path/"};
	for(auto const& it : filesystem::directory_iterator{input_path})
	{
		input = it.path();
		if(input.contains("kbd"))
			return input;
	}
	throw std::runtime_error("keyboard not found");
}

string getEventType(int evt)
{
	switch(evt)
	{
	    case EV_SYN:
		{
			return "EV_SYN";
		}
	    case EV_KEY:
		{
			return "EV_KEY";
		}
	    case EV_REL:
		{
			return "EV_REL";
		}
	    case EV_ABS:
		{
			return "EV_ABS";
		}
	    case EV_MSC:
		{
			return "EV_MSC";
		}
	    case EV_SW:
		{
			return "EV_SW";
		}
	    case EV_LED:
		{
			return "EV_LED";
		}
	    case EV_SND:
		{
			return "EV_SND";
		}
	    case EV_REP:
		{
			return "EV_REP";
		}
	    case EV_FF:
		{
			return "EV_FF";
		}
	    case EV_PWR:
	    {
	    	return "EV_PWR";
	    }    
	    case EV_FF_STATUS:
		{
			return "EV_FF_STATUS";
		}
		default:
			return "EVENT UNKNOW";
		}
		return "EVENT UNKNOW";
	
}

void Application::finish()
{
	cout << endl << "finish application" << endl;
	system("setterm -cursor on");
	system("clear");
	tcsetattr(STDIN_FILENO,TCSANOW,&tio_orig);
	fcntl(STDIN_FILENO,F_SETFL,0);
	tcflush(STDIN_FILENO,TCIFLUSH);
}

void signalhandle(int signal)
{
	cout << "Signal" << signal << "received" << endl;
}

void solveKeyEntry(const string& line,map<int,char>& keymap)
{
	int i = 0;
	string code;
	string key;
	for( i = 0 ; i < line.size() and line[i] != ' ' ; ++i)
	{
		code += line[i];
	}

	i++;	

	for( ; i < line.size() ; ++i)
	{
		key += line[i];
	}
	keymap[std::stoi(code)] = std::stoi(key);
}
