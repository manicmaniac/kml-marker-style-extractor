#include <stdio.h>
#include <string.h>
#include <iostream>
#include <kml/dom.h>
#include <kml/engine.h>
#include <curl/curl.h>

static std::string const KML_URL = "http://kml-samples.googlecode.com/svn/trunk/kml/ListStyle/item-icon-hotspot.kml";
static const size_t MAX_BUF = 6553600;

static char wr_buf[MAX_BUF + 1];
static size_t wr_index;
static int wr_error;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    size_t segsize = size * nmemb;
    if (wr_index + segsize > MAX_BUF) {
        *(int *)userp = 1;
        return 0;
    }
    memcpy((void *)&wr_buf[wr_index], buffer, segsize);
    wr_index += segsize;
    wr_buf[wr_index] = 0;
    return segsize;
}

int main(const int argc, char *const argv[]) {
    CURL *curl = curl_easy_init();
    wr_error = 0;
    wr_index = 0;

    curl_easy_setopt(curl, CURLOPT_URL, KML_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&wr_error);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    CURLcode ret = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    std::string errors;

    kmlengine::KmlFilePtr file = kmlengine::KmlFile::CreateFromParse(wr_buf, &errors);
    if (!file) {
        printf("error\n");
        return EXIT_FAILURE;
    }
    kmlengine::ElementVector placemark_elements;
    kmlengine::GetElementsById(file->root(), kmldom::Type_Placemark, &placemark_elements);
    for (auto it = placemark_elements.begin(); it != placemark_elements.end(); ++it) {
        auto feature = kmldom::AsFeature(it->get());
        auto style = kmlengine::CreateResolvedStyle(feature, file, kmldom::STYLESTATE_NORMAL);
        if (style->has_iconstyle()) {
            auto iconstyle = style->get_iconstyle();
            if (iconstyle->has_color()) {
                auto color = iconstyle->get_color();
                std::cout << color.to_string_abgr() << std::endl;
            }
        }
    }
    return EXIT_SUCCESS;
}

