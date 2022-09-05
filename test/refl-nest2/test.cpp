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
struct A
{
    T a;

    // [important]
    // 避免对模板类内声明的类使用反射
    // 因为需要明确类型 T，才能生成对应的反射信息
    struct InA
    {
        int ina;
    };
};

template<class T>
struct ReflData<A<T>>
{
    constexpr static bool hasData = true;
    constexpr static bool hasBases = false;
    constexpr static auto fields = FieldArray {
        Field{ Name<"a">{}, &A<T>::a, AttrArray{ Attribute{ Name<"meta">{} } } }
    };
};

template<>
struct ReflData<A<float>::InA>
{
    constexpr static bool hasData = true;
    constexpr static bool hasBases = false;
    constexpr static auto fields = FieldArray {
        Field{ Name<"ina">{}, &A<float>::InA::ina, AttrArray{ Attribute{ Name<"meta">{} } } }
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
    auto value = field.GetField(obj);
    cout << "field: " << field.name.data << " " << value << "(" << typeid(value).name() <<")" << "\n";

    if constexpr (field.hasAttributes)
    {
        field.attributes.ForEach(print_attrs);
    }
};

int main()
{
    // Vec<float> vec1{ 1.f, 2.f };
    // Vec<int> vec2{ 90, 45 };
    
    // Transform t{ vec1, vec2};
    // auto refl = Refl{t};
    // // refl.ForEachFields(print_field);
    // refl.RecursiveForEachFields(print_field);

    A<float>::InA x;
    x.ina = 5500;
    
    auto refl = Refl{x};
    refl.RecursiveForEachFields(print_field);

    return 0;
}