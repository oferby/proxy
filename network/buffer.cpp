#include "../common.h"

namespace Network
{
    
Buffer::Buffer(size_t len) {
    lenght = len;
    message = new char[len];
};

BufferPtr create_buffer(size_t len) {
    return std::make_shared<Buffer>(len);
};





} // namespace Network
