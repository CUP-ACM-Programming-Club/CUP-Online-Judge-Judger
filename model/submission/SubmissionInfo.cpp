//
// Created by Haoyuan Li on 2019/12/19.
//

#include "SubmissionInfo.h"
#include "../base/JSONVectorReader.h"

SubmissionInfo &SubmissionInfo::setSource(string &source) {
    this->source = source;
    return *this;
}

SubmissionInfo &SubmissionInfo::setCustomInput(string &customInput) {
    this->customInput = customInput;
    return *this;
}

SubmissionInfo &SubmissionInfo::setTimeLimit(double timeLimit) {
    this->timeLimit = timeLimit;
    return *this;
}

SubmissionInfo &SubmissionInfo::setMemoryLimit(double memoryLimit) {
    this->memoryLimit = memoryLimit;
    return *this;
}

SubmissionInfo &SubmissionInfo::setSpecialJudge(bool specialJudge) {
    this->specialJudge = specialJudge;
    return *this;
}

SubmissionInfo &SubmissionInfo::setProblemId(int problemId) {
    this->problemId = problemId;
    return *this;
}

SubmissionInfo &SubmissionInfo::setUserId(string &userId) {
    this->userId = userId;
    return *this;
}

SubmissionInfo &SubmissionInfo::setLanguage(int language) {
    this->language = language;
    return *this;
}

string &SubmissionInfo::getSource() {
    return source;
}

string &SubmissionInfo::getCustomInput() {
    return customInput;
}

double SubmissionInfo::getTimeLimit() {
    return timeLimit;
}

double SubmissionInfo::getMemoryLimit() {
    return memoryLimit;
}

bool SubmissionInfo::getSpecialJudge() {
    return specialJudge;
}

int SubmissionInfo::getProblemId() {
    return problemId;
}

string &SubmissionInfo::getUserId() {
    return userId;
}

int SubmissionInfo::getLanguage() {
    return language;
}

SubmissionInfo &SubmissionInfo::readFromFile(string &file) {
    JSONVectorReader reader(file);
    if (reader.has("test_run") && reader.GetBool("test_run")) {
        this->customInput = reader.GetString("custom_input");
    }
    this->language = reader.GetInt("language");
    this->userId = reader.GetString("user_id");
    this->problemId = reader.GetInt("problem_id");
    this->specialJudge = reader.has("spj") && reader.GetBool("spj");
    this->memoryLimit = reader.GetDouble("memory_limit");
    this->timeLimit = reader.GetDouble("time_limit");
    this->source = reader.GetString("source");
    this->solutionId = reader.GetInt("solution_id");
    return *this;
}

int SubmissionInfo::getSolutionId() {
    return solutionId;
}

SubmissionInfo &SubmissionInfo::setSolutionId(int solutionId) {
    this->solutionId = solutionId;
}

