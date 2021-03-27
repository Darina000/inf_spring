#include <fstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <filesystem>
#include <sstream>

auto compute_file_size(const std::filesystem::path & path)
{
    if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
    {
        auto file_size = std::filesystem::file_size(path);

        if (file_size != static_cast <uintmax_t> (-1))
        {
            return file_size;
        }
    }

    return static_cast <uintmax_t> (-1);
}

auto compute_directory_size(const std::filesystem::path & path)
{
    uintmax_t size = 0ULL;

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const & entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (std::filesystem::is_regular_file(entry.status()) || std::filesystem::is_symlink(entry.status()))
            {
                auto file_size = std::filesystem::file_size(entry);

                if (file_size != static_cast <uintmax_t> (-1))
                {
                    size += file_size;
                }
            }
        }
    }

    return size;
}



int main(){
    auto path = std::filesystem::current_path() / "/Users/darazemlanskaa/Documents/Week1/Week6";
    if (!std::filesystem::exists(path))
    {
        std::cerr << "Path " << path << " does not exist.\n";

        return 0;
    }
    try
    {
        std::filesystem::canonical(path).string();
    }
    catch (const std::filesystem::filesystem_error & exception)
    {
        std::cout << "exception: " << exception.what() << std::endl;
    }
    
    std::cout << path << " " <<  compute_directory_size(path) << std::endl;
    std::cout <<  std::endl;
    
    
    
    const int WIDTH = 30;
    std::cout.setf(std::ios::fixed);
    
    for (const auto & entry : std::filesystem::directory_iterator(path)){
        
        auto ftime = std::filesystem::last_write_time(entry);
        const std::filesystem::path textExtension = entry.path().extension();
        std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
        
        std::cout << entry.path()
            <<  std::setw(WIDTH/2)
            << entry.path().filename().string()
            <<  std::setw(WIDTH)
            << compute_file_size(entry.path())
            <<  std::setw(WIDTH/2)
            << textExtension
            << std::setw(WIDTH/2)
            <<  "time " <<  std::asctime(std::localtime(&cftime)) << std::endl;
        
    }
    
    return 0;
}
