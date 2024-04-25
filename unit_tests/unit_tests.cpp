#include "../include/io/bmp.h"
#include "../include/filters/filter_factory.h"

#include <iostream>

int Tests() {
    BMP image_format;
    size_t text_case = 0;

    std::cout << "== Errors testing started ==" << std::endl;

    // wrong file type
    try {
        std::string filename = "../data/Not_bmp.jpeg";
        Image image = image_format.Load(filename);
    } catch (const std::runtime_error& e) {
        std::cout << "- Test case " << ++text_case << " passed (wrong file type). Error catched: " << e.what()
                  << std::endl;
    }

    // wrong file name
    try {
        std::string filename = "Untitled.bmp";
        Image image = image_format.Load(filename);
    } catch (const std::invalid_argument& e) {
        std::cout << "- Test case " << ++text_case << " passed (wrong file name). Error catched: " << e.what()
                  << std::endl;
    }

    // wrong filter flag
    try {
        std::vector<std::string> flags = {"-rgb"};
        FilterFactory factory(flags);
    } catch (const std::invalid_argument& e) {
        std::cout << "- Test case " << ++text_case << " passed (wrong filter flag). Error catched: " << e.what()
                  << std::endl;
    }

    // wrong arguments for flag
    try {
        std::vector<std::string> flags = {"-crop", "100"};
        FilterFactory factory(flags);
    } catch (const std::invalid_argument& e) {
        std::cout << "- Test case " << ++text_case << " passed (wrong arguments for flag). Error catched: " << e.what()
                  << std::endl;
    }

    // flags testing
    std::cout << "== Flags testing started ==" << std::endl;

    std::vector<std::vector<std::string>> filter_tests = {
        {"-gs"}, {"-crop", "1000", "1000"}, {"-neg"}, {"-sharp"}, {"-edge", "0.1"}, {"-blur", "10"}, {"-cryst"}};
    std::string filename = "../data/lenna.bmp";
    std::string save_foldername = "../output/";

    for (size_t i = 0; i < filter_tests.size(); ++i) {
        Image image = image_format.Load(filename);
        FilterFactory factory(filter_tests[i]);
        image = factory.ApplyAll(image);
        image_format.Save(image, save_foldername + "lenna(" + filter_tests[i][0] + ").bmp");
        std::cout << "- Test case " << ++text_case << " passed (flag " << filter_tests[i][0] << ")." << std::endl;
    }

    return 0;
}

int main() {
    Tests();
    return 0;
}