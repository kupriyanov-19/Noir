#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <optional>
#include <time.h>
#include <sstream>
using namespace std;

#ifdef _WIN32
extern "C" char *strptime(const char *s, const char *f, struct tm *tm) {
    std::istringstream input(s);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(tm, f);
    if (input.fail()) {
        return nullptr;
    }
    return (char *) (s + input.tellg());
}

std::optional<time_t> StringToDate(const std::string &date) {
    if (date.empty())
        return 0;

    std::string pattern;
    if (date.size() > 5)
        pattern = "%H:%M %d/%m";
    else
        pattern = "%d/%m";

    tm time = {};
    if (!strptime(date.c_str(), pattern.c_str(), &time))
        return std::nullopt;

    time.tm_year = 123;
    tm first = {0};
    first.tm_year = 123;
    first.tm_mon = 1;
    first.tm_mday = 27;
    first.tm_hour = 3;
    time_t summer_time = mktime(&first);

    tm second = {};
    second.tm_year = 123;
    second.tm_mon = 8;
    second.tm_mday = 30;
    second.tm_hour = 3;
    time_t winter_time = mktime(&second);

    time_t result = mktime(&time);
    if (result >= summer_time && result < winter_time) result -= 3600;
    return result;
}
#else
std::optional<time_t> StringToDate(const std::string& date) {
    if (date.empty())
        return 0;

    std::string pattern;
    if (date.size() > 5)
        pattern = "%H:%M %d/%m";
    else
        pattern = "%d/%m";

    tm time = {};
    if (!strptime(date.c_str(), pattern.c_str(), &time))
        return std::nullopt;

    time.tm_year = 2021;
    tm first = {};
    first.tm_year = 2021;
    first.tm_mon = 2;
    first.tm_mday = 27;
    first.tm_hour = 3;
    time_t summer_time = mktime(&first);

    tm second = {};
    second.tm_year = 2021;
    second.tm_mon = 9;
    second.tm_mday = 30;
    second.tm_hour = 3;
    time_t winter_time = mktime(&second);

    time_t result = mktime(&time);
    if (result >= summer_time && result < winter_time) result -= 3600;
    return result;
}
#endif


int main() {
    std::string s;
    while(true) {
        getline(std::cin, s);
        if (s == "now") {
            auto now = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(now);
            cout << end_time << '\n';
        }
        else {
            auto f = StringToDate(s);
            if (f.has_value()) std::cout<<f.value()<<'\n';
            else cout<<"error\n";
        }
    }
}

