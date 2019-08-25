#include <iostream>
#include <sstream>

#include <Snow Owl/Core/DNDObject.hxx>

using namespace std;
using namespace snowowl;

class SampleClass;

DEFINE_ENCODABLE(SampleClass, string)
DEFINE_DECODABLE(SampleClass, string)

class SampleClass : public Object<SampleClass, std::string> {

  DEFINE_CLASS(SampleClass, std::string) { }

public:

    int sample;
};

IMPLEMENT_ENCODABLE(SampleClass, string) {

    stringstream stream;

    stream << "value is " << input->sample;

    return stream.str();
}

IMPLEMENT_DECODABLE(SampleClass, string) {

    target.sample = 100;
}

int main()
{
    SampleClass input("hello world!");

    auto output = input.encode();

    std::cout << "Hello World!\n" << output;
}
