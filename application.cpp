#include "application.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

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
	char c = '\0';

	read(STDIN_FILENO,&c,1);

	if(c == 'q')
	{
		close();
	}
}

void Application::init()
{
	cout << "start aplication" << endl;
	running = true;
	tcgetattr(STDIN_FILENO,&tio_orig);

	tio_atual = tio_orig;

	tio_atual.c_lflag &= ~ICANON;
	tio_atual.c_cc[VMIN] = 1;
	tio_atual.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO,TCSANOW,&tio_atual);
	fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);
		
}

void Application::finish()
{
	cout << "finish application" << endl;
	tcsetattr(STDIN_FILENO,TCSANOW,&tio_orig);
	fcntl(STDIN_FILENO,F_SETFL,0);	
}

void signalhandle(int signal)
{
	cout << "Signal" << signal << "received" << endl;
}
