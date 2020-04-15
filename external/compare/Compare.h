//
// Created by Haoyuan Li on 2020/3/26.
//

#ifndef COMPARE_COMPARE_H
#define COMPARE_COMPARE_H

namespace Compare {

    class Compare {
    public:
        Compare() : DEBUG(0), full_diff(1) {};

        virtual int compare(const char *stdAnswerFile, const char *userOutputFile) = 0;

        virtual void setDebug(int debug);

        virtual int getDebug();

        virtual void setFullDiff(int fullDiff);

        virtual int getFullDiff();

        virtual ~Compare() = default;

    protected:
        int DEBUG;
        int full_diff;
    };

    typedef Compare *createInstance();

    typedef void destroyInstancePointer(Compare *);
}
#endif //COMPARE_COMPARE_H
