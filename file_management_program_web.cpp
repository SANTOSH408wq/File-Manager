#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>


// Get a handle to the standard output console.


void pauseConsole() {
    std::cout << "\nPress Enter to continue...";
    std::cin.clear();
    std::string temp;
    std::getline(std::cin, temp);
}

// Function to get a validated filename from the user.
// It automatically appends a ".txt" extension if none is provided.
std::string getValidatedFilename() {
  std::string filename;
  std::cout << "Enter filename ";
  std::cout << "\033[32m";
  std::cout << "(e.g., my document.txt): ";
  std::cout << "\033[0m";
  std::getline(std::cin, filename);

  if (filename.find('.') == std::string::npos) {
    filename += ".txt";
    std::cout << "\033[31m";
    std::cout << "No extension specified, automatically using .txt.\n";
    std::cout << "\033[0m";
  }

  return filename;
}

// Function to list files in the current directory and allow navigation.
void listAndNavigateFiles() {
  std::cout << "\033[2J\033[1;1H"; // Clears the console screen.
  std::cout << "\033[33m";
  std::cout << "--- File & Directory Navigation ---\n";
  std::cout << "\033[0m";
  std::cout << "Current directory: ";
  std::cout << "\033[32m";
  std::cout << std::filesystem::current_path() << "\n\n";
  std::cout << "\033[0m";

  try {
    std::cout << "\033[33m";
    std::cout << "Contents:\n";
    std::cout << "\033[0m";
    // Iterates through the current directory and prints its contents.
    for (const auto &entry : std::filesystem::directory_iterator(".")) {
      if (std::filesystem::is_directory(entry.path())) {
        std::cout << "\033[32m";
        std::cout << "  [DIR] " << entry.path().filename().string() << "/\n";
        std::cout << "\033[0m";
      } else {
        std::cout << "\033[33m";
        std::cout << "  [FILE] " << entry.path().filename().string() << "\n";
        std::cout << "\033[0m";
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cout << "\033[31m";
    std::cerr << "Error listing directory: " << e.what() << "\n";
    std::cout << "\033[0m";
    pauseConsole(); // Pauses execution until a key is pressed.
    return;
  }

  std::cout << "\nTo change directory, enter a path ";
  std::cout << "\033[32m";
  std::cout << "(e.g., '..', 'documents/', or 'C:/my_folder')";
  std::cout << "\033[0m";
  std::cout << ".\n";
  std::cout
      << "To go back to the main menu, press Enter without typing anything.\n";
  std::cout << "Enter new path: ";

  std::string newPath;
  std::getline(std::cin, newPath);

  if (!newPath.empty()) {
    try {
      std::filesystem::current_path(
          newPath); // Changes the current working directory.
      std::cout << "Changed directory to: ";
      std::cout << "\033[32m";
      std::cout << std::filesystem::current_path() << "\n";
      std::cout << "\033[0m";
    } catch (const std::filesystem::filesystem_error &e) {
      std::cout << "\033[31m";
      std::cerr << "Error changing directory: " << e.what() << "\n";
      std::cout << "\033[0m";
    }
  }
  pauseConsole();
}

// Function to search for specific content within files in the current
// directory.
void searchForContent() {
  std::cout << "\033[2J\033[1;1H";
  std::cout << "\033[33m";
  std::cout << "--- Search for Content ---\n";
  std::cout << "\033[0m";
  std::string searchTerm;
  std::cout << "Enter the text to search for: ";
  std::getline(std::cin, searchTerm);

  if (searchTerm.empty()) {
    std::cout << "\033[31m";
    std::cout << "Search term cannot be empty.\n";
    pauseConsole();
    return;
  }

  bool found = false;
  // Iterates through all files in the current directory.
  for (const auto &entry : std::filesystem::directory_iterator(".")) {
    if (entry.is_regular_file()) {
      std::ifstream inputFile(entry.path());
      if (inputFile.is_open()) {
        std::string line;
        int lineNumber = 1;
        // Reads the file line by line to search for the term.
        while (std::getline(inputFile, line)) {
          if (line.find(searchTerm) != std::string::npos) {
            if (!found) {
              std::cout << "\033[36m";
              std::cout << "\nFound matches:\n";
              std::cout << "\033[0m";
              found = true;
            }
            std::cout << "  File: ";
            std::cout << "\033[33m";
            std::cout << entry.path().filename().string();
            std::cout << "\033[0m";
            std::cout << ", Line " << lineNumber << ": ";
            std::cout << "\033[36m";
            std::cout << line << "\n";
            std::cout << "\033[0m";
          }
          lineNumber++;
        }
        inputFile.close();
      }
    }
  }

  if (!found) {
    std::cout << "\033[31m";
    std::cout << "\nNo files containing '";
    std::cout << "\033[36m";
    std::cout << searchTerm;
    std::cout << "\033[31m";
    std::cout << "' were found.\n";
    std::cout << "\033[0m";
  }
  pauseConsole();
}

// Function to create a new file and save content entered by the user.
void createAndSaveFile() {
  std::cout << "\033[2J\033[1;1H";
  std::string filename = getValidatedFilename();

  std::ofstream outputFile(filename);
  if (!outputFile.is_open()) {
    std::cout << "\033[31m";
    std::cerr << "Error: Could not create or open file '";
    std::cout << "\033[33m";
    std::cout << filename;
    std::cout << "'.\n";
    std::cout << "\033[0m";
    pauseConsole();
    return;
  }

  std::cout << "Enter content for '";
  std::cout << "\033[33m";
  std::cout << filename;
  std::cout << "\033[0m";
  std::cout << "'.";
  std::cout << "\033[32m";
  std::cout << " Press Ctrl+D (Unix/Linux) or Ctrl+Z/F6 (Windows) followed by "
               "Enter to save";
  std::cout << "\033[0m";
  std::cout << ".\n";
  outputFile << std::cin.rdbuf(); // Reads content from standard input and
                                  // writes to the file.
  outputFile.close();

  std::cin.clear(); // Clears any error flags on cin.
  std::cout << "\033[32m";
  std::cout << "File '";
  std::cout << "\033[33m";
  std::cout << filename;
  std::cout << "\033[32m";
  std::cout << "' saved successfully.\n";
  std::cout << "\033[0m";
  pauseConsole();
}

// Function to view the contents of an existing file.
void viewFile() {
  std::cout << "\033[2J\033[1;1H";
  std::string filename = getValidatedFilename();

  std::ifstream inputFile(filename);
  if (!inputFile.is_open()) {
    std::cout << "\033[31m";
    std::cerr << "Error: File '";
    std::cout << "\033[33m";
    std::cout << filename;
    std::cout << "\033[31m";
    std::cout << "' not found. Check the filename and path.\n";
    std::cout << "\033[0m";
    pauseConsole();
    return;
  }
  std::cout << "\033[32m";
  std::cout << "\n--- Content of '";
  std::cout << "\033[33m";
  std::cout << filename;
  std::cout << "\033[32m";
  std::cout << "' ---\n";
  std::cout << "\033[0m";
  std::string line;
  // Reads and prints the file content line by line.
  while (std::getline(inputFile, line)) {
    std::cout << line << '\n';
  }
  std::cout << "\033[32m";
  std::cout << "------------------------------------\n";
  std::cout << "\033[0m";
  inputFile.close();
  pauseConsole();
}

// Function to edit a file with options to append, replace, delete, or insert
// lines.
void editFile() {
  std::cout << "\033[2J\033[1;1H";
  std::string filename = getValidatedFilename();

  std::ifstream checkFile(filename);
  if (!checkFile.is_open()) {
    std::cout << "\033[31m";
    std::cerr << "Error: File '";
    std::cout << "\033[33m";
    std::cout << filename;
    std::cout << "\033[31m";
    std::cout << "' not found or could not be opened for editing.\n";
    std::cout << "\033[0m";
    pauseConsole();
    return;
  }
  checkFile.close();

  std::cout << "\033[36m";
  std::cout << "\n--- Edit Options ---\n";
  std::cout << "\033[0m";
  std::cout << "1. Append to the end of the file\n";
  std::cout << "2. Replace an entire line\n";
  std::cout << "3. Delete a specific line\n";
  std::cout << "4. Insert a new line\n";
  std::cout << "\033[0m";
  std::cout << "Enter your choice: ";
  int editChoice;
  std::cin >> editChoice;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << "\033[2J\033[1;1H";

  std::vector<std::string> lines;
  std::ifstream currentFile(filename);
  std::string line;
  while (std::getline(currentFile, line)) {
    lines.push_back(line);
  }
  currentFile.close();

  std::cout << "\033[33m";
  std::cout << "\n--- Current File Content ---\n";
  std::cout << "\033[0m";
  for (size_t i = 0; i < lines.size(); ++i) {
    std::cout << i + 1 << ": " << lines[i] << '\n';
  }
  std::cout << "\033[33m";
  std::cout << "---------------------------\n";
  std::cout << "\033[0m";

  switch (editChoice) {
  case 1: // Append
  {
    std::ofstream outputFile(filename,
                             std::ios::app); // Opens file in append mode.
    if (!outputFile.is_open()) {
      std::cout << "\033[31m";
      std::cerr << "Error: Could not open file for appending.\n";
      std::cout << "\033[0m";
      pauseConsole();
      return;
    }
    std::cout << "\nEnter new content to append. ";
    std::cout << "\033[36m";
    std::cout << "Press Ctrl+D (Unix/Linux) or Ctrl+Z/F6 (Windows) followed by "
                 "Enter to save";
    std::cout << "\033[0m";
    std::cout << ".\n";
    outputFile << std::cin.rdbuf();
    outputFile.close();
    std::cin.clear();
    std::cout << "\033[32m";
    std::cout << "File '";
    std::cout << "\033[33m";
    std::cout << filename;
    std::cout << "\033[32m";
    std::cout << "' updated successfully.\n";
    std::cout << "\033[0m";
    break;
  }
  case 2: // Replace
  {
    int lineToEdit;
    std::cout << "Enter line number to replace: ";
    std::cin >> lineToEdit;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (lineToEdit < 1 || lineToEdit > lines.size()) {
      std::cout << "\033[31m";
      std::cerr << "Invalid line number. No changes made.\n";
      std::cout << "\033[0m";
      break;
    }
    std::cout << "\033[36m";
    std::cout << "Enter new content for line " << lineToEdit << ":\n";
    std::cout << "\033[0m";
    std::string newLineContent;
    std::getline(std::cin, newLineContent);
    lines[lineToEdit - 1] = newLineContent;
    break;
  }
  case 3: // Delete Line
  {
    int lineToDelete;
    std::cout << "Enter line number to delete: ";
    std::cin >> lineToDelete;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (lineToDelete < 1 || lineToDelete > lines.size()) {
      std::cout << "\033[31m";
      std::cerr << "Invalid line number. No changes made.\n";
      std::cout << "\033[0m";
      break;
    }
    lines.erase(lines.begin() + lineToDelete - 1);
    std::cout << "\033[32m";
    std::cout << "Line " << lineToDelete << " deleted.\n";
    break;
  }
  case 4: // Insert Line
  {
    int lineToInsert;
    std::cout << "Enter line number to insert before: ";
    std::cin >> lineToInsert;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (lineToInsert < 1 || lineToInsert > lines.size() + 1) {
      std::cout << "\033[31m";
      std::cerr << "Invalid line number. No changes made.\n";
      std::cout << "\033[0m";
      break;
    }
    std::cout << "\033[36m";
    std::cout << "Enter new content to insert:\n";
    std::cout << "\033[0m";
    std::string newLineContent;
    std::getline(std::cin, newLineContent);
    lines.insert(lines.begin() + lineToInsert - 1, newLineContent);
    std::cout << "\033[32m";
    std::cout << "Line inserted.\n";
    std::cout << "\033[0m";
    break;
  }
  default: {
    std::cout << "\033[31m";
    std::cerr << "Invalid choice. No changes made.\n";
    std::cout << "\033[0m";
    pauseConsole();
    return;
  }
  }

  // Writes the modified content back to the file.
  if (editChoice == 2 || editChoice == 3 || editChoice == 4) {
    std::ofstream outputFile(
        filename, std::ios::trunc); // Overwrites the file with the new content.
    if (!outputFile.is_open()) {
      std::cout << "\033[31m";
      std::cerr << "Error: Could not open file for writing.\n";
      std::cout << "\033[0m";
      pauseConsole();
      return;
    }
    for (const auto &l : lines) {
      outputFile << l << '\n';
    }
    outputFile.close();
    std::cout << "\033[32m";
    std::cout << "File '";
    std::cout << "\033[33m";
    std::cout << filename;
    std::cout << "\033[32m";
    std::cout << "' updated successfully.\n";
    std::cout << "\033[0m";
  }

  pauseConsole();
}

// Function to delete a file after user confirmation.
void deleteFile() {
  std::cout << "\033[2J\033[1;1H";
  std::string filename = getValidatedFilename();

  std::cout << "\033[31m";
  std::cout << "Are you sure you want to delete '";
  std::cout << "\033[33m";
  std::cout << filename;
  std::cout << "\033[31m";
  std::cout << "'? (y/n): ";
  std::cout << "\033[0m";
  char confirmation;
  std::cin >> confirmation;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (confirmation == 'y' || confirmation == 'Y') {
    std::error_code ec;
    if (std::filesystem::remove(filename, ec)) {
      std::cout << "\033[32m";
      std::cout << "File '";
      std::cout << "\033[33m";
      std::cout << filename;
      std::cout << "\033[32m";
      std::cout << "' deleted successfully.\n";
      std::cout << "\033[0m";
    } else {
      std::cout << "\033[31m";
      std::cerr << "Error: Could not delete file. " << ec.message()
                << std::endl;
      std::cout << "\033[0m";
    }
  } else {
    std::cout << "\033[36m";
    std::cout << "Deletion cancelled.\n";
    std::cout << "\033[0m";
  }
  pauseConsole();
}

// Main function that displays the menu and handles user choices.
int main() {
  int choice;
  do {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[33m";
    std::cout << "--- File Management Program ---\n";
    std::cout << "\033[0m";
    std::cout << "1. Create/Save a new file\n";
    std::cout << "2. View an existing file\n";
    std::cout << "3. Edit an existing file\n";
    std::cout << "4. Delete a file\n";
    std::cout << "5. Search for content in files\n";
    std::cout << "6. List files & navigate directories\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";

    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "\033[31m";
      std::cerr << "Invalid input. Please enter a number.\n";
      std::cout << "\033[0m";
      pauseConsole();
      continue;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
    case 1:
      createAndSaveFile();
      break;
    case 2:
      viewFile();
      break;
    case 3:
      editFile();
      break;
    case 4:
      deleteFile();
      break;
    case 5:
      searchForContent();
      break;
    case 6:
      listAndNavigateFiles();
      break;
    case 7:
      std::cout << "\033[36m";
      std::cout << "Exiting program. Goodbye!\n";
      std::cout << "\033[0m";
      break;
    default:
      std::cout << "\033[31m";
      std::cerr << "Invalid choice. Please enter a number between 1 and 7.\n";
      std::cout << "\033[0m";
      pauseConsole();
      break;
    }
  } while (choice != 7);
  pauseConsole();

  return 0;
}