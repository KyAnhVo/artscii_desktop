#include "img_manager.h"
#include "artscii_processor.h"

#include <cstdint>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#define HEIGHT_DOWNSIZE 2
#define WIDTH_DOWNSIZE  1

Img_Manager::Img_Manager(uint32_t height, uint32_t width) {
    this->height    = height;
    this->width     = width;
    uint64_t area   = static_cast<uint64_t>(height) * static_cast<uint64_t>(width);
    this->rgba      = new uint8_t[area * 4]; // because r, g, b, a is 4 vals.
}

Img_Manager::Img_Manager(const Img_Manager& other) {
    this->width = other.width;
    this->height = other.height;
    uint64_t area =
        static_cast<uint64_t>(this->width) * static_cast<uint64_t>(this->height) * 4;
    for (uint64_t i = 0; i < area; i++)
        this->rgba[i] = other.rgba[i];
}

Img_Manager::~Img_Manager() {
    delete[] this->rgba;
}

void Img_Manager::downsize(float height_downsize_ratio, float width_downsize_ratio) {
    // each pixel of the downsized image is map to its
    // height_downsize_ratio x width_downsize_ratio 
    // corresponding square.

    uint32_t downsized_width    = static_cast<uint32_t>(this->width / width_downsize_ratio),
             downsized_height   = static_cast<uint32_t>(this->height / height_downsize_ratio);
    uint8_t * downsized         = new uint8_t[downsized_width * downsized_height * 4];
    
    for (uint32_t downsized_row = 0; downsized_row < downsized_height; downsized_row++) {
        for (uint32_t downsized_col = 0; downsized_col < downsized_width; downsized_col++) {

            uint64_t downsized_ind = (downsized_row * downsized_width + downsized_col) * 4;

            uint32_t starting_row   = static_cast<uint32_t>(downsized_row * height_downsize_ratio),
                     starting_col   = static_cast<uint32_t>(downsized_col * width_downsize_ratio),
                     ending_row     = static_cast<uint32_t>((downsized_row + 1) * height_downsize_ratio),
                     ending_col     = static_cast<uint32_t>((downsized_col + 1) * width_downsize_ratio);

            uint64_t r_tot = 0, 
                     g_tot = 0, 
                     b_tot = 0, 
                     a_tot = 0;

            uint8_t max_a = 0;

            for (uint32_t row = starting_row; row < ending_row; row++) {
                for (uint32_t col = starting_col; col < ending_col; col++) {
                    uint64_t original_ind = (row * this->width + col) * 4;
                    uint8_t curr_a = this->rgba[original_ind + 3];
                    
                    r_tot += curr_a * this->rgba[original_ind + 0];
                    g_tot += curr_a * this->rgba[original_ind + 1];
                    b_tot += curr_a * this->rgba[original_ind + 2];
                    a_tot += curr_a;

                    max_a = max_a < curr_a ? curr_a : max_a;
                }
            }
            
            if (max_a > 0) {
                downsized[downsized_ind + 0] = static_cast<uint8_t>(r_tot / a_tot);
                downsized[downsized_ind + 1] = static_cast<uint8_t>(g_tot / a_tot);
                downsized[downsized_ind + 2] = static_cast<uint8_t>(b_tot / a_tot);
            }
            else {
                downsized[downsized_ind + 0] = 0;
                downsized[downsized_ind + 1] = 0;
                downsized[downsized_ind + 2] = 0;
            }

            downsized[downsized_ind + 3] = max_a;
        }
    }

    // change pointer
    delete[] this->rgba;
    this->rgba = downsized;

    // change sizes
    this->height    = downsized_height;
    this->width     = downsized_width;
}

Artscii_Processor * Img_Manager::luminance(uint8_t r, uint8_t g, uint8_t b) {
    Artscii_Processor * luminance = new Artscii_Processor(this->height, this->width);

    for (uint32_t i = 0; i < this->height * this->width * 4; i += 4) {
        float alpha = this->rgba[i + 3] / 255.0;
        float not_alpha = 1 - alpha;

        float pixel_luminance = 
            (0.2126f * rgba[i] + 0.7152f * rgba[i + 1] + 0.0722f * rgba[i + 2]) / 255;
        float background_luminance =
            (0.2126f * r + 0.7152f * g + 0.0722f * b) / 255;

        luminance->luminance[i / 4] = alpha * pixel_luminance + not_alpha * background_luminance;
    }

    return luminance;
}

void Img_Manager::to_png(char * path) {
    if (!path) {
        path = (char *) "./artscii_img.png";
    }
    
    // setup writing structs

    FILE * output_file = fopen(path, "wb");
    if (!output_file) {
        throw std::runtime_error("cannot create output file");
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(output_file);
        throw std::runtime_error("cannot write png file");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(output_file);
        throw std::runtime_error("cannot write png info ptr");
    }

    // set err handling, connect output_file to image structs

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(output_file);
        throw std::runtime_error("Error in writing image");
    }

    png_init_io(png_ptr, output_file);

    // write IHDR

    png_set_IHDR(png_ptr, info_ptr,          // write structs
            this->width, this->height,      // dimensions 
            8,                              // bit depth
            PNG_COLOR_TYPE_RGBA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    // write image
    
    png_write_info(png_ptr, info_ptr);

    // setup image ptr
    uint8_t ** row_ptrs = new uint8_t*[this->height];
    for (uint32_t i = 0; i < this->height; i++)
        row_ptrs[i] = this->rgba + 4 * i * this->width;
    
    // write to image
    png_write_image(png_ptr, row_ptrs);

    // clean mem

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(output_file);
    delete[] row_ptrs;
} 


