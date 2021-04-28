#include <string>

class NamePattern
{
public:
    NamePattern();
    bool isValid() const;
    std::string getName() const;
    unsigned getOffset() const { return offset; }
    unsigned getIdx() const { return idx; }
    void next() { idx++; }
    void bumpOffset() { offset++; }
    void setIdx(unsigned new_idx) { idx = new_idx; }
    static NamePattern parse(const std::string & filename);
private:
    bool initWithPattern(const std::string &filename, std::string::size_type pos);
    bool initWithNumber(const std::string &filename);
private:
    std::string pattern;
    unsigned offset;
    unsigned idx;
    bool valid;
};
