#include <iostream>
#include <dlfcn.h>
#include <filesystem>
#include <cassert>
#include <fstream>

struct Printer {
    typedef void (*printer_t)();

    Printer(const std::string& lib) : lib(lib), wd(std::filesystem::path(lib).parent_path().string()) {
        handle = dlopen(lib.c_str(), RTLD_LAZY);
        assert(handle != nullptr);

        handle_load = reinterpret_cast<printer_t>(dlsym(handle, "load_file"));
        assert(handle_load != nullptr);

        handle_printer = reinterpret_cast<printer_t>(dlsym(handle, "print_lu"));
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

std::string create_working_dir(const std::string& wd) {
    std::filesystem::remove_all(wd);
    std::filesystem::create_directory(wd);

    const auto file_content = "hello from " + wd + "\n";
    {
        std::ofstream ofs{wd + "/fort.10"};
        ofs << file_content;
    }
    {
        std::ofstream ofs{wd + "/file.txt"};
        ofs << file_content;
    }

    const auto library_copy = wd + "/libfileprinter.so." + wd;
    std::filesystem::copy("libfileprinter.so", library_copy);
    return library_copy;
}

int main() {
    const auto lib_1 = create_working_dir("1");
    const auto lib_2 = create_working_dir("2");

    Printer p1{lib_1};
    Printer p2{lib_2};
    
    p1.load();
    p1.print();
    p1.close();

    p2.load();
    p2.print();
    p2.close();

    return 0;
}