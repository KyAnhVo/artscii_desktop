#include "main_window.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <qboxlayout.h>

Main_Window::Main_Window(QWidget * parent) : QMainWindow(parent) {
    this->setup_main_widgets();
    this->setup_layout();
};

Main_Window::~Main_Window() {}

// Slots
void Main_Window::on_open_file_button_clicked() {}
void Main_Window::on_preview_button_clicked() {}
void Main_Window::on_save_button_clicked() {}
void Main_Window::on_brightness_slider_changed() {}
void Main_Window::on_zoom_slider_changed() {}

void Main_Window::setup_main_widgets() {
    // open file button
    this->open_file_button = new QPushButton(tr("Open file"));
    this->connect(
            this->open_file_button, &QPushButton::clicked,
            this, &Main_Window::on_open_file_button_clicked
            );

    // preview button
    this->preview_button = new QPushButton(tr("Preview Image"));
    this->connect(
            this->preview_button, &QPushButton::clicked,
            this, &Main_Window::on_preview_button_clicked
            );

    // save button
    this->save_button = new QPushButton(tr("Save image"));
    this->connect(
            this->save_button, &QPushButton::clicked,
            this, &Main_Window::on_save_button_clicked
            );

    // zoom slider
    this->zoom_slider = new QSlider(Qt::Horizontal);
    this->zoom_slider->setRange(30, 200);
    this->zoom_slider->setValue(100);
    this->zoom_slider->setTickInterval(5);
    this->zoom_slider->setTickPosition(QSlider::NoTicks);
    this->connect(
            this->zoom_slider, &QSlider::valueChanged,
            this, &Main_Window::on_zoom_slider_changed
            );

    // brightness slider
    this->brightness_slider = new QSlider(Qt::Horizontal);
    this->connect(
            this->brightness_slider, &QSlider::valueChanged,
            this, &Main_Window::on_brightness_slider_changed
            );

    // preview text box
    this->preview_text_edit = new QPlainTextEdit();
    QFont font("Monospace", 1);
    font.setStyleHint(QFont::Monospace);
    this->preview_text_edit->setFont(font);

    // info labels and text boxes
    this->zoom_label        = new QLabel("Zoom      ");
    this->brightness_label  = new QLabel("Brightness");
    this->link_box          = new QLineEdit();
}

void Main_Window::setup_layout() {
    // Link HBox
    QHBoxLayout * link_hbox = new QHBoxLayout();
    link_hbox->addWidget(this->link_box, 1);
    link_hbox->addWidget(this->open_file_button);

    // Brightness HBox
    QHBoxLayout * brightness_hbox = new QHBoxLayout();
    brightness_hbox->addWidget(this->brightness_label);
    brightness_hbox->addWidget(this->brightness_slider, 1);

    // Zoom HBox
    QHBoxLayout * zoom_hbox = new QHBoxLayout();
    zoom_hbox->addWidget(this->zoom_label);
    zoom_hbox->addWidget(this->zoom_slider, 1);

    // Sliders VBox
    QVBoxLayout * sliders_vbox = new QVBoxLayout();
    sliders_vbox->addLayout(zoom_hbox);
    sliders_vbox->addLayout(brightness_hbox);

    // Buttons VBox
    QVBoxLayout * buttons_vbox = new QVBoxLayout();
    buttons_vbox->addWidget(this->preview_button);
    buttons_vbox->addWidget(this->save_button);

    // Image altering layout
    QHBoxLayout * img_options_hbox = new QHBoxLayout();
    img_options_hbox->addLayout(sliders_vbox, 1);
    img_options_hbox->addLayout(buttons_vbox);

    // Main VBox
    QVBoxLayout * main_vbox = new QVBoxLayout();
    main_vbox->addLayout(link_hbox);
    main_vbox->addWidget(this->preview_text_edit, 1);
    main_vbox->addLayout(img_options_hbox);

    // add to this
    QWidget * central = new QWidget();
    central->setLayout(main_vbox);
    this->setCentralWidget(central);
}
