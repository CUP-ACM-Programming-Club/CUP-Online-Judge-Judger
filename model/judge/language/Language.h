//
// Created by Haoyuan Li on 2019/12/24.
//

#ifndef JUDGE_CLIENT_LANGUAGE_H
#define JUDGE_CLIENT_LANGUAGE_H
#define extlang extern "C" Language*
#define deslang extern "C" void
#define HOJ_MAX_LIMIT (-1)
const int call_array_size = 512;
#include <vector>
#include <string>
#include <sys/resource.h>
class Language {
public:
    virtual void run(int memory) = 0;
    virtual void setProcessLimit();
    virtual void setCompileProcessLimit();
    virtual void compile(std::vector<std::string>&, const char*, const char*);
    virtual void buildRuntime(const char* work_dir);
    virtual double buildTimeLimit(double timeLimit, double bonus);
    virtual int buildMemoryLimit(int memoryLimit, int bonus);
    virtual void setExtraPolicy(const char* oj_home, const char* work_dir);
    virtual void initCallCounter(int* call_counter) = 0;
    virtual void setCompileExtraConfig();
    virtual void setCompileMount(const char* work_dir);
    virtual int getCompileResult(int status);
    virtual int fixACStatus(int acFlag);
    virtual int getMemory(rusage ruse, pid_t pid);
    virtual std::string getFileSuffix() = 0;
    virtual ~Language();
protected:
    virtual void setCPULimit();
    virtual void setFSizeLimit();
    virtual void setASLimit();
    virtual void setAlarm();
};

typedef Language* createLanguageInstance();

typedef void destroyLanguageInstance(Language*);


#endif //JUDGE_CLIENT_LANGUAGE_H
