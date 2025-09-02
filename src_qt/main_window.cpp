#include "main_window.h"
#include "artscii_processor.h"
#include "img_manager.h"
#include "png_decoder.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QBoxLayout>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <stdlib.h>
#include <iostream>
#include <string>

#define BRIGHTNESS_MIN      30
#define BRIGHTNESS_MAX      200
#define BRIGHTNESS_DEFAULT  100
#define BRIGHTNESS_TICK     5

#define ZOOM_MIN            500
#define ZOOM_MAX            10000
#define ZOOM_DEFAULT        1000
#define ZOOM_TICK           250

QString to_png(QString);

Main_Window::Main_Window(QWidget * parent) : QMainWindow(parent) {
    this->original_image    = nullptr;
    this->preview_image     = nullptr;
    this->saved_image       = nullptr;

    this->setup_main_widgets();
    this->setup_layout();
};

Main_Window::~Main_Window() {}

/**
 * Slot functions
 */

void Main_Window::on_open_file_button_clicked() {
    QString file_name = QFileDialog::getOpenFileName(
            this, 
            "Choose image", 
            QDir::currentPath(),
            "Images (*.png *.jpg *.jpeg)"
            );
    if (file_name.length() == 0) {
        return;
    }

    file_name = to_png(file_name);
    if (file_name.length() == 0) {
        std::cerr << "Can't see file" << std::endl;
        return;
    }

    if (this->original_image != nullptr) {
        delete this->original_image;
        this->original_image = nullptr;
    }
    if (this->preview_image != nullptr) {
        delete this->preview_image;
        this->preview_image = nullptr;
    }
    if (this->saved_image != nullptr) {
        delete this->saved_image;
        this->saved_image = nullptr;
    }
    
    Png_Input * png_input   = new Png_Input(file_name.toStdString());
    this->original_image    = png_input->read_image();

    this->preview_image     = new Img_Manager(original_image);
    this->preview_image->downsize(this->font_height * 2, this->font_width * 2);

    std::cout << "resized" << std::endl;

    Artscii_Processor * artscii = this->preview_image->luminance(0, 0, 0);
    char * artscii_arr = artscii->gen_artscii();
    
    std::cout << "start printing" << std::endl;

    QString print_arr;
    for (uint64_t i = 0; i < artscii->height; i++) {
        for (uint64_t j = 0; j < artscii->width; j++) {
            print_arr.append(QChar(artscii_arr[i * artscii->width + j]));
        }
        print_arr.append("\n");
    }
    std::cout << "done constructing print_arr" << std::endl;
    this->preview_text_edit->clear();
    this->preview_text_edit->appendPlainText(print_arr);

    delete artscii;
    delete artscii_arr;
}
void Main_Window::on_preview_button_clicked() {
    if (this->original_image == nullptr) {
        QMessageBox msg_box;
        msg_box.setText("No image chosen yet.");
        msg_box.exec();
        return;
    }
    if (this->preview_image != nullptr) delete this->preview_image;
    this->preview_image = new Img_Manager(this->original_image);
    
    double height_downsize  = this->font_height * this->zoom_slider->value() * 0.01;
    double width_downsize   = this->font_width * this->zoom_slider->value() * 0.01;
    this->preview_image->downsize(height_downsize, width_downsize);

    Artscii_Processor * artscii = this->preview_image->luminance(0, 0, 0);
    artscii->gamma_correction(this->brightness_slider->value() * 0.01);
    char * artscii_arr = artscii->gen_artscii();
    
    std::cout << "start printing" << std::endl;

    QString print_arr;
    for (uint64_t i = 0; i < artscii->height; i++) {
        for (uint64_t j = 0; j < artscii->width; j++) {
            print_arr.append(QChar(artscii_arr[i * artscii->width + j]));
        }
        print_arr.append("\n");
    }
    std::cout << "done constructing print_arr" << std::endl;
    this->preview_text_edit->clear();
    this->preview_text_edit->appendPlainText(print_arr);

    delete artscii;
    delete artscii_arr;
}
void Main_Window::on_save_button_clicked() {}
void Main_Window::on_brightness_slider_changed() {}
void Main_Window::on_zoom_slider_changed() {}



/**
 * Utility functions
 */

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
    this->zoom_slider->setRange(ZOOM_MIN, ZOOM_MAX);
    this->zoom_slider->setTickInterval(ZOOM_TICK);
    this->zoom_slider->setValue(ZOOM_DEFAULT);
    this->zoom_slider->setTickPosition(QSlider::NoTicks);
    this->connect(
            this->zoom_slider, &QSlider::valueChanged,
            this, &Main_Window::on_zoom_slider_changed
            );

    // brightness slider
    this->brightness_slider = new QSlider(Qt::Horizontal);
    this->brightness_slider->setRange(BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    this->brightness_slider->setTickInterval(BRIGHTNESS_TICK);
    this->brightness_slider->setValue(BRIGHTNESS_DEFAULT);
    this->brightness_slider->setTickPosition(QSlider::NoTicks);
    this->connect(
            this->brightness_slider, &QSlider::valueChanged,
            this, &Main_Window::on_brightness_slider_changed
            );

    // Font preprocessing and check monospacing
    QFont font("Monospace", 5);
    font.setStyleHint(QFont::Monospace);
    QFontMetrics font_metrics(font);
    this->font_height   = font_metrics.height();
    this->font_width    = font_metrics.horizontalAdvance(' ');
    for (int i = 0; i < 8; i++) {
        if (font_metrics.horizontalAdvance(Artscii_Processor::artscii_chars[i]) != this->font_width) {
            std::cerr << "Program cannot identify monospace font for ascii art" << std::endl;
            exit(1);
        }
    }

    // preview text box color pallete
    QPalette p;
    p.setColor(QPalette::Base, Qt::black);   // background
    p.setColor(QPalette::Text, Qt::white);   // text

    // preview text box
    this->preview_text_edit = new QPlainTextEdit();
    this->preview_text_edit->setFont(font);
    this->preview_text_edit->setLineWrapMode(QPlainTextEdit::NoWrap);
    this->preview_text_edit->setReadOnly(true);
    this->preview_text_edit->setPalette(p);

    // info labels and text boxes
    this->zoom_label            = new QLabel("Zoom      ");
    this->brightness_label      = new QLabel("Brightness");
    this->link_box              = new QLineEdit();
    this->zoom_val_label        = new QLabel(QString::fromStdString(std::to_string(this->zoom_slider->value())));
    this->brightness_val_label  = new QLabel(QString::fromStdString(std::to_string(this->brightness_slider->value())));

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
    brightness_hbox->addWidget(this->brightness_val_label);

    // Zoom HBox
    QHBoxLayout * zoom_hbox = new QHBoxLayout();
    zoom_hbox->addWidget(this->zoom_label);
    zoom_hbox->addWidget(this->zoom_slider, 1);
    zoom_hbox->addWidget(this->zoom_val_label);

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

QString to_png(QString original) {
    if (original.endsWith("png"))
        return original;
    std::string ffmpeg_cmd = "ffmpeg -loglevel error -y -i \"";
    ffmpeg_cmd += original.toStdString();
    ffmpeg_cmd += "\" /tmp/artscii_converted_output.png";
    int ffmpeg_status = system(ffmpeg_cmd.c_str());
    if (ffmpeg_status != 0) {
        std::cerr << "conversion to png failed" << std::endl;
        return "";
    }
    return "/tmp/artscii_converted_output";
}
