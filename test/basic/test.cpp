#include "base.h"
#include "attribute.h"
#include "field.h"

#include <string_view>
#include <iostream>
using namespace std;
using namespace PRefl;

template<class T1, class T2>
ostream& operator<<(ostream& out, const std::pair<T1, T2>& p)
{
    out << "(" << p.first << "," << p.second << ")";
    return out;
}

void test_attrs()
{
    cout << "======================================= attr array\n";
    constexpr auto attr1 = Attribute{ Name<"attr1">{} };
    constexpr auto attr2 = Attribute{ Name<"attr2">{}, "meta" };
    constexpr auto attr3 = Attribute{ Name<"attr3">{}, std::make_pair(0, 1) };

    constexpr auto attrs = AttrArray{ attr1, attr2, attr3 };

    auto print_attrs = [] (const auto& attr)
    {
        cout << "attr: " << attr.name.data;
        if constexpr (attr.hasValue)
        {
            cout << " " << attr.value;
        }
        cout << "\n";
    };
    attrs.ForEach(print_attrs);
}

struct TestStruct
{
    int x;
    int y;
    int z;
};

void test_field()
{
    cout << "======================================= attr field\n";
    constexpr auto xf = Field{ Name<"x">{}, &TestStruct::x, AttrArray{ Attribute{ Name<"meta">{} } } };
    constexpr auto yf = Field{ Name<"y">{}, &TestStruct::y, AttrArray{ Attribute{ Name<"meta">{} }, Attribute{ Name<"range">{}, std::make_pair(-1, 1) } } };
    constexpr auto zf = Field{ Name<"z">{}, &TestStruct::z, AttrArray{ Attribute{ Name<"meta">{} }, Attribute{ Name<"range">{}, std::make_pair(0.f, 1.f) } } };

    constexpr auto fs = FieldArray{ xf, yf, zf};

    auto print_attrs = [] (const auto& attr)
    {
        cout << "   attr: " << attr.name.data;
        if constexpr (attr.hasValue)
        {
            cout << " " << attr.value;
        }
        cout << "\n";
    };

    auto print_field = [&print_attrs](const auto& field)
    {
        cout << "field: " << field.name.data << " " << field.value << "\n";
        if constexpr (field.hasAttributes)
        {
            field.attributes.ForEach(print_attrs);
        }
    };

    fs.ForEach(print_field);

    TestStruct test{ 1, 2, 3 };
    auto print_field_value = [&test](const auto& field)
    {
        cout << field.name.data << ": " << test.*(field.value) << "\n";
    };
    
    fs.ForEach(print_field_value);
}

void test_func()
{
    cout << "======================================= func\n";
    constexpr auto xf = Field{ Name<"x">{}, &TestStruct::x, AttrArray{ Attribute{ Name<"meta">{} } } };
    constexpr auto yf = Field{ Name<"y">{}, &TestStruct::y, AttrArray{ Attribute{ Name<"meta">{} }, Attribute{ Name<"range">{}, std::make_pair(-1, 1) } } };
    constexpr auto zf = Field{ Name<"z">{}, &TestStruct::z, AttrArray{ Attribute{ Name<"meta">{} }, Attribute{ Name<"range">{}, std::make_pair(0.f, 1.f) } } };

    constexpr auto fs = FieldArray{ xf, yf, zf};

    constexpr auto cmp = []<typename T>(const char* targetName, const T e)
    {
        // for ()
        return std::string_view{targetName} == std::string_view{e.name.data};
        // return targetName == e.name.data;
    };

    constexpr auto index1 = fs.FindIf("x", cmp);
    cout << "x: " << index1 << "\n";
    constexpr auto index2 = fs.FindIf("z", cmp);
    cout << "z: " << index2 << "\n";
    constexpr auto index3 = fs.FindIf("y", cmp);
    cout << "y: " << index3 << "\n";

    auto print_attrs = [] (const auto& attr)
    {
        cout << "   attr: " << attr.name.data;
        if constexpr (attr.hasValue)
        {
            cout << " " << attr.value;
        }
        cout << "\n";
    };

    auto print_field = [&print_attrs](const auto& field)
    {
        cout << "field: " << field.name.data << " " << field.value << "\n";
        if constexpr (field.hasAttributes)
        {
            field.attributes.ForEach(print_attrs);
        }
    };
    
    while (1)
    {
        std::string in;
        cin >> in;
        auto index4 = fs.FindIf(in.data(), cmp);
        if (index4 == -1) break;
        cout << index4;
        // print_field(f4);
    }

    // cout << index4;
    // auto f4 = fs.Get(index4);
    // print_field(f4);
}

int main()
{
    constexpr auto s = StringLiteral{"td"};
    cout << s.data << "\n";

    constexpr auto x = Name<"s">{};
    cout << x.view.data << "\n";
    constexpr auto fx = IsName<int>;
    constexpr auto f = IsName<decltype(x)>;

    constexpr auto test_NamedValue = NamedValue{ Name<"tse">{}, 0.8 };
    cout << test_NamedValue.name.data << " " << test_NamedValue.value << "\n";
    
    constexpr auto test_NamedValue2 = NamedValue{ Name<"tse2">{} };
    cout << test_NamedValue2.name.data << "\n";
    // cout << test_NamedValue2.name.data << " " << test_NamedValue2.value << "\n";

    constexpr auto test_Attr = Attribute{ Name<"attr">{}, std::make_pair(0.f, 1.f) };
    cout << test_Attr.name.data << " " << test_Attr.value << "\n";
    
    constexpr auto test_Attr2 = Attribute{ Name<"attr2">{} };
    cout << test_Attr2.name.data << "\n";
    // cout << test_Attr2.name.data << " " << test_Attr2.value << "\n";
    
    test_attrs();
    test_field();

    test_func();

    return 0;
}