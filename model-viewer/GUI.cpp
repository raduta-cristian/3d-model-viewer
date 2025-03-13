#include "GUI.h"

void GUI::buildUI() {


	selectedFileLabel->setAlignment(Qt::AlignCenter);
	selectedFileLabel->setMinimumWidth(250);
	selectedFileLabel->setStyleSheet("background: lightgreen; color : black");


	QLabel* title = new QLabel("3D Model Viewer");
	title->setStyleSheet("color: lightgreen; font-size: 40px;");
	QLabel* info = new QLabel("INFO");
	info->setStyleSheet("color: lightgreen; font-size: 15px;");
	QLabel* information = new QLabel(" > Select the model file that you want to view\n > The model should be in .obj format, with its textures stored in the same directory\n");
	QLabel* ctrls = new QLabel("CONTROLS");
	ctrls->setStyleSheet("color: lightgreen; font-size: 15px;");
	QLabel* controls = new QLabel(" > WASD - move forward/left/backward/right\n > QE - move down/up\n > Render modes (press number to activate):\n\t1 - Phong lighting\n\t2 - Lit\n\t3 - Wireframe\n > Mouse - look around\n > Scroll wheel: zoom in/out");


	QVBoxLayout* vbox = new QVBoxLayout();

	vbox->addSpacing(10);
	vbox->addWidget(title);
	vbox->addWidget(selectedFileLabel);
	vbox->addSpacing(10);
	vbox->addWidget(selectBtn);
	vbox->addWidget(viewBtn);
	vbox->addSpacing(10);


	vbox->addWidget(info);
	vbox->addWidget(information);
	vbox->addWidget(ctrls);
	vbox->addWidget(controls);
	vbox->addSpacing(10);

	this->setLayout(vbox);
}

QString GUI::getFileName(string path) const {
	std::stringstream ss(path);

	string t;

	char del = '/';
	string last;

	while (std::getline(ss, t, del))
		last = t;

	return QString::fromStdString(last);
}


void GUI::connectUI() {
	QObject::connect(selectBtn, &QPushButton::pressed, [this]() {
		filename = QFileDialog::getOpenFileName(this,
			tr("Open Model"), ".", tr("Model Files(*.obj *.fbx)"));
		selectedFileLabel->setText(getFileName(filename.toStdString()));
		
		}
	);

	QObject::connect(viewBtn, &QPushButton::pressed, [this]() {
		if (filename.size() == 0)
			return;

		toClose = false;
		close();
		});
}