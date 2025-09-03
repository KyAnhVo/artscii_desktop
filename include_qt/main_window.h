#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "img_manager.h"
#include "artscii_processor.h"

class Main_Window : public QMainWindow {
    Q_OBJECT

public:
    Main_Window(QWidget * parent=nullptr);
    ~Main_Window();

public slots:
    // buttons clicked slots
    void on_open_file_button_clicked();
    void on_preview_button_clicked();
    void on_save_button_clicked(); 

    // sliders adjusted slots
    void on_zoom_slider_changed();
    void on_brightness_slider_changed();

private:
    char current_file[100];
    Img_Manager * original_image, * preview_image, * saved_image;
    int font_width, font_height;
    QString current_path;

    // buttons
    QPushButton * open_file_button, 
                * preview_button, 
                * save_button;
    // sliders
    QSlider * zoom_slider,
            * brightness_slider;
    // main preview section
    QPlainTextEdit * preview_text_edit;

    // labels, info text boxes, etc.
    QLabel * zoom_label,
           * brightness_label,
           * zoom_val_label,
           * brightness_val_label;
    QLineEdit * link_box;

    // util functions
    void setup_layout();
    void setup_main_widgets();
};

#endif
