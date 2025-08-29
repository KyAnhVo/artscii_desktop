#include "artscii_processor.h"
#include "img_manager.h"

#include <iostream>
#include <math.h>

uint8_t Artscii_Processor::bitmap_created = 0;
uint8_t Artscii_Processor::char_bitmap[128][64];

void copy_bitmap(uint8_t * arr, uint8_t * original) {
    for (uint8_t i = 0; i < 64; i++) {
        arr[i] = original[i];
    }
}

void Artscii_Processor::gen_char_bitmap() {
    // SPACE
    uint8_t * temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap[' '], temp);
    delete[] temp;

    // PERIOD
    temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap['.'], temp);
    delete[] temp;

    // HYPHEN
    temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap['-'], temp);
    delete[] temp;

    // EQUAL
    temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap['='], temp);
    delete[] temp;

    // PLUS
    temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap['+'], temp);
    delete[] temp;

    // STAR
    temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 1, 0, 1, 0, 1, 0, 0,
        0, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 0, 1, 0, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap['*'], temp);
    delete[] temp;
    
    // POUND
    temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 0, 0,
        0, 0, 1, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap['#'], temp);
    delete[] temp;

    // AT
    temp = new uint8_t[64] {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    copy_bitmap(Artscii_Processor::char_bitmap['@'], temp);
    delete[] temp;

    // Set flag
    Artscii_Processor::bitmap_created = 1;
}

Artscii_Processor::Artscii_Processor(uint32_t height, uint32_t width) {
    this->height    = height;
    this->width     = width;
    uint64_t area   = static_cast<uint64_t>(height) * static_cast<uint64_t>(width); 
    this->luminance = new float[area];
}

Artscii_Processor::~Artscii_Processor() {
    delete[] this->luminance;
}

void Artscii_Processor::gamma_correction(float gamma) {
    if (gamma < 0.5 || gamma > 3) {
        std::cerr << "gamma range [0.5, 3], reset to 1" << std::endl;
        return;
    }
    for (uint64_t i = 0; i < this->width * this->height; i++) {
        this->luminance[i] = std::pow(this->luminance[i], 1 / gamma);
    }
}

char * Artscii_Processor::gen_artscii() {
    char * artscii = new char[this->width * this->height];
    for (uint64_t i = 0; i < this->width * this->height; i++) {
        uint8_t brightness_level = static_cast<uint8_t>(this->luminance[i] * 7.999f);
        artscii[i] = Artscii_Processor::artscii_chars[brightness_level];
    }
    return artscii;
}

Img_Manager * Artscii_Processor::gen_artscii_bitmap() {
    if (!bitmap_created) Artscii_Processor::gen_char_bitmap();

    char * artscii_arr          = this->gen_artscii();
    Img_Manager * artscii_img   = new Img_Manager(this->height * 8, this->width * 8);
    
    for (uint32_t row = 0; row < this->height; row++) {
        for (uint32_t col = 0; col < this->width; col++) {
            
            // Start handle current character

            uint64_t current_pixel      = row * this->width + col;
            uint32_t starting_row       = row * 8, 
                     starting_col       = col * 8;
            uint8_t * curr_char_bitmap  = Artscii_Processor::char_bitmap[artscii_arr[current_pixel]];

            for (uint32_t i = 0; i < 8; i++) {
                uint64_t row_starting_pixel = 
                    static_cast<uint64_t>(starting_row + i) * static_cast<uint64_t>(artscii_img->width) + static_cast<uint64_t>(starting_col);
                for (uint32_t j = 0; j < 8; j++) {

                    // handle current pixel

                    uint64_t current_pixel  = row_starting_pixel + j,
                             color_index    = current_pixel * 4,
                             char_pixel     = curr_char_bitmap[i * 8 + j];
                    uint8_t  color          = char_pixel ? 255 : 0;

                    // r, g, b, a = color, color, color, 255 since (color, color, color)
                    // is either 0xffffff (white) or 0x000000 (black) and finally we do
                    // not need any transparency hence hardcoded 255.

                    artscii_img->rgba[color_index + 0] = color;
                    artscii_img->rgba[color_index + 1] = color;
                    artscii_img->rgba[color_index + 2] = color;
                    artscii_img->rgba[color_index + 3] = 255;
                }
            }

            // End handle current character

        }
    }

    delete[] artscii_arr;
    return artscii_img;
}


