/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "IO.h"

// For image loading : https://learnopengl.com/Getting-started/Textures
//#define STB_IMAGE_IMPLEMENTATION
//#include "../Dependencies/stb/stb_image.h"

namespace ME {
    std::ifstream IO::open(const std::string& path) {
        std::ifstream
            ifs(path);
        if (!ifs.is_open())
            throw(std::runtime_error(std::string("Cannot find ") + path));
        return ifs;
    }
    // Text
    std::string IO::read_text(const std::string& path) {
        std::ifstream
            ifs = open(path);
        std::string
            str;
        ifs.seekg(0, std::ios::end);
        auto
            size = ifs.tellg();
        str.resize(size);
        ifs.seekg(0, std::ios::beg);
        ifs.read(&str[0], size);
        ifs.close();
        return str;
    }
    std::vector<std::string> IO::read_text_lines(const std::string& path) {
        std::ifstream
            ifs = open(path);
        if (!ifs.is_open()) {
            throw std::invalid_argument("Invalid path name : " + path);
        }
        std::vector<std::string>
            ret;
        while (!ifs.eof()) {
            std::string
                line;
            std::getline(ifs, line);
            ret.push_back(line);
        }
        ifs.close();
        return ret;
    }
    // Image
    //void IO::read_image(const std::string& path, unsigned char** bits, int& width, int& height) {
    //    int nr_channels;
    //    *bits = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
    //}
    //void IO::free_image(unsigned char* bits) {
    //    stbi_image_free(bits);
    //}
    //void IO::save_image(const std::string& filename, unsigned char* bytes, int width, int height) {
    //    // Later...
    //}
}