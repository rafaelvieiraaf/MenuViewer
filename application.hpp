#ifndef APPLICATION_H
#define APPLICATION_H
#include <vector>
#include <string>
#include <termios.h>

class Application
{
	std::string keyboard;
	termios tio_orig,tio_atual;
	bool running;
	std::vector<std::string> args;
	public:
		Application(int argc, char * argv[]);
		int run();
		virtual ~Application();
	private:
		void keyhandle();
		void init();
		void finish();
		void close();
	
};
std::string getEventType(int evt);
std::string getKeyboardPath();
void signalhandle(int signal);

#endif //APPLICATION_H
