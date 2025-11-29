#include "../imported/tinyfiledialogs.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

std::string readFileToString(const char* filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "Error";
    }

    // Get the size of the file
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg); // Move the file pointer back to the beginning

    // Create a string with the exact size of the file
    std::string fileContents(size, '\0');  // Initialize the string with null-terminators

    // Read the entire file into the string
    if (!file.read(&fileContents[0], size)) {
        std::cerr << "Error reading the file." << std::endl;
        return "Error";
    }

    file.close();  // Close the file

    return fileContents;  // Return the string containing file contents
}

void SaveShip(const std::string &blueprint, const std::string& file_name)
{
    const char* filePath = tinyfd_saveFileDialog(
        "Save Ship File",            // Title of the dialog
        file_name.c_str(),             // Default filename
        1,                           // Number of file filters (1 for simplicity)
        new const char* { "*.json" }, // File filters (only allow JSON files)
        "Ship Files"                 // Filter description (e.g., "JSON Files")
    );

    if (filePath) // Check if a file path was selected
    {
        std::ofstream ShipFile(filePath);

        ShipFile << blueprint;

        ShipFile.close();
    }
    else
    {
        std::cout << "Save operation cancelled." << std::endl;
    }
}

std::string LoadShip(std::string * path)
{
    const char* filePath = tinyfd_openFileDialog(
        "Open Ship File",            // Title of the dialog
        "",                          // Initial directory (empty string means default)
        1,                           // Number of file filters (set to 1 for simplicity)
        new const char* { "*.json" }, // File filters (only JSON files in this case)
        "Ship Files",                 // Filter description (e.g., "JSON Files")
        0                             // No multiple selects
    );

    if (filePath) // Check if a file was selected
    {
        std::cout << "File selected: " << filePath << std::endl;

		// char drive[_MAX_DRIVE];
		// char dir[_MAX_DIR];
		// char fname[_MAX_FNAME];
		// char ext[_MAX_EXT];

        // _splitpath_s(filePath, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);

        std::filesystem::path p(filePath);
        std::string drive = p.root_name().string();
        std::string dir = p.parent_path().string();
        std::string fname = p.stem().string();
        std::string ext = p.extension().string();


        if(path != nullptr)
            *path = filePath;

        std::ifstream shipFile(filePath);
        if (!shipFile) {
            std::cout << "Failed to open file: " << filePath << std::endl;
            return "Error";
        }
        return readFileToString(filePath);
    }
    else
    {
        std::cout << "No file selected." << std::endl;
        return "";
    }
}