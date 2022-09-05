#include "reflection.h"
#include "test2.h"

#include <iostream>
using namespace std;

template<class T1, class T2>
ostream& operator<<(ostream& out, const std::pair<T1, T2>& p)
{
    out << "(" << p.first << "," << p.second << ")";
    return out;
}
template<class T>
ostream& operator<<(ostream& out, const Data<T>& p)
{
    out << "(" << p.x << "," << p.y << ")";
    return out;
}

auto print_attrs = [] (const auto& attr)
{
    cout << "   attr: " << attr.name.data;
    if constexpr (attr.hasValue)
    {
        cout << " " << attr.value;
    }
    cout << "\n";
};

auto print_field = [](auto& obj, const auto& field)
{
    cout << typeid(obj).name() << "\n";
    auto value = field.GetField(obj);
    if constexpr (std::is_pointer_v<decltype(value)>){
        cout << "field: " << field.name.data << " " << *value << "(" << typeid(*value).name() <<")" << "\n";
    }
    else
        cout << "field: " << field.name.data << " " << value << "(" << typeid(value).name() <<")" << "\n";
    if constexpr (field.hasAttributes)
    {
        field.attributes.ForEach(print_attrs);
    }
};

int main()
{
    std::cout << "Test Class ================\n";
    {
        TestClass c{5};
        auto re = PRefl::Refl{c};
        re.ForEachFields(print_field);
    }

    std::cout << "Test Class2 ================\n";
    {
        Data<float> d{0.1f, 0.2f};
        TestNsp::TestClass2<float> test2;
        test2.a = 10;
        test2.b = 3;
        test2.data = &d;
        auto re = PRefl::Refl{test2};
        re.RecursiveForEachFields(print_field);
    }
}