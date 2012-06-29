#include <QApplication>

#include "view.hpp"
#include "model.hpp"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	
	Model *model = new Model();
	View *view = new View(model);
	
	model->registerView(view);
	model->Init();

	view->show();
	int ret = app.exec();

	delete view;

	return ret;
}
