#include "application.hpp"
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <linux/input.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

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
	cout << "seach keyboard" << endl;
	try
	{
		keyboard =  getKeyboardPath();
	}
	catch(std::runtime_error ex)
	{
		cerr << ex.what();
		return -1;
	}
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
	bool keys[KEY_MAX + 1] = {false};
	
	while(bytes > 0)
	{
		if(ev.type == EV_KEY)
		{
			keys[ev.code] = (ev.value != 0);
			if(keys[KEY_LEFTCTRL] or keys[KEY_RIGHTCTRL])
			{
				cout << "ctrl" << endl;
			}
			else if(keys[KEY_LEFTALT] or keys[KEY_RIGHTALT])
			{
				cout << "alt" << endl;
			}
			else
			{
				char c = ev.code;
				cout << ev.code<< "=" << c << endl;
				if(c == 'q')
				{
					close();
				}
			}
		}
		bytes = read(fd,&ev,sizeof(ev));
	}
	
}

void Application::init()
{
	system("clear");
	cout << "start aplication" << endl;
	system("stty -icanon");
	system("setterm -cursor off");
	running = true;
	tcgetattr(STDIN_FILENO,&tio_orig);

	tio_atual = tio_orig;
	
	tio_atual.c_lflag &= ~(ICANON | ISIG | ECHO);
	tio_atual.c_lflag |= CS8;
	tio_atual.c_cc[VMIN] = 1;
	tio_atual.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO,TCSANOW,&tio_atual);
	fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);
		
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
	tcsetattr(STDIN_FILENO,TCSANOW,&tio_orig);
	fcntl(STDIN_FILENO,F_SETFL,0);	
}

void signalhandle(int signal)
{
	cout << "Signal" << signal << "received" << endl;
}
