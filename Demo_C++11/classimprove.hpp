#ifndef CLASSIMPROVE_HPP_INCLUDED
#define CLASSIMPROVE_HPP_INCLUDED

#include <string>
#include <iostream>
#include <memory>

#include <assert.h>

class Base
{
public:
    Base(int i_, long j_, std::string k_) : i(i_), j(j_), k(k_)
    {
    }

    Base(int i_) : Base(i_, 0, "")
    {
    }

    Base() = default;

    Base(const Base&) = default;
    Base& operator=(const Base&) = delete;

public:
    virtual void fun1()
    {
        std::cout << "Base::fun1" << std::endl;
    }

    virtual void fun2()
    {
        std::cout << "Base::fun2" << std::endl;
    }

private:
    int         i;
    long        j;
    std::string k;
};

/**Drive类禁止被继承*/
class Drive final : public Base
{
public:

public:
    void fun1() override   /**显式虚函数重载，基类必须有该虚函数声明*/
    {
        std::cout << "Drive::fun1" << std::endl;
    }

    virtual void fun2() final    /**禁止子类覆盖本函数*/
    {
        std::cout << "Drive::fun2" << std::endl;
    }

    explicit operator bool()
    {
        return false;
    }
};

void classimprove()
{
    Base b1;
    Base b2(1);
    auto b3 = b2;

    Drive d1;
    d1.fun1();
    d1.fun2();

    Drive *d2 = nullptr;
    if (!d2){
        std::cout << "d2 is null" << std::endl;
    }

    std::weak_ptr<Drive>   d_weak;
    assert(d_weak.expired());
    {
        std::shared_ptr<Drive> d_ptr(new Drive);
        d_weak = d_ptr;
        assert(!d_weak.expired());

        long i = static_cast<bool>(*d_ptr);
        if (*d_ptr){
            std::cout << " automatic Drive::operator bool" << std::endl;
        }
    }
    assert(d_weak.expired());
}
#endif // CLASSIMPROVE_HPP_INCLUDED
