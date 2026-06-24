#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

void pauseConsole() {
  std::cout << "\nPress Enter to continue...";
  std::cin.clear();
  std::string temp;
  std::getline(std::cin, temp);
}
std::string getValidatedFilename() {
  std::string filename;
  std::cout << "Enter filename ";
  std::cout << "\033[32m";
  std::cout << "(e.g., my document.txt): ";
  std::cout << "\033[0m";
  std::getline(std::cin, filename);

  if (filename.find('.') == std::string::npos) {
    filename += ".txt";
    std::cout << "\033[91m";
    std::cout << "No extension specified, automatically using .txt.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
  }

  return filename;
}

void listAndNavigateFiles() {
  std::cout << "\033[2J\033[1;1H";
  std::cout << "\033[33m";
  std::cout << "--- File & Directory Navigation ---\n";
  std::cout << "\033[0m";
  std::cout << "Current directory: ";
  std::cout << "\033[32m";
  std::cout << std::filesystem::current_path() << "\n\n";
  std::cout << "\033[0m";

  try {
    std::cout << "\033[93m";
    std::cout << "Contents:\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    for (const auto &entry : std::filesystem::directory_iterator(".")) {
      if (std::filesystem::is_directory(entry.path())) {
        std::cout << "\033[32m";
        std::cout << "  [DIR] " << entry.path().filename().string() << "/\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                              FOREGROUND_GREEN);
      } else {
        std::cout << "\033[33m";
        std::cout << "  [FILE] " << entry.path().filename().string() << "\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                              FOREGROUND_GREEN);
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cout << "\033[31m";
    std::cerr << "Error listing directory: " << e.what() << "\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    pauseConsole();
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
      std::filesystem::current_path(newPath);
      std::cout << "Changed directory to: ";
      std::cout << "\033[32m";
      std::cout << std::filesystem::current_path() << "\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    } catch (const std::filesystem::filesystem_error &e) {
      std::cout << "\033[31m";
      std::cerr << "Error changing directory: " << e.what() << "\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    }
  }
  pauseConsole();
}
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
              SetConsoleTextAttribute(hConsole, FOREGROUND_RED |
                                                    FOREGROUND_BLUE |
                                                    FOREGROUND_GREEN);
              found = true;
            }
            std::cout << "  File: ";
            std::cout << "\033[33m";
            std::cout << entry.path().filename().string();
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                                  FOREGROUND_GREEN);
            std::cout << ", Line " << lineNumber << ": ";
            std::cout << "\033[36m";
            std::cout << line << "\n";
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                                  FOREGROUND_GREEN);
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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
  }
  pauseConsole();
}

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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
  outputFile << std::cin.rdbuf();
  outputFile.close();

  std::cin.clear();
  std::cout << "\033[32m";
  std::cout << "File '";
  std::cout << "\033[33m";
  std::cout << filename;
  std::cout << "\033[32m";
  std::cout << "' saved successfully.\n";
  std::cout << "\033[0m";
  pauseConsole();
}

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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
  while (std::getline(inputFile, line)) {
    std::cout << line << '\n';
  }
  std::cout << "\033[32m";
  std::cout << "------------------------------------\n";
  std::cout << "\033[0m";
  inputFile.close();
  pauseConsole();
}

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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      pauseConsole();
      return;
    }
    std::cout << "\nEnter new content to append. ";
    std::cout << "\033[36m";
    std::cout << "Press Ctrl+D (Unix/Linux) or Ctrl+Z/F6 (Windows) followed by "
                 "Enter to save";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      break;
    }
    std::cout << "\033[36m";
    std::cout << "Enter new content for line " << lineToEdit << ":\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      break;
    }
    std::cout << "\033[36m";
    std::cout << "Enter new content to insert:\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    std::string newLineContent;
    std::getline(std::cin, newLineContent);
    lines.insert(lines.begin() + lineToInsert - 1, newLineContent);
    std::cout << "\033[32m";
    std::cout << "Line inserted.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    break;
  }
  default: {
    std::cout << "\033[31m";
    std::cerr << "Invalid choice. No changes made.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    } else {
      std::cout << "\033[31m";
      std::cerr << "Error: Could not delete file. " << ec.message()
                << std::endl;
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    }
  } else {
    std::cout << "\033[36m";
    std::cout << "Deletion cancelled.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      break;
    default:
      std::cout << "\033[31m";
      std::cerr << "Invalid choice. Please enter a number between 1 and 7.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      pauseConsole();
      break;
    }
  } while (choice != 7);
  pauseConsole();

  return 0;
}