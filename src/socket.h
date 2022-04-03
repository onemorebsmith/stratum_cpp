#pragma once

#include <stdlib.h>
#include <string_view>
#include <memory>
#include <optional>

namespace stratum
{
    struct packet{
        constexpr static size_t buff_sz = 1024;
        constexpr std::string_view data() const{
            return std::string_view(&buffer[0]);
        }
        size_t len;
        std::array<char, buff_sz> buffer;
    };

    using maybePacket = std::optional<std::unique_ptr<packet>>;

    class tcp_socket
    {
        public:
            static std::unique_ptr<tcp_socket> Open(std::string_view host, int port);
            ~tcp_socket();   

            bool connect();
            void close();
            bool send(void*data, size_t dataLen);
            maybePacket Receive();
            bool connected() const { return socketHandle > 0; }
        private:
            tcp_socket(std::string_view host, int port);


        int socketHandle;
        const int port;
        const std::string hostname;
    };

    typedef std::unique_ptr<tcp_socket> SocketHandle;


  

} // namespace stratum

