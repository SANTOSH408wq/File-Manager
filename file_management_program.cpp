#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <windows.h> // This header is used for Windows-specific console functions.

// Get a handle to the standard output console.
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Function to get a validated filename from the user.
// It automatically appends a ".txt" extension if none is provided.
std::string getValidatedFilename() {
  std::string filename;
  std::cout << "Enter filename ";
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << "(e.g., my document.txt): ";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  std::getline(std::cin, filename);

  if (filename.find('.') == std::string::npos) {
    filename += ".txt";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << "No extension specified, automatically using .txt.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
  }

  return filename;
}

// Function to list files in the current directory and allow navigation.
void listAndNavigateFiles() {
  system("cls"); // Clears the console screen.
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << "--- File & Directory Navigation ---\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  std::cout << "Current directory: ";
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << std::filesystem::current_path() << "\n\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

  try {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN |
                                          FOREGROUND_INTENSITY);
    std::cout << "Contents:\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    // Iterates through the current directory and prints its contents.
    for (const auto &entry : std::filesystem::directory_iterator(".")) {
      if (std::filesystem::is_directory(entry.path())) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        std::cout << "  [DIR] " << entry.path().filename().string() << "/\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                              FOREGROUND_GREEN);
      } else {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED);
        std::cout << "  [FILE] " << entry.path().filename().string() << "\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                              FOREGROUND_GREEN);
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cerr << "Error listing directory: " << e.what() << "\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    system("pause"); // Pauses execution until a key is pressed.
    return;
  }

  std::cout << "\nTo change directory, enter a path ";
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << "(e.g., '..', 'documents/', or 'C:/my_folder')";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
      std::cout << std::filesystem::current_path() << "\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    } catch (const std::filesystem::filesystem_error &e) {
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Error changing directory: " << e.what() << "\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    }
  }
  system("pause");
}

// Function to search for specific content within files in the current
// directory.
void searchForContent() {
  system("cls");
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << "--- Search for Content ---\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  std::string searchTerm;
  std::cout << "Enter the text to search for: ";
  std::getline(std::cin, searchTerm);

  if (searchTerm.empty()) {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << "Search term cannot be empty.\n";
    system("pause");
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
              SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
              std::cout << "\nFound matches:\n";
              SetConsoleTextAttribute(hConsole, FOREGROUND_RED |
                                                    FOREGROUND_BLUE |
                                                    FOREGROUND_GREEN);
              found = true;
            }
            std::cout << "  File: ";
            SetConsoleTextAttribute(hConsole,
                                    FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << entry.path().filename().string();
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                                  FOREGROUND_GREEN);
            std::cout << ", Line " << lineNumber << ": ";
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
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
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << "\nNo files containing '";
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    std::cout << searchTerm;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << "' were found.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
  }
  system("pause");
}

// Function to create a new file and save content entered by the user.
void createAndSaveFile() {
  system("cls");
  std::string filename = getValidatedFilename();

  std::ofstream outputFile(filename);
  if (!outputFile.is_open()) {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cerr << "Error: Could not create or open file '";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    std::cout << filename;
    std::cout << "'.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    system("pause");
    return;
  }

  std::cout << "Enter content for '";
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << filename;
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  std::cout << "'.";
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << " Press Ctrl+D (Unix/Linux) or Ctrl+Z/F6 (Windows) followed by "
               "Enter to save";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  std::cout << ".\n";
  outputFile << std::cin.rdbuf(); // Reads content from standard input and
                                  // writes to the file.
  outputFile.close();

  std::cin.clear(); // Clears any error flags on cin.
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << "File '";
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << filename;
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << "' saved successfully.\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  system("pause");
}

// Function to view the contents of an existing file.
void viewFile() {
  system("cls");
  std::string filename = getValidatedFilename();

  std::ifstream inputFile(filename);
  if (!inputFile.is_open()) {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cerr << "Error: File '";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    std::cout << filename;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << "' not found. Check the filename and path.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    system("pause");
    return;
  }
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << "\n--- Content of '";
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << filename;
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << "' ---\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  std::string line;
  // Reads and prints the file content line by line.
  while (std::getline(inputFile, line)) {
    std::cout << line << '\n';
  }
  SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
  std::cout << "------------------------------------\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  inputFile.close();
  system("pause");
}

