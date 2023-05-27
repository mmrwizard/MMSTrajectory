#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip> // for std::setprecision and std::fixed

int main(int argc, char* argv[]) {
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " input.txt output.txt eastingOffset northingOffset hEllOffset" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile) {
        std::cerr << "Could not open input file." << std::endl;
        return 1;
    }

    std::ofstream outfile(argv[2]);
    if (!outfile) {
        std::cerr << "Could not open output file." << std::endl;
        return 1;
    }

    // Parse offsets
    double eastingOffset = std::stod(argv[3]);
    double northingOffset = std::stod(argv[4]);
    double hEllOffset = std::stod(argv[5]);

    std::string line;
    std::vector<int> colIndices;
    std::vector<std::string> colNames;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string word;
        int col = 0;

        if (colIndices.empty()) { // find the indices
            while (iss >> word) {
                if (word == "GPSTime" || word == "Easting" || word == "Northing" || word == "H-Ell") {
                    colIndices.push_back(col);
                    colNames.push_back(word);
                }
                col++;
            }
        }
        else { // retrieve the data
            std::vector<double> rowData(colIndices.size(), 0.0);
            bool invalidRow = false;
            while (iss >> word) {
                for (std::size_t i = 0; i < colIndices.size(); i++) {
                    if (col == colIndices[i]) {
                        try {
                            double value = std::stod(word);
                            // Apply offset
                            if (colNames[i] == "Easting") value -= eastingOffset;
                            else if (colNames[i] == "Northing") value -= northingOffset;
                            else if (colNames[i] == "H-El") value -= hEllOffset;

                            rowData[i] = value;
                        }
                        catch (std::invalid_argument& e) {
                            invalidRow = true;
                            break;
                        }
                        break;
                    }
                }
                if (invalidRow) {
                    break;
                }
                col++;
            }
            if (invalidRow) {
                continue;
            }
            // output to file
            for (std::size_t i = 0; i < rowData.size(); i++) {
                outfile << std::setprecision(10) << std::fixed << rowData[i]; // set precision here
                if (i != rowData.size() - 1) {
                    outfile << "\t";
                }
                else {
                    outfile << "\n";
                }
            }
        }
    }
    infile.close();
    outfile.close();
    return 0;
}
