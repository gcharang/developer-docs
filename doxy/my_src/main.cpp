/*
 * main.cpp
 * Bryan Beus
 * Komodo Platform
 * April 5, 2020
 */

/*******************************************************************************
  * This script takes the files previously output by the Doxybook2 engine
  * and reassembles them so that files that are related to each other are
  * compacted into one file.
  *
  * This keeps the list of contents in Komodo's documentation sidebar in a more
  * user friendly and readable format.
  * 
  * The script then outputs the new files into the
  * /docs/basic-docs/antara-gaming-sdk/ directory, and updates the 
  * sidebar.js file to reflect the new sidebar organization.
*******************************************************************************/

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <string> 
#include <algorithm>
#include <iterator> 
#include <cstddef>

#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/JsonConverter.hpp>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/TextPlainPrinter.hpp>
#include <Doxybook/Config.hpp>
#include <Doxybook/Enums.hpp>
#include <Doxybook/Generator.hpp>
#include <Doxybook/Log.hpp>
#include <Doxybook/Node.hpp>
#include <Doxybook/Path.hpp>
#include <Doxybook/Renderer.hpp>
#include <Doxybook/Template.hpp>
#include <Doxybook/TemplateLoader.hpp>
#include <Doxybook/TemplateDefaultLoader.hpp>
#include <Doxybook/Utils.hpp>
#include <Doxybook/Xml.hpp>
#include <Doxybook/XmlTextParser.hpp>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::count;

using namespace Doxybook2;

// A custom function to recurisvely create a list of refids for the source code
nlohmann::json createIndex(const Node& parent) {

    // Create a default result{} object
    nlohmann::json result{};


    // Iterate through each child of the parent node and
    // create a heirarchy of all children with applicable types
    for (const auto& child : parent.getChildren()) {
    if (child->getKind() == Kind::NAMESPACE) {
        auto test = createIndex(*child);
            if (!test.empty()) {
            result[child->getName()] = test;
            }
        } else if (child->getKind() == Kind::CLASS || child->getKind() == Kind::STRUCT){
            result[child->getName()] = child->getRefid();
        }
    }

    // Return the resulting heirarchy
    return result;
}

// A custom function to recurisvely create a list of refids for the source code
void createSidebarJSFile(std::ofstream& file, const vector<string> fileNames) {

    string test;

    // Create the file structure to track each file in the sidebar
    for (auto name : fileNames) {

        file << string(4, ' ') << "[" << endl;
        file
            << string(6, ' ') 
            << "\"" 
            << "/basic-docs/antara-gaming-sdk/"
            << name
            << ".md"
            << "\"," << endl;
        file
            << string(6, ' ') 
            << "\"" 
            << name
            << "\"," << endl;
        file 
            << string(4, ' ') << "]," << endl; 
    }

} 

