#include <iostream>

#include <QCoreApplication>

#include "AutoCounter.h"
#include "Log.h"
#include "myclass.hpp"
#include "ProjectConfig.h"

using namespace std;

int main(int argc, char** argv)
{
	QCoreApplication app{argc, argv};

	logr::init();

	cout << "Hello World!" << " Major: " << PROJECT_VERSION_MAJOR << endl;

	MyClass myclass{};

	return 0;
}
