#include <iostream>
#include <dlfcn.h>
#include <filesystem>
#include <cassert>

struct Printer {
    typedef void (*printer_t)();

    Printer(const std::string& lib, const std::string wd) : lib(lib), wd(wd) {
        handle = dlopen(lib.c_str(), RTLD_LAZY);
        assert(handle != nullptr);

        handle_load = reinterpret_cast<printer_t>(dlsym(handle, "load_file"));
        assert(handle_load != nullptr);

        handle_printer = reinterpret_cast<printer_t>(dlsym(handle, "print_file"));
        assert(handle_printer != nullptr);

        handle_close = reinterpret_cast<printer_t>(dlsym(handle, "close_file"));
        assert(handle_close != nullptr);
    }

    ~Printer() {
        dlclose(handle);
    }

    void load() {
        const auto old_wd = std::filesystem::current_path();
        
        std::cout << "** changing to \"" << wd << "\" and calling " << lib << "::load\n";
        std::filesystem::current_path(wd);
        handle_load();
     
        std::cout << "** restoring " << old_wd << "\n";
        std::filesystem::current_path(old_wd);
    }

    void print() {
        const auto old_wd = std::filesystem::current_path();
        
        std::cout << "** changing to \"" << wd << "\" and calling " << lib << "::print\n\n";
        std::filesystem::current_path(wd);
        handle_printer();

        std::cout << "\n** restoring " << old_wd << "\n";
        std::filesystem::current_path(old_wd);
    }

    void close() {
        handle_close();
    }

    std::string lib;
    std::string wd;
    void* handle;
    printer_t handle_load;
    printer_t handle_printer;
    printer_t handle_close;
};

int main() {
    std::filesystem::remove_all("1");
    std::filesystem::remove_all("2");

    std::filesystem::create_directory("1");
    std::filesystem::copy("../fort.10_1", "1/fort.10");
    std::filesystem::create_directory("2");
    std::filesystem::copy("../fort.10_2", "2/fort.10");

    Printer p1{"./libfileprinter_1.so", "1"};
    Printer p2{"./libfileprinter_2.so", "2"};
    
    p1.load();
    p1.print();
    p1.close();

    p2.load();
    p2.print();
    p2.close();

    return 0;
}