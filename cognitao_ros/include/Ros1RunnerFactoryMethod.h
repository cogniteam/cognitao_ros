#include <CogniTao.h>

class Ros1RunnerFactoryMethod : public RunnerFactoryMethod
{
public:
    Ros1RunnerFactoryMethod() {}
    virtual ~Ros1RunnerFactoryMethod() {}

    virtual Runner *createRunner(
        std::string action,
        BehaviourTask *t)
    {

        Runner *tRet = nullptr;
        tRet = new Ros1Runner(action, t->getParameters());
        cout << "USER RUNNER CREATED" << endl;
        return tRet;
    }
};