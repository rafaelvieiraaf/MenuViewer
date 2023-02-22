#ifndef APPLICATION_H
#define APPLICATION_H
#include <vector>
#include <string>
#include <termios.h>
#include <map>
#include <linux/input.h>
class Application
{
	bool keys[KEY_MAX+1] = {false};
	std::map<int,char> keymap;
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
		bool init();
		void finish();
		void close();
	
};
std::string getEventType(int evt);
std::string getKeyboardPath();
void signalhandle(int signal);
void solveKeyEntry(const std::string& line,std::map<int,char>& keymap);

#endif //APPLICATION_H
