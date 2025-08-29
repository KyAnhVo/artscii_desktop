#ifndef IMG_MANAGER_H
#define IMG_MANAGER_H

#include <cstdint>

class Artscii_Processor;

class Img_Manager {
public:
    uint8_t * rgba;
    uint32_t height, width;

    // constructor

    /**
     * @brief create Img_Manager object that has initialized empty rgb arrays
     */
    Img_Manager(uint32_t, uint32_t);

    /**
     * @brief create Img_Manager object that is a deep copy of another Img_Manager object
     */
    Img_Manager(const Img_Manager&);

    /**
     * @brief destruct the current Img_Manager object and its rgba array.
     */
    ~Img_Manager();

    // methods

    /**
     * @brief downsize the image's height by a factor of height_downsize
     * and width by width_downsize
     *
     * @param height_downsize The factor to downsize height
     * @param width_downsize The factor to downsize width
     * @return The pointer to a new Img_Manager downsized by
     * those factors.
     */
    void downsize(float, float);

    /**
     * @brief recolor the image to its luminance analysis
     *
     * Each pixel's luminance value is calculated by first summing 
     * the rgb value with their corresponding factor (subject to change
     * during development) then divide by 255 (since 0-255 is range of rgb)
     *
     * @param r_background red value of background
     * @param g_background green value of background
     * @param b_background blue value of background
     *
     * @return a vector of luminance value.
     * @note for the image's size and how to imagine the luminance on a picture,
     * use this image's height and width.
     */
    Artscii_Processor * luminance(uint8_t r, uint8_t g, uint8_t b);
    
    /**
     * @brief export the current img_manager to png image with given path
     *
     * @param path path to new file
     */
    void to_png(char *);
};

#endif
