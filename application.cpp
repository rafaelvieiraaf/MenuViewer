#include "application.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "keysdefine.hpp"
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
		keyhandle();	
	}
	return 1;
}
void Application::close()
{
	running = false;
}
void Application::keyhandle()
{
	char c = getKey(Keys::KEY_NULL);

	read(STDIN_FILENO,&c,1);

	if(c == getKey(Keys::KEY_ESC))
	{
		cout << "escape" << endl;
	}
	else if(c != getKey(Keys::KEY_NULL))
	{
		cout << "key code: "<< static_cast<int>(c) << endl;
	}
	if(c == 'q')
	{
		close();
	}
}

void Application::init()
{
	cout << "start aplication" << endl;
	system("stty -icanon");
	system("setterm -cursor off");
	system("clear");
	running = true;
	tcgetattr(STDIN_FILENO,&tio_orig);

	tio_atual = tio_orig;
	
	tio_atual.c_lflag &= ~(ECHO | ICANON | ECHOE | ISIG | OPOST | CSTOPB | PARENB | CSIZE);
	tio_atual.c_lflag |= CS8;
	tio_atual.c_cc[VMIN] = 1;
	tio_atual.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO,TCSANOW,&tio_atual);
	fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);
		
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
