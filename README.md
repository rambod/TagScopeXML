# TagScope XML

**TagScope XML** is an open-source tool for analyzing and editing XML files. It provides a clear overview of an XML file’s structure by summarizing tag counts, displaying detailed tag information (including child elements and values), and offering an in-app editor to modify the XML content. Designed with a clean, dark-themed interface, TagScope XML is ideal for developers and anyone looking to gain insights into their XML data.

## Features

- **XML Analysis:**  
  - Loads XML files and computes overall statistics such as total elements, unique tags, total attributes, and the most frequent tag.
  - Displays a sorted tag frequency table for quick insights.

- **Tag Details:**  
  - Select a tag from the list to view detailed information.
  - View an HTML table of immediate child elements and their values for the selected tag.
  - Filter tag details by input to quickly locate specific values.

- **XML Editing:**  
  - Edit XML content directly within the integrated text editor.
  - Save modifications back to the original file or “Save As…” to a new file.
  - Reload the XML to refresh statistics.

- **User-Friendly Interface:**  
  - A clear split-view layout with a tag list on the left and a tabbed panel on the right.
  - “Show Full Summary” button to easily return to the overall XML summary.
  
## Installation

### Prerequisites

- **Qt 6.8.1** (or later) – available from [Qt Downloads](https://www.qt.io/download)
- **CMake 3.30+** – for building the project

### Building the Project

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/yourusername/TagScopeXML.git
   cd TagScopeXML
   ```

2. **Configure the Project:**

   Create a build directory and run CMake:

   ```bash
   mkdir build && cd build
   cmake ..
   ```

3. **Build the Project:**

   ```bash
   cmake --build .
   ```

4. **Run the Application:**

   On Linux/macOS:

   ```bash
   ./TagScopeXML
   ```

   On Windows:

   ```bash
   TagScopeXML.exe
   ```

## Usage

1. **Open XML File:**  
   Click the **Open XML File** button to load an XML file. The file’s content is loaded into the editor and analyzed for tag statistics.

2. **View Summary:**  
   The **Summary** tab displays overall XML statistics and a tag frequency table. Use the **Show Full Summary** button to restore this view if needed.

3. **Inspect Tag Details:**  
   Select a tag from the list on the left to view its details. The **Tag Details** tab will display an HTML table listing the immediate child elements (and their values) of the first occurrence of the tag. You can filter these details using the filter box.

4. **Edit XML:**  
   Use the **Editor** tab to modify the XML content. When finished, save your changes using the provided toolbar actions.

## Contributing

Contributions are welcome! To contribute:

1. Fork this repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and test thoroughly.
4. Submit a pull request with a clear description of your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For questions, suggestions, or feedback, please open an issue in this repository or contact me via [rambod.net](https://rambod.net).

---

*TagScope XML* offers a practical, user-friendly approach to XML analysis and editing. Enjoy exploring and enhancing your XML data!