// Function to edit a file with options to append, replace, delete, or insert
// lines.
void editFile() {
  system("cls");
  std::string filename = getValidatedFilename();

  std::ifstream checkFile(filename);
  if (!checkFile.is_open()) {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cerr << "Error: File '";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    std::cout << filename;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cout << "' not found or could not be opened for editing.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    system("pause");
    return;
  }
  checkFile.close();

  SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
  std::cout << "\n--- Edit Options ---\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  std::cout << "1. Append to the end of the file\n";
  std::cout << "2. Replace an entire line\n";
  std::cout << "3. Delete a specific line\n";
  std::cout << "4. Insert a new line\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
  std::cout << "Enter your choice: ";
  int editChoice;
  std::cin >> editChoice;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  system("cls");

  std::vector<std::string> lines;
  std::ifstream currentFile(filename);
  std::string line;
  while (std::getline(currentFile, line)) {
    lines.push_back(line);
  }
  currentFile.close();

  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << "\n--- Current File Content ---\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  for (size_t i = 0; i < lines.size(); ++i) {
    std::cout << i + 1 << ": " << lines[i] << '\n';
  }
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << "---------------------------\n";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

  switch (editChoice) {
  case 1: // Append
  {
    std::ofstream outputFile(filename,
                             std::ios::app); // Opens file in append mode.
    if (!outputFile.is_open()) {
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Error: Could not open file for appending.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      system("pause");
      return;
    }
    std::cout << "\nEnter new content to append. ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    std::cout << "Press Ctrl+D (Unix/Linux) or Ctrl+Z/F6 (Windows) followed by "
                 "Enter to save";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    std::cout << ".\n";
    outputFile << std::cin.rdbuf();
    outputFile.close();
    std::cin.clear();
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "File '";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    std::cout << filename;
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Invalid line number. No changes made.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      break;
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Invalid line number. No changes made.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      break;
    }
    lines.erase(lines.begin() + lineToDelete - 1);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Invalid line number. No changes made.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      break;
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    std::cout << "Enter new content to insert:\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    std::string newLineContent;
    std::getline(std::cin, newLineContent);
    lines.insert(lines.begin() + lineToInsert - 1, newLineContent);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "Line inserted.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    break;
  }
  default: {
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    std::cerr << "Invalid choice. No changes made.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
    system("pause");
    return;
  }
  }

  // Writes the modified content back to the file.
  if (editChoice == 2 || editChoice == 3 || editChoice == 4) {
    std::ofstream outputFile(
        filename, std::ios::trunc); // Overwrites the file with the new content.
    if (!outputFile.is_open()) {
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Error: Could not open file for writing.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      system("pause");
      return;
    }
    for (const auto &l : lines) {
      outputFile << l << '\n';
    }
    outputFile.close();
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "File '";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    std::cout << filename;
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    std::cout << "' updated successfully.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
  }

  system("pause");
}

// Function to delete a file after user confirmation.
void deleteFile() {
  system("cls");
  std::string filename = getValidatedFilename();

  SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
  std::cout << "Are you sure you want to delete '";
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << filename;
  SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
  std::cout << "'? (y/n): ";
  SetConsoleTextAttribute(hConsole,
                          FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
  char confirmation;
  std::cin >> confirmation;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (confirmation == 'y' || confirmation == 'Y') {
    std::error_code ec;
    if (std::filesystem::remove(filename, ec)) {
      SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
      std::cout << "File '";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
      std::cout << filename;
      SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
      std::cout << "' deleted successfully.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    } else {
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Error: Could not delete file. " << ec.message()
                << std::endl;
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
    }
  } else {
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    std::cout << "Deletion cancelled.\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                          FOREGROUND_GREEN);
  }
  system("pause");
}

// Main function that displays the menu and handles user choices.
int main() {
  int choice;
  do {
    system("cls");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Invalid input. Please enter a number.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      system("pause");
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
      SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
      std::cout << "Exiting program. Goodbye!\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      break;
    default:
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
      std::cerr << "Invalid choice. Please enter a number between 1 and 7.\n";
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
                                            FOREGROUND_GREEN);
      system("pause");
      break;
    }
  } while (choice != 7);
  system("pause");

  return 0;
}