// Collect all data from the provided Node& object and compile the data into more compact file structures
void createNamespaceFiles(ofstream& file, int hashCount, const Node& parent, vector<string>& fileNames) {
    // Discover number of children objects belonging to the parent
    int numChildren = (int)parent.getChildren().size();

    // Initiate a bool value to see if any of the children have a type
    // that would require further compilation
    // If this remains false, this parent node will not call further children nodes recursively
    bool childHasValidType = false;

    // Test each child to see if there is a valid child node requiring further recursion
    if (numChildren > 0) { 
        for (const auto& child : parent.getChildren()) {
            if (child->getKind() == Kind::NAMESPACE || child->getKind() == Kind::STRUCT || child->getKind() == Kind::CLASS) {
                // If the parent node has children with types that match any of the above
                // switch the childHasValidType bool to true
                childHasValidType = true;
            }
        }
    }

    // Capture the parent name
    string parentName = parent.getName();

    // If there are no further valid children, end this recursion
    if (!childHasValidType) {
        return;

        // Otherwise, test if this parent node is either one of the initial
        // nodes in the index that may have further valid children, or
        // if it is a node that has children further down in the tree that are valid
        // If so, initiate another recursion on those child nodes
        // Once finished, end this recursion to prevent printing these files to the 
        // output
    } else if (parentName == "antara::gaming" || parentName.find("antara::gaming") == std::string::npos) {

        // For the children with valid types, initiate recursive loop for all valid children
        for (auto child : parent.getChildren()) {
            createNamespaceFiles(file, hashCount, *child, fileNames);
        }

        // End the loop 
        return;
    }


    // Test if there are any invalid characters in the current node
    // If there are, end this recursive loop as these types of files are not needed
    // and also will cause breaking in the Vuepress engine
    if (parentName.find('@') != std::string::npos) {
        return;
    }

    // Initiate variables to track progress of recursion and file creation
    string parentKind = Doxybook2::toStr(parent.getKind()); 
    string filename = "";
    int iteration = 0;

    // Initiate for loop to create file out of data from each child
    for (const auto& child : parent.getChildren()) {

        // Determine if we are on the main antara::gaming::CLASS level
        // or if we are on a lower level
        // If on main level, create new file stream and save all values to it
        // If lower, do not create new file stream, but save all values to existing filestream 
        if (count(parentName.begin(), parentName.end(), ':') == 4) {

            // Create the filename for the output stream and save it to the fileNames vector
            size_t lastInstance = parentName.find_last_of(':');

            // Save only the end of the name, for brevity and formatting clarity
            string tempFilename = "";
            tempFilename = parentName.substr((int)(++lastInstance), (int)parentName.size());

            // Create a temporary filename string out of the current iteration
            string prefix = "../../../docs/basic-docs/antara-gaming-sdk/";
            tempFilename = prefix + tempFilename + ".md";

            // Test whether the filename variable provided to this function as a parameter is
            // different from the temporary filename
            if (tempFilename != filename) {

                // If they are different, set the filename to the new value
                filename = tempFilename;

                // Create the new output stream
                ofstream newFout(filename);

                if (!newFout) {
                    cout << "Error creating output file" << endl;
                    exit(0);
                }

                // Set the main "file" stream to the new output stream
                file = std::move(newFout); 

                // Create a name for the fileNames vector and store it
                string createNewName = "";
                lastInstance = filename.find_last_of('/');
                size_t mdLocation = filename.find(".md");
                createNewName = filename.substr(lastInstance + 1, mdLocation - lastInstance - 1);
                fileNames.push_back(createNewName);
            }

        }

        // Save content from relevant file to the current output stream 

        // Test first to make sure there are no invalid characters
        // If an invalid character is found, skip this child
        string childName = child->getName();
        if (childName.find('@') != std::string::npos || childName == "@") continue;

        // Capture the Kind name for this child
        string childKindStr;
        switch (child->getKind()) {
            case Kind::NAMESPACE:
                childKindStr = "Namespaces/";
                break;
            case Kind::CLASS:
                childKindStr = "Classes/";
                break;
            case Kind::STRUCT:
                childKindStr = "Classes/";
                break;
            default:
                // If this is not the type of node we are searching for
                // End this cycle of the for() loop so as not to print
                // to the output stream content that is not relevant
                continue;
        }

        // Read in content from existing file
        // TODO: (?) Need an input directory declared somewhere in a config file...
        string fileLocation = "../../outputDir/";
        string fileUrl = child->getRefid();
        fileLocation = fileLocation + childKindStr + fileUrl + ".md";
        ifstream fin(fileLocation); 

        if (!fin) {
            cout << "Error reading input file: " << fileLocation << endl;
            exit(0);
        }

        // Note the name of the file from which everything is drawn
        // (Active only when debugging
        // if (iteration > 0) {
            // file << "<!--" << endl;
            // file << "  New File: " << fileLocation << endl;
            // file << "  Topic name: " << child->getName() << endl;
            // file << "  Hash count: " << hashCount << endl;
            // file << "-->" << endl;
        // }
        iteration++;

        // Write all content to the output file
        string inputLine;
        while (true) {
            getline(fin, inputLine);

            file << inputLine << endl;

            if (fin.eof()) {
                break;
            }
        } 
        file << "\n" << endl;

        // Capture the index for the children of this child
        auto test = createIndex(*child);

        // If there is a valid child node for this child, initiate another recursive loop
        if (!test.empty() && (child->getKind() == Kind::NAMESPACE || child->getKind() == Kind::CLASS || child->getKind() == Kind::STRUCT || child->getKind() == Kind::INDEX)) { 
            createNamespaceFiles(file, ++hashCount, *child, fileNames);
        } 
    }
}

int main()
{
    using namespace Doxybook2;

    // Indicate the location of the XML files 
    std::string inputDir = "../../submodules/antara-gaming-sdk/build/docs/doxygen/xml";

    // Create basic configuration settings
    Config config;
    config.baseUrl = "/";
    config.copyImages = false;

    // Setup new Doxygen object
    Doxygen doxygen(config);

    // Load Plain and Markdown Printers
    // (These are not used in this script)
    TextPlainPrinter plainPrinter(config, doxygen);
    TextMarkdownPrinter markdownPrinter(config, inputDir, doxygen);


    // Load and parse the XML files, may take few seconds
    doxygen.load(inputDir);
    doxygen.finalize(plainPrinter, markdownPrinter);

    // Get the index, this holds hierarchical data.
    // If a class belongs to a namespace, the index will hold the namespace object,
    // but the namespace object will hold the class, not the index.
    const Node& index = doxygen.getIndex();

    // Create the output filename for the gaming-skd-sidebar.js file
    // This file is a sub-file of the full sidebar.js file, which is responsible
    // for organizing the sidebar directory
    string outputFilename = "../../../docs/.vuepress/gaming-sdk-sidebar.js";

    // Create an ofstream with this output file
    // This is not actually used, and serves only to initiate the below recursive function
    ofstream fout(outputFilename);

    if (!fout) {
        cout << "Error opening file" << endl;
        exit(0);
    }

    // Create a vector<string> object to hold all the titles/names of the files
    // This is used later to create the above gaming-sdk-sidebar.js sub-file
    vector<string> fileNames;

    // Create the namespace files
    // (This function creates the compacted sub-files)
    // (Pass in the outputFilename-based fout object for now
    // it is replaced later in the function, and therefore here this fout
    // only serves to start the recursive loop)
    createNamespaceFiles(fout, 0, index, fileNames);

    // Close ofstream
    fout.close();

    // Create a new file stream to create the sidebar file
    ofstream fout2(outputFilename);

    if (!fout2) {
        cout << "Error opening file" << endl;
        exit(0);
    }

    // Create Sidebar Navigation File
    // Initiate lines here setup the overall structure for the file
    // while the createSidebarJSFile() handles all the recursive and repeatable elements
    fout2 << "var gamingSidebar = {" << endl;
    fout2 << "  title: \"Antara Gaming SDK\"," << endl;
    fout2 << "  collapsible: true," << endl;
    fout2 << "  children: [" << endl; 

        createSidebarJSFile(fout2, fileNames);

    fout2 << " ]" << endl;
    fout2 << "};" << endl;
    fout2 << "module.exports = gamingSidebar;";


    return 0;
}
