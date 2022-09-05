#include "reflection.h"

#include <iostream>
using namespace std;
using namespace PRefl;

template<class T1, class T2>
ostream& operator<<(ostream& out, const std::pair<T1, T2>& p)
{
    out << "(" << p.first << "," << p.second << ")";
    return out;
}

template<class T>
struct Vec
{
    T x;
    T y;

    Vec(T x_, T y_) : x(x_), y(y_) {}
};

template<typename T>
struct ReflData<Vec<T>>
{
    constexpr static bool hasData = true;
    constexpr static bool hasBases = false;
    constexpr static auto fields = FieldArray {
        Field{ Name<"x">{}, &Vec<T>::x, AttrArray{ Attribute{ Name<"meta">{} } } },
        Field{ Name<"y">{}, &Vec<T>::y, AttrArray{ Attribute{ Name<"meta">{} }, Attribute{ Name<"range">{}, std::make_pair(-1, 1) } } }
    };
};

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
    auto& value = field.GetField(obj);
    cout << "field: " << field.name.data << " " << value << "(" << typeid(value).name() <<")" << "\n";
    if constexpr (field.hasAttributes)
    {
        field.attributes.ForEach(print_attrs);
    }
    value = static_cast<std::remove_reference_t<decltype(value)>>(rand());
};

int main()
{
    Vec<float> vec{ 1.f, 2.f };
    // vec.x = 5.f;
    // vec.y = -1.f;
    auto refl = Refl{vec};
    refl.ForEachFields(print_field);
    refl.ForEachFields(print_field);
    
    return 0;
}