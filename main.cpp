#include "application.hpp"
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	Application app(argc,argv);
	return app.run();
}
