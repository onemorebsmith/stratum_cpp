#include "socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

#include <iostream>

namespace stratum
{
    bool resolve_dns(std::string_view host, addrinfo &out);

    std::unique_ptr<tcp_socket> tcp_socket::Open(std::string_view host, int port)
    {
        return std::unique_ptr<tcp_socket>(new tcp_socket(host, port));
    }

    tcp_socket::tcp_socket(std::string_view _host, int _port)
        : port(_port), hostname(_host)
    {
    }

    tcp_socket::~tcp_socket()
    {
        close();
    }

    bool tcp_socket::connect()
    {
        socketHandle = socket(AF_INET, SOCK_STREAM, 0);
        if (socketHandle == -1)
        {
            return false;
        }

        struct sockaddr_in server = {};
        server.sin_port = htons(port);

        auto resolvedAddr = addrinfo{};
        if (resolve_dns(hostname, resolvedAddr))
        {
            switch (resolvedAddr.ai_family)
            {
            case AF_INET:
                server.sin_family = AF_INET;
                server.sin_addr = ((struct sockaddr_in *)resolvedAddr.ai_addr)->sin_addr;
                break;

            case AF_INET6:
                return false;
                server.sin_family = AF_INET6;
                // server.sin_addr = ((struct sockaddr_in6 *) resolvedAddr.ai_addr)->sin6_addr;
                break;
            }
        }

        std::cout << "connecting: " << inet_ntoa(server.sin_addr) << ":" << port << std::endl;
        if (::connect(socketHandle, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            return false;
            // return 1;
        }

        return true;
    }

    void tcp_socket::close(){
        ::shutdown(socketHandle, 2);
        ::close(socketHandle);
    }

    bool tcp_socket::send(void *data, size_t dataLen)
    {
        const auto sent = ::send(this->socketHandle, data, dataLen, 0);
        if (sent < 0)
        {
            return false;
        }

        return true;
    }

    maybePacket tcp_socket::Receive()
    {
        static thread_local auto workbuffer =  std::array<char, packet::buff_sz>{};
        memset(&workbuffer[0], 0, sizeof(char) * packet::buff_sz);

        maybePacket response = nullptr;
        const auto resp = recv(this->socketHandle, &workbuffer[0], packet::buff_sz, 0);
        if (resp >= 0)
        {
            response = std::make_unique<packet>();
            auto p = response->get();

            p->len = static_cast<size_t>(resp);
            memcpy(&p->buffer[0], &workbuffer[0], sizeof(char) * packet::buff_sz);
        }

        return response;
    }

    bool resolve_dns(std::string_view host, addrinfo &out)
    {
        auto hints = addrinfo{};
        addrinfo *result = nullptr;
        int errcode = 0;
        char addrstr[100] = {};
        void *ptr;

        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags |= AI_CANONNAME;

        errcode = getaddrinfo(host.data(), NULL, &hints, &result);
        if (errcode != 0)
        {
            perror("getaddrinfo");
            return -1;
        }

        out = *result;

        return true;
    }
}
