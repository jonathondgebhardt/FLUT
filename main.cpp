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

        handle_load_lu = reinterpret_cast<printer_t>(dlsym(handle, "load_lu"));
        assert(handle_load_lu != nullptr);

        handle_print_lu = reinterpret_cast<printer_t>(dlsym(handle, "print_lu"));
        assert(handle_print_lu != nullptr);

        handle_close_lu = reinterpret_cast<printer_t>(dlsym(handle, "close_lu"));
        assert(handle_close_lu != nullptr);

        handle_load_file = reinterpret_cast<printer_t>(dlsym(handle, "load_file"));
        assert(handle_load_file != nullptr);

        handle_print_file = reinterpret_cast<printer_t>(dlsym(handle, "print_file"));
        assert(handle_print_file != nullptr);

        handle_close_file = reinterpret_cast<printer_t>(dlsym(handle, "close_file"));
        assert(handle_close_file != nullptr);
    }

    ~Printer() {
        dlclose(handle);
    }

    void load_lu() {
        const auto old_wd = std::filesystem::current_path();
        
        std::cout << "** changing to \"" << wd << "\" and calling " << lib << "::load_lu\n";
        std::filesystem::current_path(wd);
        handle_load_lu();
     
        std::cout << "** restoring " << old_wd << "\n";
        std::filesystem::current_path(old_wd);
    }

    void load_file() {
        const auto old_wd = std::filesystem::current_path();
        
        std::cout << "** changing to \"" << wd << "\" and calling " << lib << "::load_file\n";
        std::filesystem::current_path(wd);
        handle_load_file();
     
        std::cout << "** restoring " << old_wd << "\n";
        std::filesystem::current_path(old_wd);
    }

    void print_lu() {
        const auto old_wd = std::filesystem::current_path();
        
        std::cout << "** changing to \"" << wd << "\" and calling " << lib << "::print_lu\n\n";
        std::filesystem::current_path(wd);
        handle_print_lu();

        std::cout << "\n** restoring " << old_wd << "\n";
        std::filesystem::current_path(old_wd);
    }

    void print_file() {
        const auto old_wd = std::filesystem::current_path();
        
        std::cout << "** changing to \"" << wd << "\" and calling " << lib << "::print_file\n\n";
        std::filesystem::current_path(wd);
        handle_print_file();

        std::cout << "\n** restoring " << old_wd << "\n";
        std::filesystem::current_path(old_wd);
    }

    void close_lu() {
        handle_close_lu();
    }

    void close_file() {
        handle_close_file();
    }

    std::string lib;
    std::string wd;
    
    void* handle;
    
    printer_t handle_load_lu;
    printer_t handle_print_lu;
    printer_t handle_close_lu;

    printer_t handle_load_file;
    printer_t handle_print_file;
    printer_t handle_close_file;
};

std::string create_working_dir(const std::string& wd) {
    std::filesystem::remove_all(wd);
    std::filesystem::create_directory(wd);

    std::ofstream ofs{wd + "/fort.10"};
    ofs << "hello from " + wd + "\n";

    const auto library_copy = wd + "/libfileprinter.so." + wd;
    std::filesystem::copy("libfileprinter.so", library_copy);
    return library_copy;
}

int main() {
    const auto lib_1 = create_working_dir("1");
    const auto lib_2 = create_working_dir("2");

    
    // These subroutines are using the same logical unit, so they conflict.
    {
        std::cout << "**** Using 'LU' subroutines\n";

        Printer p1{lib_1};
        Printer p2{lib_2};

        p1.load_lu();
        p1.print_lu();
        // p1.close_lu(); // not closing the lu causes p2 to show p1's content

        p2.load_lu(); // this should load 2/fort.10, but the lu is already occupied
        p2.print_lu();
        p2.close_lu();

        // p1.print_lu(); // p2 closed p1's lu, this shows garbage
    }

    // Even though we're not explicitly closing the file streams, this works as expected because
    // the subroutines are loading the file into unique logical units.
    {
        std::cout << "**** Using 'file' subroutines\n";
        
        Printer p1{lib_1};
        Printer p2{lib_2};

        // p1.load_file();
        p1.print_file();
        // p1.close_file();

        // p2.load_file();
        p2.print_file();
        // p2.close_file();
    }

    return 0;
}