//
// Created by eleme on 2019-08-16.
//

#include "CompilerConfigReader.h"

CompilerConfigReader::CompilerConfigReader() {

}

CompilerConfigReader::CompilerConfigReader(string filePath) {

}

CompilerConfigReader::CompilerConfigReader(JSONVectorReader reader) {

}

vector<string> CompilerConfigReader::getCompilerArgs() {
    return vector<string>();
}

vector<string> CompilerConfigReader::getCompilerArgs(LanguageConstants language) {
    return vector<string>();
}

void CompilerConfigReader::setLanguage(LanguageConstants language) {
    this->language = language;
}

LanguageConstants CompilerConfigReader::getLanguage() {
    return language;
}

void CompilerConfigReader::clear() {

}
