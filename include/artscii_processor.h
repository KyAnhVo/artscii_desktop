#ifndef ARTSCII_PROCESSOR_H
#define ARTSCII_PROCESSOR_H

#include <stdint.h>

class Img_Manager;

/**
 * @brief a luminance view of an Img_Manager, containing
 * luminance of the origianl image.
 */
class Artscii_Processor {
private:
    static uint8_t  char_bitmap[128][64];
    static uint8_t  bitmap_created;
    static void     gen_char_bitmap();
public:
    static constexpr char artscii_chars[] = " .-=+*#@";
    uint32_t height, width;
    float * luminance;

    /**
     * @brief create Artscii_Processor object that has initialized empty luminance array
     *
     * @param height height of luminance image
     * @param width width of luminance image
     */
    Artscii_Processor(uint32_t, uint32_t);
    ~Artscii_Processor();

    /**
     * gamma-correct the array, essentially "adjust the brightness".
     * gamma = 1 is normal, gamma < 1 is dark, gamma > 1 is bright.
     * limit gamma from 0.5 to 3.
     *
     * @param gamma brightness factor
     */
    void gamma_correction(float);

    /**
     * create a char array size width * height where each character is
     * its artscii representation of the luminance view.
     * 
     * @return char array of the char array
     *
     * @note reminder to delete the char arr after using.
     */
    char * gen_artscii();
    
    /**
     * @brief generate pixelized artscii image in the form of an Img_Manager *
     *
     * Convert the artscii processor's luminance values, convert each luminance
     * value into a 8x8 square char.
     *
     * @return pointer to the created Img_Manager.
     */
    Img_Manager * gen_artscii_bitmap();
};

#endif
