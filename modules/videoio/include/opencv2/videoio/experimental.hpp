#ifndef EXPERIMENTAL_HPP
#define EXPERIMENTAL_HPP

#include "opencv2/core.hpp"
#include <map>
#include <utility>
#include <iostream>
#include <iomanip>

namespace cv { namespace experimental {

struct IntPair
{
    union { int a; int w; int num  ; };
    union { int b; int h; int denom; };

    static IntPair create(int a, int b) { IntPair res; res.a = a; res.b = b; return res; }
    bool operator==(const IntPair & other) const { return a == other.a && b == other.b; }
    double toFPS() const { return static_cast<double>(num) / static_cast<double>(denom);}
    std::pair<int, int> toPair() const { return std::make_pair(a, b); }
};

typedef IntPair FPSType;

//=============================

struct ByteQuad
{
    union { unsigned char a; unsigned char f; };
    union { unsigned char b; unsigned char o; };
    union { unsigned char c; unsigned char u; };
    union { unsigned char d; unsigned char r; };
};

typedef ByteQuad FourCCType;

//=============================

struct PMV
{
    enum { ReservedSize = 56 };
    enum Tag
    {
        PMV_None = -1,
        PMV_Bool,
        PMV_Int,
        PMV_Double,
        PMV_IntPair,
        PMV_ByteQuad,
        PMV_String,
        PMV_MAX,
        PMV_Reserved = 0xFFFFFFFF
    } t;
    union
    {
        bool valBool;
        int valInt;
        double valDouble;
        IntPair valIntPair;
        ByteQuad valByteQuad;
        const std::string * valString;
        unsigned char reserved[ReservedSize];
    } s;
    PMV() : t(PMV_None) {}
    PMV(bool val) : t(PMV_Bool) { s.valBool = val; }
    PMV(int val) : t(PMV_Int) { s.valInt = val; }
    PMV(double val) : t(PMV_Double) { s.valDouble = val; }
    PMV(int a, int b) : t(PMV_IntPair) { s.valIntPair.a = a; s.valIntPair.b = b; }
    PMV(const std::pair<int, int> & val) : t(PMV_IntPair) { s.valIntPair.a = val.first; s.valIntPair.b = val.second; }
    PMV(unsigned char a, unsigned char b, unsigned char c, unsigned char d) : t(PMV_ByteQuad) { s.valByteQuad.a = a; s.valByteQuad.b = b; s.valByteQuad.c = c; s.valByteQuad.d = d; }
    PMV(const char * val) : t(PMV_String) { s.valString = new std::string(val); }
    PMV(const std::string & val) : t(PMV_String) { s.valString = new std::string(val); }
    ~PMV() { cleanup(); }

    PMV(const PMV & other)
    {
        *this = other;
    }

    PMV & operator=(const PMV & other)
    {
        cleanup();
        t = other.t;
        s = other.s;
        copyValue();
        return *this;
    }

