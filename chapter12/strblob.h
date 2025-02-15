#include <memory>
#include <vector>
#include <stdexcept>
#include <string>
#include <new>

class StrBlobPtr;

class StrBlob
{
    friend class StrBlobPtr;

    public:
        typedef std::vector<std::string>::size_type size_type;
        StrBlob();
        StrBlob(std::initializer_list<std::string> il);
        size_type size() const
        {
            return data->size();
        }
        bool empty() const
        {
            return data->empty();
        }
        void push_back(const std::string& t)
        {
            data->push_back(t);
        }
        void pop_back();
        std::string& front();
        std::string& back();
        StrBlobPtr begin();
        StrBlobPtr end();
    private:
        std::shared_ptr<std::vector<std::string>> data;
        void check(size_type i, const std::string& msg) const;
};

void StrBlob::check(StrBlob::size_type i, const std::string& msg) const
{
    if(i >= StrBlob::data->size())
    {
        throw std::out_of_range(msg);
    }
}

void StrBlob::pop_back()
{
    StrBlob::check(0, "pop back on empty StrBlob");
    StrBlob::data->pop_back();
}

std::string& StrBlob::front()
{
    StrBlob::check(0, "front on empty StrBlob");
    return StrBlob::data->front();
}

std::string& StrBlob::back()
{
    StrBlob::check(0, "back on empty StrBlob");
    return StrBlob::data->back();
}

StrBlob::StrBlob(): data(std::make_shared<std::vector<std::string>>()){}
StrBlob::StrBlob(std::initializer_list<std::string> il): data(std::make_shared<std::vector<std::string>>(il)){}


class StrBlobPtr
{
    public:
        StrBlobPtr(): curr(0){}
        StrBlobPtr(StrBlob& a, std::size_t sz = 0): wptr(a.data), curr(sz){}
        std::string& deref() const;
        StrBlobPtr& incr();
    private:
        std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string&) const;
        std::weak_ptr<std::vector<std::string>> wptr;
        std::size_t curr;
};

std::shared_ptr<std::vector<std::string>> 
StrBlobPtr::check(std::size_t i, const std::string& msg) const
{
    auto ret = wptr.lock();
    if(!ret)
    {
        throw std::runtime_error("unbound StrBlobPtr");
    }
    if(i >= ret->size())
    {
        throw std::out_of_range(msg);
    }
    return ret;
}

std::string& 
StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

StrBlobPtr& 
StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()
{
    auto ret = StrBlobPtr(*this, data->size());
    return ret;
}