//
//  request.hpp
//  fibio
//
//  Created by Chen Xu on 14-3-13.
//  Copyright (c) 2014 0d0a.com. All rights reserved.
//

#ifndef fibio_http_server_request_hpp
#define fibio_http_server_request_hpp

#include <memory>
#include <string>
#include <vector>
#include <boost/iostreams/restrict.hpp>
#include <fibio/http/common/cookie.hpp>

namespace fibio {
namespace http {

struct server_request : common::request
{
    void clear();

    bool accept_compressed() const;

    bool read(std::istream& is);

    const common::cookie_map& cookies();

    inline bool has_body() const { return (chunked || content_length > 0) && body_stream_.get(); }

    inline std::istream& raw_stream() const { return *raw_stream_; }

    inline std::istream& body_stream()
    {
        // TODO: Throw if body stream is not setup
        return *(body_stream_.get());
    }

    // Consume and discard body
    void drop_body();

    const std::string& param(const std::string name) const
    {
        static const std::string empty;
        auto i = std::find_if(
            params.begin(), params.end(),
            [&name](const std::pair<std::string, std::string>& e) { return e.first == name; });
        if (i == params.end()) return empty;
        return i->second;
    }

    template <typename T>
    std::istream& operator>>(T& t)
    {
        return body_stream() >> t;
    }

    std::vector<std::pair<std::string, std::string>> params;

    // private:
    std::unique_ptr<boost::iostreams::restriction<std::istream>> restriction_;
    std::unique_ptr<std::istream> body_stream_;
    std::unique_ptr<common::cookie_map> cookies_;

    std::istream* raw_stream_ = nullptr;
};

inline std::istream& operator>>(std::istream& is, server_request& v)
{
    v.read(is);
    return is;
}

inline std::ostream& operator<<(std::ostream& os, server_request& req)
{
    req.write_header(os);
    if (req.has_body()) os << req.body_stream().rdbuf();
    os.flush();
    return os;
}

} // End of namespace http
} // End of namespace fibio

#endif