    operator bool() const { verifyType(PMV_Bool); return s.valBool; }
    operator int() const { verifyType(PMV_Int); return s.valInt; }
    operator double() const { verifyType(PMV_Double); return s.valDouble; }
    operator IntPair() const { verifyType(PMV_IntPair); return s.valIntPair; }
    operator ByteQuad() const { verifyType(PMV_ByteQuad); return s.valByteQuad; }
    operator std::string() const { verifyType(PMV_String); return *s.valString; }

private:
    void verifyType(Tag tag) const
    {
        if (t != tag)
        {
            std::cout << "Actual type " << getTypeName(t) << " != " << getTypeName(tag) << std::endl;
            throw std::bad_cast();
        }
    }
    void cleanup()
    {
        if (t == PMV_String && s.valString)
        {
            delete s.valString;
            s.valString = 0;
        }
    }
    void copyValue()
    {
        if (t == PMV_String && s.valString)
            s.valString = new std::string(*s.valString);
    }
    static const char * getTypeName(Tag t)
    {
        switch (t)
        {
        case PMV_Bool: return "Bool";
        case PMV_String: return "String";
        case PMV_Int: return "Int";
        case PMV_Double: return "Double";
        case PMV_IntPair: return "IntPair";
        case PMV_ByteQuad: return "ByteQuad";
        case PMV_None: return "None";
        case PMV_MAX:
        case PMV_Reserved:
            return "MAX";
        }
        return "Unknown";
    }

public:
    void print(std::ostream & out) const
    {
        out << getTypeName(t) << " [";
        switch (t)
        {
        case PMV_Bool: out << (s.valBool ? "true" : "false"); break;
        case PMV_String: out << *s.valString; break;
        case PMV_Int: out << s.valInt; break;
        case PMV_Double: out << s.valDouble; break;
        case PMV_IntPair: out << s.valIntPair.a << ", " << s.valIntPair.b; break;
        case PMV_ByteQuad: out << s.valByteQuad.a << ", " << s.valByteQuad.b << ", " << s.valByteQuad.c << ", " << s.valByteQuad.d; break;
        case PMV_None: out << "?"; break;
        case PMV_MAX:
        case PMV_Reserved:
            out << "MAX"; break;
        }
        out << "]";
    }
};

//=============================

template<PMV::Tag tag> struct tag_helper;
template<> struct tag_helper<PMV::PMV_Bool> { typedef bool value_type; };
template<> struct tag_helper<PMV::PMV_Int> { typedef int value_type; };
template<> struct tag_helper<PMV::PMV_Double> { typedef double value_type; };
template<> struct tag_helper<PMV::PMV_IntPair> { typedef IntPair value_type; };
template<> struct tag_helper<PMV::PMV_ByteQuad> { typedef ByteQuad value_type; };
template<> struct tag_helper<PMV::PMV_String> { typedef std::string value_type; };

//=============================

enum CapProp
{
    CP_Invalid = -1,
    CP_FPS,
    CP_Name,
    CP_Size,
    CP_Gstreamer_Sync,
    CP_FourCC,
    CP_MAX,
    CP_Reserved = 0xFFFFFFFF
};

//=============================

template<CapProp prop> struct prop_helper;
template<> struct prop_helper<CP_FPS> { static const PMV::Tag type_tag = PMV::PMV_IntPair; };
template<> struct prop_helper<CP_Name> { static const PMV::Tag type_tag = PMV::PMV_String; };
template<> struct prop_helper<CP_Size> { static const PMV::Tag type_tag = PMV::PMV_IntPair; };
template<> struct prop_helper<CP_Gstreamer_Sync> { static const PMV::Tag type_tag = PMV::PMV_Bool; };
template<> struct prop_helper<CP_FourCC> { static const PMV::Tag type_tag = PMV::PMV_ByteQuad; };

//=============================

struct PropMap
{
private:
    std::map<CapProp, PMV> storage;

public:
    template<CapProp prop, typename T = typename tag_helper<prop_helper<prop>::type_tag>::value_type>
    inline T get() const
    {
        std::map<CapProp, PMV>::const_iterator i = storage.find(prop);
        if (i != storage.end())
            return i->second;
        throw std::out_of_range("Property not found");
    }

    template<CapProp prop, typename T = typename tag_helper<prop_helper<prop>::type_tag>::value_type>
    inline void set(const T & val)
    {
        storage[prop] = val;
    }

    PMV getD(CapProp prop)
    {
        std::map<CapProp, PMV>::const_iterator i = storage.find(prop);
        if (i != storage.end())
            return i->second;
        throw std::out_of_range("Property not found");
    }

    void setD(CapProp prop, const PMV & val)
    {
        storage[prop] = val;
    }

    void print(std::ostream & out) const
    {
        out << "Property storage {" << std::endl;
        for (std::map<CapProp, PMV>::const_iterator i = storage.begin(); i != storage.end(); ++i)
        {
            out << std::setw(4) << i->first << " -> ";
            i->second.print(out);
            out << std::endl;
        }
        out << "}" << std::endl;
    }
};


}} // cv::experimental::


#endif // EXPERIMENTAL_HPP
