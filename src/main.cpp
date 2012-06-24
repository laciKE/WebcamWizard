#include <QApplication>
 
#include "view.hpp"
 
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
        View *view = new View();

        view->show();
	return app.exec();
}
