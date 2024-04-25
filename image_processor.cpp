#include "io/bmp.h"
#include "filters/filter_factory.h"

int main(int argc, const char *argv[]) {
    if (argc <= 3) {
        throw std::runtime_error(
            "Description of the form of command line arguments:\n"
            "{program name} "
            "{path to input file} {path to output file} [-{filter 1 name} "
            "[filter 1 parameter] [filter 2 parameter] ...] [-{filter 2 name} "
            "[filter 1 parameter] [ filter parameter 2] ...] ...\n\n"
            "Available filters:\n"
            "+---------+-----------------+----------------+\n"
            "| flag    | name            | args           |\n"
            "+---------+-----------------+----------------+\n"
            "| -gs     | Crop            |                |\n"
            "| -neg    | Negative        | width, height  |\n"
            "| -sharp  | Sharpening      |                |\n"
            "| -edge   | Edge Detection  | threshold      |\n"
            "| -blur   | Gaussian Blur   | sigma          |\n"
            "| -cryst  | Crystallize     |                |\n"
            "+---------+-----------------+----------------+\n");
        return 0;
    }

    std::string filename = argv[1];
    std::string save_filename = argv[2];

    std::vector<std::string> flags;

    for (int i = 3; i < argc; ++i) {
        flags.push_back(argv[i]);
    }

    FilterFactory factory(flags);

    BMP image_format;

    Image image = image_format.Load(filename);

    image = factory.ApplyAll(image);

    image_format.Save(image, save_filename);

    return 0;
}