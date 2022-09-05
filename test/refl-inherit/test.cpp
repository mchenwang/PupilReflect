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

struct A { int a; };
struct B : A { int b; };
template<typename T> struct C { T c; };
struct D : C<float>, B { int d; };

template<>
struct ReflData<A>
{
    constexpr static bool hasData = true;
    constexpr static bool hasBases = false;
    constexpr static auto fields = FieldArray {
        Field { Name<"a">{}, &A::a, AttrArray {} }
    };
};
template<>
struct ReflData<B>
{
    constexpr static bool hasData = true;
    constexpr static bool hasBases = true;
    constexpr static auto bases = ReflDataArray {
        ReflData<A> {}
    };
    constexpr static auto fields = FieldArray {
        Field { Name<"b">{}, &B::b, AttrArray {} }
    };
};
template<typename T>
struct ReflData<C<T>>
{
    constexpr static bool hasData = true;
    constexpr static bool hasBases = false;
    constexpr static auto fields = FieldArray {
        Field { Name<"c">{}, &C<T>::c, AttrArray {} }
    };
};
template<>
struct ReflData<D>
{
    constexpr static bool hasData = true;
    constexpr static bool hasBases = true;
    constexpr static auto bases = ReflDataArray {
        ReflData<C<float>> {},
        ReflData<B> {}
    };
    constexpr static auto fields = FieldArray {
        Field { Name<"d">{}, &D::d, AttrArray {} }
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
    auto test_a = A{1};
    auto test_b = B{5, 2};
    auto test_c = C{0.5};
    auto test_d = D{0.5f, test_b, 1};

    auto re = Refl{test_d};
    // re.ForEachFields(print_field);
    re.RecursiveForEachFields(print_field);

    return 0;
}