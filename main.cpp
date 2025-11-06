#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include "utils.hpp"
#include <iomanip>
#include <algorithm>
#include "HuffmanTree.hpp"

namespace fs = std::filesystem;

// Read "word code" pairs into vector
static error_type readHeader(const std::string& hdrPath, std::vector<std::pair<std::string,std::string>>& pairs) {

    std::ifstream in(hdrPath);

    if (!in)
        return UNABLE_TO_OPEN_FILE;

    pairs.clear();
    std::string line;

    while (std::getline(in, line)) {

        if (line.empty())
            continue;

        auto pos = line.find(' ');

        if (pos == std::string::npos)
            return ERR_TYPE_NOT_FOUND;

        std::string word = line.substr(0, pos);
        std::string code = line.substr(pos + 1);

        if (word.empty() || code.empty())
            return ERR_TYPE_NOT_FOUND;

        //header lines begin with a word, not a 0/1
        if (word[0] == '0' || word[0] == '1')
            return ERR_TYPE_NOT_FOUND;

        pairs.emplace_back(std::move(word), std::move(code));
    }

    return NO_ERROR;
}

int main(int argc, char* argv[]) {

    // Project 4
    if (argc != 3) {

        std::cerr << "Usage: " << argv[0] << " <header.hdr> <bits.code>\n";

        return 1;
    }

    const std::string dirName = "input_output";
    const std::string hdrNameArg  = argv[1]; //TheBells.hdr
    const std::string codeNameArg = argv[2]; //TheBells.code

    // Build full paths to inside folder
    const std::string hdrFullPath  = dirName + "/" + hdrNameArg;
    const std::string codeFullPath = dirName + "/" + codeNameArg;

    // Base name from the .hdr argument
    // "stem" removes their extension
    const std::string base = fs::path(hdrNameArg).stem().string();
    const std::string outTokensDecoded = dirName + "/" + base + ".tokens_decoded";

    if (error_type status; (status = directoryExists(dirName)) != NO_ERROR)
        exitOnError(status, dirName);

    if (error_type status; (status = regularFileExistsAndIsAvailable(hdrFullPath)) != NO_ERROR)
        exitOnError(status, hdrFullPath);

    if (error_type status; (status = regularFileExistsAndIsAvailable(codeFullPath)) != NO_ERROR)
        exitOnError(status, codeFullPath);

    if (error_type status; (status = canOpenForWriting(outTokensDecoded)) != NO_ERROR)
        exitOnError(status, outTokensDecoded);

    //Read header lines, (word, code) pairs
    std::vector<std::pair<std::string,std::string>> pairs;
    if (auto st = readHeader(hdrFullPath, pairs); st != NO_ERROR)
        exitOnError(st, hdrFullPath);

    //Build decoder Huffman tree from header
    HuffmanTree decoder;
    if (auto st = decoder.buildFromHeader(pairs); st != NO_ERROR)
        exitOnError(st, hdrFullPath);

    //Decode the .code file into <base>.tokens_decoded
    std::ifstream code_in(codeFullPath);
    if (!code_in) exitOnError(UNABLE_TO_OPEN_FILE, codeFullPath);

    std::ofstream out(outTokensDecoded);
    if (!out) exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, outTokensDecoded);

    if (auto st = decoder.decode(code_in, out); st != NO_ERROR)
        exitOnError(st, outTokensDecoded);

    return 0;
}
